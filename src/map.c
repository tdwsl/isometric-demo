#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "draw.h"
#include "initSDL.h"
#include "map.h"
#include "actor.h"

int *map=0, mapW, mapH;
SDL_Texture *tileset;

void loadTileset() {
	tileset = loadTexture("img/tileset.bmp");
}

void loadMap(const char *filename) {
	FILE *fp = fopen(filename, "r");
	assert(fp);
	fscanf(fp, "%d%d", &mapW, &mapH);
	map = malloc(sizeof(int)*mapW*mapH);
	for(int i = 0; i < mapW*mapH; i++)
		fscanf(fp, "%d", &map[i]);
	fclose(fp);
}

void saveMap(const char *filename) {
	FILE *fp = fopen(filename, "w");
	assert(fp);
	fprintf(fp, "%d %d\n", mapW, mapH);
	for(int y = 0; y < mapH; y++) {
		for(int x = 0; x < mapW; x++)
			fprintf(fp, "%d ", map[y*mapW+x]);
		fprintf(fp, "\b\n");
	}
	fprintf(fp, "\b");
	fclose(fp);
}

int tileBlocks(int t) {
	switch(t) {
		case TILE_WATER:
		case TILE_WOODWALL:
		case TILE_TREE:
			return 1;
		default:
			return 0;
	}
}

void drawMap() {
	for(int i = 0; i < mapW*mapH; i++) {
		int x = i%mapW, y = i/mapW;
		SDL_Rect src = (SDL_Rect){0, 0, 16, 16};
		SDL_Rect dst = (SDL_Rect){(x-y)*8-xo, ((x+y)*8)/2-yo, 16, 16};
		int wall = 0;
		switch(map[i]) {
			case TILE_GRASS: break;
			case TILE_SAND: src.x = 16; break;
			case TILE_WATER:
				src.x = 2+((SDL_GetTicks()/350)+((x+y)%2)*2+y%2)%4;
				src.y = (src.x/4)*16;
				src.x = (src.x%4)*16;
				break;
			case TILE_WOODFLOOR: src.x = 32; src.y = 16; break;
			case TILE_WOODWALL: src.x = 48; src.y = 16; wall = 1; break;
			case TILE_TREE: src.x = 0; src.y = 32; src.h = 32; dst.h = 32; dst.y -= 16; wall = 1; break;
			default: break;
		}
		if(wall)
			addClip(tileset, src, dst);
		else
			SDL_RenderCopy(renderer, tileset, &src, &dst);
	}
}

int mapBlocked(int x, int y) {
	if(x < 0 || y < 0 || x >= mapW || y >= mapH)
		return 1;
	return(tileBlocks(map[y*mapW+x]));
}
