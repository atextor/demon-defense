#include "unit.h"

#define NUMUNITDEFINITIONS 11
static const UnitDefinition unitDefinitions[NUMUNITDEFINITIONS] = {
	{ "Player", "PLAY", 100, 0},
	{ "Imp", "TROO", 100, 0},
	{ "Soldier", "POSS", 100, 0},
	{ "Shotgunner", "SPOS", 100, 0},
	{ "Pinky", "SARG", 100, 0}, 
	{ "Lost Soul", "SKUL", 100, 0},
	{ "Cacodemon", "HEAD", 100, 0},
	{ "Hell Knight", "BOS2", 100, 0},
	{ "Baron of Hell", "BOSS", 100, 0},
	{ "Arachnotron", "SPID", 100, 0},
	{ "Cyber Demon", "CYBR", 100, 0}
};

static AnimationSet **unitAnimationSets;

static UnitPath *newUnitPath(int numNodes) {
	UnitPath *result = (UnitPath*)malloc(sizeof(UnitPath));
	result->xCoords = (Uint16*)calloc(numNodes, sizeof(Uint16));
	result->yCoords = (Uint16*)calloc(numNodes, sizeof(Uint16));
	result->numNodes = 0;
	result->maxNodes = numNodes;

	return result;
}

static void unitPathAddNode(UnitPath *path, int x, int y) {
	if (path->numNodes < path->maxNodes) {
		path->xCoords[path->numNodes] = x;
		path->yCoords[path->numNodes] = y;
		path->currentPath = path->numNodes;
		if (path->numNodes < path->maxNodes) {
			path->numNodes++;
		}
	}
}

static char *lumpName(char *buf, UnitTypes unitType, int direction,
	char frame) {

	char *unitName = unitDefinitions[unitType].lumpName;

	switch (direction) {
		case 1:
		case 5: sprintf(buf, "%s%c%d", unitName, frame, direction);
			break;
		case 2:
		case 8: sprintf(buf, unitType == LOSTSOUL ? "%s%c8%c2" : "%s%c2%c8",
				unitName, frame, frame);
			break;
		case 3:
		case 7: sprintf(buf, unitType == LOSTSOUL ? "%s%c7%c3" : "%s%c3%c7",
				unitName, frame, frame);
			break;
		case 4:
		case 6: sprintf(buf, unitType == LOSTSOUL ? "%s%c6%c4" : "%s%c4%c6",
				unitName, frame, frame);
			break;
	}

	return buf;
}

void unitInit() {
	unitAnimationSets = (AnimationSet**)calloc(NUMUNITDEFINITIONS,
		sizeof(AnimationSet*));
}

Unit *newUnit(UnitTypes unitType, int x, int y) {
	int i;
	int j;
	Unit *result;
	UnitPath *path;
	Animation *animation;
	AnimationSet *animationSet = unitAnimationSets[unitType];
	char tmpString[9] = "       ";

	if (animationSet == NULL) {
		animationSet = newAnimationSet(8, 0);

		for (i = 1; i <= 8; i++) {
			animation = newAnimation(4);
			for (j = (int)'A'; j <= ((int)'D'); j++) {
				animationAddFrame(animation, lumpName(tmpString, unitType,
					i, (char)j), (i > 5));
			}
			animationSetAddAnimation(animationSet, animation);
		}
	}

	path = newUnitPath(16);
	unitPathAddNode(path, 75, 480);
	unitPathAddNode(path, 270, 480);
	unitPathAddNode(path, 270, 265);
	unitPathAddNode(path, 200, 210);
	unitPathAddNode(path, 200, 120);
	unitPathAddNode(path, 270, 50);
	unitPathAddNode(path, 680, 50);
	unitPathAddNode(path, 725, 120);
	unitPathAddNode(path, 725, 210);
	unitPathAddNode(path, 680, 265);
	unitPathAddNode(path, 470, 265);
	unitPathAddNode(path, 410, 300);
	unitPathAddNode(path, 410, 420);
	unitPathAddNode(path, 480, 480);
	unitPathAddNode(path, 790, 480);
	unitPathAddNode(path, 75, 0);

	path->currentPath = 0;

	result = (Unit*)malloc(sizeof(Unit));
	result->name = unitDefinitions[unitType].name;
	result->running = animationSet;
	result->health = unitDefinitions[unitType].health;
	result->armor = unitDefinitions[unitType].armor;
	result->x = x;
	result->y = y;
	result->speed = 4;
	result->path = path;
	result->rotation = 0;

	return result;
}

static void unitLookAt(Unit *unit, int x, int y) {
	double x1, y1, x2, y2;
	double angle;
	int phase;

	x1 = (double)unit->x;
	y1 = -1.0;
	x2 = (double)x - x1;
	y2 = (double)unit->y - (double)y;
	angle = (double)((x1 * x2) + (y1 * y2)) /
		(double)(sqrt(x1 * x1 + y1 *y1) * sqrt(x2 * x2 +y2 * y2));
	angle = acos(angle);
	if (x1 * y2 - y1 * x1 > 0) angle *= -1;
	phase = (int)((angle * 4.0 / M_PI - 0.08)+23) % 8;

	unit->rotation = phase;
}

void unitFindTarget(Unit *unit, Unit *target) {
	unitLookAt(unit, target->x + 20, target->y - 40);
}

void unitMove(Unit *unit) {
	double factor;
	int x0, y0, x1, y1, ax, ay;
	UnitPath *p = unit->path;

	if (p->currentPath != -1) {
		/* move the unit */
		x0 = unit->x;
		y0 = unit->y;
		x1 = p->xCoords[p->currentPath];
		y1 = p->yCoords[p->currentPath];
		ax = x1 - x0;
		ay = y1 - y0;
		factor = (double)unit->speed / (double)sqrt(ax * ax + ay * ay);
		unit->x += (int)(factor * ax);
		unit->y += (int)(factor * ay);
		if ((abs(unit->x - x1) < unit->speed) &&
			(abs(unit->y - y1) < unit->speed)) {
			p->currentPath++;
			if (p->currentPath >= p->numNodes) {
				p->currentPath = 0;
			}
		}

		/* set the appropriate animation direction */
		unitLookAt(unit, p->xCoords[p->currentPath],
			p->yCoords[p->currentPath]);
	}

}

void unitDraw(Unit *unit) {
	sgDrawImage(animationSetGetFrame(unit->running, unit->rotation),
		unit->x, unit->y);
}

