#include <SDL2/SDL.h>
#include <stdlib.h>
#include <assert.h>
#include "initSDL.h"

SDL_Renderer *renderer;
SDL_Window *window;

SDL_Texture **textures = 0;
int numTextures = 0;

const Uint8 *keyboardState;

void initSDL() {
	assert(SDL_Init(SDL_INIT_EVERYTHING) >= 0);
	assert(window = SDL_CreateWindow("CharmIsle", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_RESIZABLE));
	assert(renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE));
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xff);
	SDL_RenderSetIntegerScale(renderer, SDL_FALSE);
	keyboardState = SDL_GetKeyboardState(NULL);
}

SDL_Texture *loadTexture(const char *filename) {
	numTextures++;
	textures = realloc(textures, numTextures*sizeof(SDL_Texture*));
	SDL_Texture *tex = textures[numTextures+1];
	SDL_Surface *surf = SDL_LoadBMP(filename);
	assert(surf);
	SDL_SetColorKey(surf, SDL_TRUE, SDL_MapRGB(surf->format, 0xff, 0, 0xff));
	assert(tex = SDL_CreateTextureFromSurface(renderer, surf));
	SDL_FreeSurface(surf);
	return tex;
}

void endSDL() {
	for(int i = 0; i < numTextures; i++)
		SDL_DestroyTexture(textures[i]);
	free(textures);
	textures = 0;
	numTextures = 0;

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}
