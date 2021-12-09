#include <SDL2/SDL.h>
#include <stdlib.h>
#include "initSDL.h"
#include "draw.h"
#include "actor.h"
#include "map.h"

#define ACTOR_SPEED 0.003

struct actor *actors = 0;
int numActors = 0;

struct actor *addActor(int x, int y, SDL_Texture *tex) {
	numActors++;
	actors = realloc(actors, sizeof(struct actor) * numActors);
	struct actor *a = &actors[numActors-1];

	a->x = x;
	a->y = y;
	a->xo = 0;
	a->yo = 0;
	a->d = 0;
	a->tex = tex;
	a->cooldown = 0;
	return a;
}

void freeActors() {
	free(actors);
	actors = 0;
	numActors = 0;
}

void updateActors(int d) {
	for(int i = 0; i < numActors; i++) {
		struct actor *a = &actors[i];
		if(a->cooldown) {
			a->cooldown -= d;
			if(a->cooldown < 0)
				a->cooldown = 0;
			continue;
		}

		if(a->xo)
			a->xo += (a->xo > 0) ? ACTOR_SPEED*d : -ACTOR_SPEED*d;
		else if(a->yo)
			a->yo += (a->yo > 0) ? ACTOR_SPEED*d : -ACTOR_SPEED*d;

		if(a->xo <= -1.0) { a->xo = 0; a->x--; }
		if(a->xo >= 1.0) { a->xo = 0; a->x++; }
		if(a->yo <= -1.0) { a->yo = 0; a->y--; }
		if(a->yo >= 1.0) { a->yo = 0; a->y++; }
	}
}

void drawActors() {
	for(int i = 0; i < numActors; i++) {
		struct actor *a = &actors[i];
		SDL_Rect src = (SDL_Rect){0, a->d*16, 16, 16};
		if(a->xo || a->yo) {
			int step = (a->xo + a->yo) * 4;
			if(step < 0)
				step *= -1;
			if(step%2)
				src.x = 16 + (step/2) * 16;
		}
		float x = (float)a->x+a->xo, y = (float)a->y+a->yo;
		SDL_Rect dst = (SDL_Rect){(x-y)*8-xo, ((x+y)/2)*8-yo, 16, 16};
		addClip(a->tex, src, dst);
	}
}

struct actor *actorAt(int x, int y) {
	for(int i = 0; i < numActors; i++) {
		struct actor *a = &actors[i];
		if(a->x == x && a->y == y)
			return a;
	}
	return 0;
}

void moveActor(struct actor *a, int x, int y) {
	if(a->cooldown)
		return;
	if(a->xo || a->yo)
		return;
	if(!x && !y)
		return;
	if(x && y)
		return;
	int od = a->d;
	if(y == -1)
		a->d = 0;
	if(x == 1)
		a->d = 1;
	if(y == 1)
		a->d = 2;
	if(x == -1)
		a->d = 3;
	if(a->d != od) {
		a->cooldown = 100;
		return;
	}
	int dx = a->x+x, dy = a->y+y;
	if(mapBlocked(dx, dy))
		return;
	if(actorAt(dx, dy))
		return;
	a->xo = x*ACTOR_SPEED;
	a->yo = y*ACTOR_SPEED;
}

void controlActor(struct actor *a) {
	if(a->cooldown)
		return;
	if(a->xo || a->yo)
		return;
	if(a->cooldown)
		return;

	int x = 0, y = 0;
	if(keyboardState[SDL_SCANCODE_UP])
		x = -1;
	else if(keyboardState[SDL_SCANCODE_DOWN])
		x = 1;
	else if(keyboardState[SDL_SCANCODE_LEFT])
		y = 1;
	else if(keyboardState[SDL_SCANCODE_RIGHT])
		y = -1;

	if(x || y) {
		moveActor(a, x, y);
		return;
	}

	int mx, my;
	if(SDL_GetMouseState(&mx, &my)&SDL_BUTTON_LMASK) {
		float scale;
		SDL_RenderGetScale(renderer, &scale, NULL);
		mx /= scale;
		my /= scale;
		mx += xo - 4;
		my += yo - 8;
		x = (2*my+mx)/2;
		y = (2*my-mx)/2;
		x /= 8;
		y /= 8;
		actorNav(a, x, y);
	}
}

void actorNav(struct actor *a, int tx, int ty) {
	if(a->xo || a->yo)
		return;
	if(a->cooldown)
		return;
	if(a->x == tx && a->y == ty)
		return;
	if(tx < 0 || ty < 0 || tx >= mapW || ty >= mapH)
		return;
	if(mapBlocked(tx, ty))
		return;

	int *pmap = malloc(sizeof(int)*mapW*mapH);
	for(int i = 0; i < mapW*mapH; i++)
		pmap[i] = (tileBlocks(map[i]) || actorAt(i%mapW, i/mapW))*-1;
	pmap[a->y*mapW+a->x] = 1;
	int g = 1;
	while(pmap[ty*mapW+tx] == 0) {
		if(g > 500)
			return;
		for(int i = 0; i < mapW*mapH; i++)
			if(pmap[i] == g) {
				int x = i%mapW, y = i/mapW;
				for(int xm = -1; xm <= 1; xm++)
					for(int ym = -1; ym <= 1; ym++) {
						if(xm && ym) continue;
						if(!xm && !ym) continue;
						int dx = x+xm, dy = y+ym;
						if(dx < 0 || dy < 0 || dx >= mapW || dy >= mapH)
							continue;
						if(pmap[dy*mapW+dx])
							continue;
						pmap[dy*mapW+dx] = g+1;
					}
			}
		g++;
	}

	int x = tx, y = ty;
	int xm, ym;
	while(x != a->x || y != a->y) {
		xm = 0;
		ym = 0;
		for(int sxm = -1; sxm <= 1 && xm == ym; sxm++)
			for(int sym = -1; sym <= 1 && xm == ym; sym++) {
				if(sxm && sym) continue;
				if(!sxm && !sym) continue;
				int dx = x+sxm, dy = y+sym;
				if(dx < 0 || dy < 0 || dx >= mapW || dy >= mapH)
					continue;
				if(pmap[dy*mapW+dx] == g-1) {
					xm = -sxm;
					ym = -sym;
					y += sym;
					x += sxm;
				}
			}
		g--;
	}

	moveActor(a, xm, ym);
}
