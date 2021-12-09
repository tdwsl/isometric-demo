#ifndef ACTOR_H
#define ACTOR_H

struct actor {
	int x, y, d;
	float xo, yo;
	SDL_Texture *tex;
	int cooldown;
};

extern struct actor *actors;
extern int numActors;

struct actor *addActor(int x, int y, SDL_Texture *tex);
void drawActors();
void controlActor(struct actor *a);
void updateActors(int d);
void freeActors();
struct actor *actorAt(int x, int y);
void actorNav(struct actor *a, int tx, int ty);

#endif
