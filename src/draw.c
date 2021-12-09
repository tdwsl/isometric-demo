#include <SDL2/SDL.h>
#include <stdlib.h>
#include "initSDL.h"
#include "map.h"
#include "draw.h"
#include "actor.h"

int xo = 0, yo = 0;

struct clip {
	SDL_Texture *tex;
	SDL_Rect src, dst;
};

struct clip *clips;
int numClips = 0;

void addClip(SDL_Texture *tex, SDL_Rect src, SDL_Rect dst) {
	clips[numClips] = (struct clip){tex, src, dst};
	numClips++;
}

void draw() {
	SDL_RenderClear(renderer);
	int w, h;
	SDL_GetWindowSize(window, &w, &h);

	int xs = (w+160)/320, ys = (h+120)/240;
	int scale = (w < h) ? ys : xs;
	if(!scale)
		scale = 1;
	SDL_RenderSetScale(renderer, scale, scale);
	w /= scale;
	h /= scale;

	xo = (mapW/2-mapH/2)*16+8 - w/2;
	yo = ((mapW/2+mapH/2)/2)*8+8 - h/2;
	numClips = 0;
	clips = malloc(sizeof(struct clip)*(mapW*mapH+numActors*2));
	drawMap();
	drawActors();
	int f;
	do {
		f = 0;
		for(int i = 1; i < numClips; i++)
			if(clips[i-1].dst.y+clips[i-1].dst.h > clips[i].dst.y+clips[i].dst.h) {
				struct clip c = clips[i];
				clips[i] = clips[i-1];
				clips[i-1] = c;
				f = 1;
			}
	} while(f);
	for(int i = 0; i < numClips; i++)
		SDL_RenderCopy(renderer, clips[i].tex, &clips[i].src, &clips[i].dst);
	free(clips);

	SDL_RenderPresent(renderer);
}
