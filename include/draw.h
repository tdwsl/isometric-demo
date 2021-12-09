#ifndef DRAW_H
#define DRAW_H

#include <SDL2/SDL.h>

extern int xo, yo;

void addClip(SDL_Texture *tex, SDL_Rect src, SDL_Rect dst);
void draw();

#endif
