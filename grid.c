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

#define GRID_LENGTH GRID_COL* GRID_ROW

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

struct Grid
{
	grid_t grids[GRID_ROW][GRID_COL];
	SDL_Rect rect_array[GRID_LENGTH];
};

typedef struct game_t
{
	enum state_t state;
	struct Grid grid;
} game_t;

void initGame(game_t* game)
{
	for(uint16_t i = 0U; i < GRID_ROW; ++i)
		for(uint16_t j = 0U; j < GRID_COL; ++j)
		{
			vec_init(&game->grid.grids[i][j].v);
			vec_reserve(&game->grid.grids[i][j].v, 4U);
			game->grid.rect_array[j + i * GRID_COL] =
				(SDL_Rect){j * GRID_W, i * GRID_H, GRID_W, GRID_H};
		}

	game->state = RUNNING;
}

void deinitGame(game_t* game)
{
	for(uint16_t i = 0U; i < GRID_ROW; ++i)
		for(uint16_t j = 0U; j < GRID_COL; ++j)
			vec_deinit(&game->grid.grids[i][j].v);

	game->state = QUITTING;
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
			switch(e.type)
			{
				case SDL_QUIT: deinitGame(&game); break;
				case SDL_KEYDOWN:
					switch(e.key.keysym.sym)
					{
						case SDLK_UP: printf("UP!\n"); break;
						case SDLK_DOWN: printf("DOWN!\n"); break;
						case SDLK_LEFT: printf("LEFT!\n"); break;
						case SDLK_RIGHT: printf("RIGHT!\n"); break;
					}
					break;
				default: break;
			}
		}
		// Render
		SDL_SetRenderDrawColor(render, 0U, 0U, 0U, 255U);
		SDL_RenderClear(render);

		SDL_SetRenderDrawColor(render, 255U, 255U, 255U, 255U);
		SDL_RenderDrawRects(render, game.grid.rect_array, GRID_LENGTH);

		SDL_RenderPresent(render);
	}

	SDL_DestroyRenderer(render);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}
