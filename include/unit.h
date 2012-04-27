#ifndef _UNIT_H_
#define _UNIT_H_

#include <math.h>
#include "SDL.h"
#include "animation.h"
#include "tools.h"

typedef struct {
	Uint16 *xCoords;
	Uint16 *yCoords;
	int numNodes;
	int maxNodes;
	int currentPath;
} UnitPath;

typedef struct {
	char *name;
	char *lumpName;
	Uint16 health;
	Uint16 armor;
} UnitDefinition;

struct UnitStruct {
	char *name;
	AnimationSet *running;
	Uint16 health;
	Uint16 armor;
	Uint8 speed;
	Uint16 x;
	Uint16 y;
	UnitPath *path;
	int rotation;
	struct UnitStruct *next;
};
typedef struct UnitStruct Unit;

typedef enum {
	PLAYER, IMP, SOLDIER, SHOTGUNNER, PINKY, LOSTSOUL, CACODEMON,
	HELLKNIGHT, BARONOFHELL, ARACHNOTRON, CYBERDEMON
} UnitTypes;

enum impFrames {
	TROOA1, TROOA2A8, TROOA3A7, TROOA4A6, TROOA5, TROOB1,
	TROOB2B8, TROOB3B7, TROOB4B6, TROOB5, TROOC1, TROOC2C8,
	TROOC3C7, TROOC4C6, TROOC5, TROOD1, TROOD2D8, TROOD3D7,
	TROOD4D6, TROOD5, TROOE1, TROOE2E8, TROOE3E7, TROOE4E6,
	TROOE5, TROOF1, TROOF2F8, TROOF3F7, TROOF4F6, TROOF5,
	TROOG1, TROOG2G8, TROOG3G7, TROOG4G6, TROOG5, TROOH1,
	TROOH2H8, TROOH3H7, TROOH4H6, TROOH5, TROOI0, TROOJ0,
	TROOK0, TROOL0, TROOM0, TROON0, TROOO0, TROOP0, TROOQ0,
	TROOR0, TROOS0, TROOT0, TROOU0, TROOCOUNT
};

Unit *newUnit(UnitTypes unitType, int x, int y);
void unitDraw(Unit *unit);
void unitInit();
void unitMove(Unit *unit);
void unitFindTarget(Unit *unit, Unit *target);

#endif
