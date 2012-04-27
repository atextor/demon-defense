#include "input.h"

bool checkInput() {
	static SDL_Event event;

	while (SDL_PollEvent(&event)) {
		/* closed window */
		if ( event.type == SDL_QUIT ) {
 			return false;
		}

		/* mouse click */
		if ( event.type == SDL_MOUSEBUTTONDOWN ) {
			return false;
		}

		/* key press */
		if ( event.type == SDL_KEYDOWN ) {
			switch(event.key.keysym.sym) {
				case SDLK_SPACE:
					break;
				case SDLK_ESCAPE:
					return false;
				default: break;
			}
		}
	}

	return true;
}

