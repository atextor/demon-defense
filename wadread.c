#include "wadread.h"

#define OFFSETCACHESIZE 5
static Uint32 palette[256];
static FILE *wadFile = NULL;
static LumpMap *lumpMap;
static int lumpMapLength;

static int wadReadLong(long *value) { 
	/* FIXME: make this work on big-endian */
	*value = getc(wadFile)
		| (getc(wadFile) <<  8)
		| (getc(wadFile) << 16)
		| (getc(wadFile) << 24);
	return feof(wadFile) || ferror(wadFile);
}

static short wadReadShort(short *value) {
	/* FIXME: make this work on big-endian */
	*value = getc(wadFile)
		| (getc(wadFile) << 8);
	return feof(wadFile) || ferror(wadFile);
}

static void wadReadLumpPalette(long offset) {
	int i;
	Uint8 red;
	Uint8 green;
	Uint8 blue;

	fseek(wadFile, offset, SEEK_SET);
	for (i = 0; i < 256; i++) {
		red = (Uint8)getc(wadFile);
		green = (Uint8)getc(wadFile);
		blue = (Uint8)getc(wadFile);
		palette[i] = sgCreateColor(red, green, blue);
	}
}

static void wadReadDir(long numLumps, long dirOffset) {
	int i;
	int result;
	long lumpOffset;
	long lumpSize;
	char *lumpName;
	fpos_t filepos;

	lumpName = (char*)calloc(1, 9);
	lumpMap = (LumpMap*)malloc(sizeof(LumpMap) * numLumps);
	lumpMapLength = numLumps;

	fseek(wadFile, dirOffset, SEEK_SET);

	/* read all lumps and build the map */
	for (i = 0; i < numLumps; i++) {
		result = wadReadLong(&lumpOffset);
		assert(result == 0);
		result = wadReadLong(&lumpSize);
		assert(result == 0);
		result = fread(lumpName, 8, 1, wadFile);
		assert(result == 1);

		strncpy(lumpMap[i].lumpName, lumpName, 8);
		lumpMap[i].lumpName[8] = 0;
		lumpMap[i].lumpOffset = lumpOffset;
		lumpMap[i].lumpSize = lumpSize;

		/* no need to search again for it later */
		if (strcmp(lumpName, "PLAYPAL") == 0) {
			fgetpos(wadFile, &filepos);
			wadReadLumpPalette(lumpOffset);
			fsetpos(wadFile, &filepos);
		}
	}

	free(lumpName);
}

static long wadLumpNameToOffset(char *lumpName) {
	/* Maybe TODO: make list sorted (add qsort to wadReadDir),
	 * then perform a binary search instead of iterating the list.
	 * But it's not really needed; there are ~2000 lumps,
	 * running through them doesn't take that long... */
	int i;
	int stringLength;
	static LumpMap cache[OFFSETCACHESIZE];

	stringLength = strlen(lumpName);

	/* first, search the cache */
	for (i = 0; i < OFFSETCACHESIZE; i++) {
		if (strncmp(cache[i].lumpName, lumpName, stringLength) == 0) {
			return cache[i].lumpOffset;
		}
	}

	/* not found in cache; age cache and search in lumpMap */
	for (i = 0; i < OFFSETCACHESIZE - 1; i++) {
		strncpy(cache[i].lumpName, cache[i + 1].lumpName, 8);
		cache[i].lumpOffset = cache[i + 1].lumpOffset;
		cache[i].lumpSize = cache[i + 1].lumpSize;
	}

	for (i = 0; i < lumpMapLength; i++) {
		if (strncmp(lumpMap[i].lumpName, lumpName, stringLength) == 0) {
			/* entry found. put into cache and return */
			strncpy(cache[OFFSETCACHESIZE - 1].lumpName, lumpName, 8);
			cache[OFFSETCACHESIZE - 1].lumpOffset = lumpMap[i].lumpOffset;
			cache[OFFSETCACHESIZE -1 ].lumpSize = lumpMap[i].lumpSize;
			return lumpMap[i].lumpOffset;
		}
	}

	fprintf(stderr, "Lump Name '%s' not found in WAD, aborting\n", lumpName);
	exit(EXIT_FAILURE);
	return 0;
}

SDL_Surface *wadReadLumpImage(char *lumpName, bool revert) {
	int i;
	int j;
	fpos_t filepos;
	unsigned char byte;
	unsigned char pixelcolor;
	unsigned char numpixels;
	short image_width;
	short image_height;
	short image_leftoffset;
	short image_topoffset;
	long image_row;
	long offset;
	SDL_Surface *image;

	/* first of all, find the offset of the desired lump */
	offset = wadLumpNameToOffset(lumpName);

	/* read image lump header */
	fseek(wadFile, offset, SEEK_SET);
	assert(wadReadShort(&image_width) == 0);
	assert(wadReadShort(&image_height) == 0);
	assert(wadReadShort(&image_leftoffset) == 0);
	assert(wadReadShort(&image_topoffset) == 0);

	/* create image */
	image = sgCreateSurface(image_width, image_height);

	/* read image columns from lump */
	for (i = 0; i < image_width; i++) {
		assert(wadReadLong(&image_row) == 0);
		fgetpos(wadFile, &filepos);
		fseek(wadFile, offset + image_row, SEEK_SET);

		/* get the byte indicating the vertical offset
		 * of the first post */
		byte = (unsigned char)getc(wadFile);
		/* then read posts until no more post is found */
		while (byte != 0xff) {
			/* how many pixels to draw downward */
			numpixels = (unsigned char)getc(wadFile);

			/* ignore leading byte */
			getc(wadFile);

			/* read pixel data */
			for (j = 0; j < (int)numpixels; j++) {
				pixelcolor = (unsigned char)getc(wadFile);
				sgPutPixel(image, revert ? image_width - 1 - i : i,
					byte + j, palette[pixelcolor]);
			}
			/* ignore trailing byte */
			getc(wadFile);

			/* get next byte */
			byte = (unsigned char)getc(wadFile);
		}
		
		fsetpos(wadFile, &filepos);
	}

	return image;
}

int wadInit(char *wadFileName) {
	char wadHeaderType[5] = "xWAD";
	long wadHeaderNumLumps;
	long wadHeaderDirOffset;
	int result;
	void *buf;

	buf = calloc(1, 5);
	if (buf == NULL) {
		perror("wadInit");
		exit(EXIT_FAILURE);
	}

	wadFile = fopen(wadFileName, "r");
	if (wadFile == NULL) {
		perror("wadInit");
		exit(EXIT_FAILURE);
	}

	assert(wadFile != NULL);

	/* read header */
	result = fread(buf, 4, 1, wadFile);
	assert(result == 1);
	strncpy(wadHeaderType, buf, 4);
	if (strcmp(wadHeaderType, "IWAD") != 0) {
		fprintf(stderr, "Error: %s is no valid WAD file\n", wadFileName);
		exit(EXIT_FAILURE);
	}

	/* read lumps */
	result = wadReadLong(&wadHeaderNumLumps);
	assert(result == 0);
	result = wadReadLong(&wadHeaderDirOffset);
	assert(result == 0);

	/* now process the directory */
	wadReadDir(wadHeaderNumLumps, wadHeaderDirOffset);
	free(buf);

	return 0;
}

