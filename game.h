#pragma once

#include "doub_link_list.h"
#include "quadtree.h"
#include "vec.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define WIDTH  (1280U)
#define HEIGHT (720U)

#define GRID_W (40U)
#define GRID_H (40U)

#define GRID_ROW (HEIGHT / GRID_H)
#define GRID_COL (WIDTH / GRID_W)

#define GRID_LENGTH (GRID_COL * GRID_ROW)

typedef vec_t(Point) vec_point_t;

enum state_t
{
	RUNNING,
	QUITTING,
	INPUT,
	OUTPUT
};

struct mouse
{
	int x;
	int y;
} mouse;

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
