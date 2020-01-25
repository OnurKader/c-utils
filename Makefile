all:
	gcc -o main main.c -O2 -g -lm -std=c11 -Wall -Wextra -Wshadow -Wpedantic

sdl:
	gcc -o grid grid.c -O2 -g -lm -std=c11 -Wall -Wextra -Wshadow -Wpedantic `sdl2-config --cflags --libs`
