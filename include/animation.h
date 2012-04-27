#ifndef _ANIMATION_H_
#define _ANIMATION_H_

#include <stdbool.h>
#include <time.h>
#include <stdlib.h>
#include "simplegfx.h"

/* don't include wadread.h to resolve cyclic dependency,
 * but we still need to know about this funtion */
SDL_Surface *wadReadLumpImage(char *lumpName, bool revert);

/* Holds numFrames images for one animation of one object. */
typedef struct {
	SDL_Surface **frames;
	Uint8 numFrames;
	Uint8 usedFrames;
} Animation;

/* An AnimationSet contains eight animations, one
 * for each direction of the object
        3
      4 | 2
       \|/
     5--*----> 1   Thing is facing this direction
       /|\
      6 | 8
        7
*/
typedef struct {
	Animation **animations;
	int numAnimations;
	int usedAnimations;
	int currentFrame;
	int frameRepeat;
} AnimationSet;

Animation *newAnimation(Uint8 numFrames);
AnimationSet *newAnimationSet(Uint8 numAnimations, Uint8 startFrame);
void deleteAnimation(Animation *animation);
void deleteAnimationSet(AnimationSet *animationSet);
void animationAddFrame(Animation *animation, char *imageName, bool revert);
void animationSetAddAnimation(AnimationSet *animationSet,
	Animation *animation);
SDL_Surface *animationSetGetFrame(AnimationSet *animationSet, int ani);

#endif

