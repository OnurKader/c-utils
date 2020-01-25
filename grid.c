#include "doub_link_list.h"
#include "quadtree.h"
#include "vec.h"

#include <SDL2/SDL.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define WIDTH  1280U
#define HEIGHT 720U

#define GRID_W 40U
#define GRID_H 40U

#define GRID_ROW HEIGHT / GRID_H
#define GRID_COL WIDTH / GRID_W

typedef vec_t(Point) vec_point_t;

enum state_t
{
	RUNNING,
	QUITTING,
	INPUT,
	OUTPUT
};

typedef struct grid_t
{
	vec_point_t v;
} grid_t;

typedef struct game_t
{
	enum state_t state;
	grid_t grids[GRID_ROW][GRID_COL];
} game_t;

void initGame(game_t* game)
{
	for(uint16_t i = 0U; i < GRID_ROW; ++i)
		for(uint16_t j = 0U; j < GRID_COL; ++j)
		{
			vec_init(&game->grids[i][j].v);
			vec_reserve(&game->grids[i][j].v, 4U);
		}

	game->state = RUNNING;
}

int main(void)
{
	game_t game;
	initGame(&game);

	if(SDL_Init(SDL_INIT_VIDEO))
	{
		fprintf(stderr, "Couldn't initialize SDL: %s\n", SDL_GetError());
		exit(1);
	}

	SDL_Window* window = SDL_CreateWindow("Grid Particle System",
										  SDL_WINDOWPOS_UNDEFINED,
										  SDL_WINDOWPOS_UNDEFINED,
										  WIDTH,
										  HEIGHT,
										  SDL_WINDOW_SHOWN | SDL_WINDOW_BORDERLESS);
	if(!window)
	{
		fprintf(stderr, "Couldn't create window: %s\n", SDL_GetError());
		exit(1);
	}

	SDL_Renderer* render = SDL_CreateRenderer(
		window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);

	if(!render)
	{
		fprintf(stderr, "Couldn't create renderer: %s\n", SDL_GetError());
		SDL_DestroyWindow(window);
		exit(1);
	}

	while(game.state != QUITTING)
	{
		SDL_Event e;
		while(SDL_PollEvent(&e))
		{
			if(e.type == SDL_QUIT)
				game.state = QUITTING;
			else if(e.type == SDL_KEYDOWN)
				switch(e.key.keysym.sym)
				{
					case SDLK_UP: printf("Pressed UP!\n"); break;
					case SDLK_DOWN: printf("Pressed DOWN!\n"); break;
					case SDLK_LEFT: printf("Pressed LEFT!\n"); break;
					case SDLK_RIGHT: printf("Pressed RIGHT!\n"); break;
				}
		}
		// Render
		SDL_SetRenderDrawColor(render, 0U, 0U, 0U, 255U);
		SDL_RenderClear(render);
		SDL_RenderPresent(render);
	}

	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}
