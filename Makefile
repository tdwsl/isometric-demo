CC = gcc

charmisle: obj/map.o obj/initSDL.o obj/charmisle.o obj/draw.o obj/actor.o
	$(CC) obj/*.o -Iinclude -lSDL2 -lm -o charmisle

obj/map.o: src/map.c
	$(CC) -c src/map.c -Iinclude -o obj/map.o
obj/initSDL.o: src/initSDL.c
	$(CC) -c src/initSDL.c -Iinclude -o obj/initSDL.o
obj/charmisle.o: src/charmisle.c
	$(CC) -c src/charmisle.c -Iinclude -o obj/charmisle.o
obj/draw.o: src/draw.c
	$(CC) -c src/draw.c -Iinclude -o obj/draw.o
obj/actor.o: src/actor.c
	$(CC) -c src/actor.c -Iinclude -o obj/actor.o

clean:
	rm obj/* charmisle
