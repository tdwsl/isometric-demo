#ifndef MAP_H
#define MAP_H

#define freeMap() if(map) free(map)

enum {
	TILE_GRASS=0,
	TILE_SAND,
	TILE_WATER,
	TILE_WOODFLOOR,
	TILE_WOODWALL,
	TILE_TREE,
};

extern int *map, mapW, mapH;

void loadTileset();
void loadMap(const char *filename);
void saveMap(const char *filename);
void drawMap();
int mapBlocked(int x, int y);
int tileBlocks(int t);

#endif
