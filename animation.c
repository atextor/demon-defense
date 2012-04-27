#include "animation.h"

static const int frameRepeat = 5;

void deleteAnimation(Animation *animation) {
	int i;
	
	for (i = 0; i < animation->numFrames; i++) {
		if (animation->frames[i] != NULL) {
			SDL_FreeSurface(animation->frames[i]);
		}
	}

	free(animation->frames);
	free(animation);
}

void deleteAnimationSet(AnimationSet *animationSet) {
	int i;

	for (i = 0; i < animationSet->numAnimations; i++) {
		if (animationSet->animations[i] != NULL) {
			deleteAnimation(animationSet->animations[i]);
		}
	}

	free(animationSet->animations);
	free(animationSet);
}

Animation *newAnimation(Uint8 numFrames) {
	Animation *result;

	result = (Animation*)malloc(sizeof(Animation));
	result->numFrames = numFrames;
	result->usedFrames = 0;
	result->frames = (SDL_Surface**)calloc(numFrames, sizeof(SDL_Surface*));

	return result;
}

AnimationSet *newAnimationSet(Uint8 numAnimations, Uint8 startFrame) {
	AnimationSet *result;

	result = (AnimationSet*)malloc(sizeof(AnimationSet));
	result->numAnimations = numAnimations;
	result->usedAnimations = 0;
	result->currentFrame = startFrame;
	result->frameRepeat = (rand() % frameRepeat);
	result->animations =
		(Animation**)calloc(numAnimations, sizeof(Animation*));

	return result;
}

void animationAddFrame(Animation *animation, char *imageName, bool revert) {
	if (animation->usedFrames < animation->numFrames) {
		animation->frames[animation->usedFrames] =
			wadReadLumpImage(imageName, revert);
		animation->usedFrames = animation->usedFrames + 1;
	}
}

void animationSetAddAnimation(AnimationSet *animationSet,
	Animation *animation) {

	if (animationSet->usedAnimations < animationSet->numAnimations) {
		animationSet->animations[animationSet->usedAnimations] = animation;
		animationSet->usedAnimations = animationSet->usedAnimations + 1;
	}
}

SDL_Surface *animationSetGetFrame(AnimationSet *animationSet, int ani) {
	SDL_Surface *result;

	if (animationSet->frameRepeat < frameRepeat) {
		result = animationSet->animations[ani]->
			frames[animationSet->currentFrame];
		animationSet->frameRepeat = animationSet->frameRepeat + 1;
	} else {
		animationSet->frameRepeat = 0;
		animationSet->currentFrame = animationSet->currentFrame + 1;
		if (animationSet->currentFrame >=
			animationSet->animations[ani]->usedFrames) {

			animationSet->currentFrame = 0;
		}
		result = animationSet->animations[ani]->
			frames[animationSet->currentFrame];
	}

	return result;
}


