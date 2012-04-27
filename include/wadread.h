#ifndef _WADREAD_H_
#define _WADREAD_H_

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>
#include "simplegfx.h"
#include "unit.h"

typedef struct {
	char lumpName[9];
	long lumpOffset;
	long lumpSize;
} LumpMap;

int wadInit(char *wadFileName);
SDL_Surface *wadReadLumpImage(char *lumpName, bool revert);

#endif

