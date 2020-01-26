main:
	gcc -o main main.c vec.o -O2 -g -lm -std=c11 -Wall -Wextra -Wshadow -Wpedantic

sdl:
	gcc -o grid grid.c vec.o -O2 -g -lm -std=c11 -Wall -Wextra -Wshadow -Wpedantic `sdl2-config --cflags --libs` -lSDL2_gfx
