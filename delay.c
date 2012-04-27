#include "delay.h"

static Uint32 timeLeft() {
	static Uint32 nextTime = 0;
	Uint32 now;

	now = SDL_GetTicks();
	if (nextTime <= now) {
		nextTime = now + 50;
		return 0;
	}
	return (nextTime - now);
}


void loopDelay() {
	SDL_Delay(timeLeft());
}

