#ifndef _SIMPLEGFX_H_
#define _SIMPLEGFX_H_

#include <stdbool.h>
#include "tools.h"
#include "SDL.h"

int sgInit(bool fullscreen);
SDL_Surface *sgCreateSurface(int width, int height);
Uint32 sgCreateColor(Uint8 r, Uint8 g, Uint8 b);
void sgPutPixel(SDL_Surface *surface, int x, int y, Uint32 color);
void sgDrawImage(SDL_Surface *img, int x, int y);
void sgUpdateScreen();
void sgClearScreen();
void sgDrawText(char *text, int x, int y, Uint32 color);

#endif

