#include <SDL2/SDL.h>
#include "initSDL.h"
#include "map.h"
#include "draw.h"
#include "actor.h"

int main() {
	initSDL();
	loadMap("sav/level1.txt");
	loadTileset();
	struct actor *player = addActor(2, 2, loadTexture("img/player.bmp"));

	int quit = 0;
	int lastUpdate = SDL_GetTicks();

	while(!quit) {
		SDL_Event ev;
		while(SDL_PollEvent(&ev))
			switch(ev.type) {
				case SDL_QUIT: quit = 1; break;
			}

		int currentTime = SDL_GetTicks();
		int d = currentTime - lastUpdate;
		lastUpdate = currentTime;

		updateActors(d);
		controlActor(player);

		draw();
	}

	freeActors();
	freeMap();
	endSDL();
	return 0;
}
