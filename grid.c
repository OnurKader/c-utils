#include "game.h"

// Define an SDL_FRect vector
typedef vec_t(SDL_FRect) vec_frect_t;
typedef vec_t(SDL_Rect) vec_rect_t;

// Function to update the rendered rectangles
void updateQTGrid(vec_rect_t* vec, vec_frect_t* f_vec)
{
	if(!vec || !f_vec)
		return;

	for(uint32_t i = 0U; i < f_vec->length; ++i)
	{
		SDL_Rect temp = (SDL_Rect){
			f_vec->data[i].x, f_vec->data[i].y, f_vec->data[i].w, f_vec->data[i].h};
		vec_push(vec, temp);
	}
}

// Function to get the rectangular boundaries from QT and put them into a SDL_FRect vector
void getFRect(vec_frect_t* const vec, QuadTree* const qt)
{
	if(!qt)
		return;

	vec_push(vec, qt->boundary);

	if(!qt->north_west)
		return;

	getFRect(vec, qt->north_west);
	getFRect(vec, qt->north_east);
	getFRect(vec, qt->south_west);
	getFRect(vec, qt->south_east);
}

void insertPointsIntoQT(QuadTree* const qt, const SDL_Point* point_array)
{
	for(uint32_t i = 0U; i < POINT_COUNT; ++i)
	{
		Point p = makePoint(point_array[i].x, point_array[i].y);
		qt_insert(qt, p);
	}
}

int main(void)
{
	if(SDL_Init(SDL_INIT_VIDEO))
	{
		fprintf(stderr, "Couldn't initialize SDL: %s\n", SDL_GetError());
		exit(1);
	}

	game_t game;
	initGame(&game);
	generatePoints();

	vec_frect_t qt_frect_vec;
	vec_init(&qt_frect_vec);
	vec_reserve(&qt_frect_vec, 64U);

	vec_rect_t qt_rect_vec;
	vec_init(&qt_rect_vec);
	vec_reserve(&qt_rect_vec, 64U);

	bool draw_grid = false;

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
		SDL_SetRenderDrawColor(render, 12U, 12U, 32U, 255U);
		SDL_RenderClear(render);

		SDL_Event e;
		while(SDL_PollEvent(&e))
		{
			switch(e.type)
			{
				case SDL_QUIT: deinitGame(&game); break;
				case SDL_KEYDOWN:
					switch(e.key.keysym.sym)
					{
						case SDLK_ESCAPE:
						case SDLK_q: deinitGame(&game); break;
						default: break;
					}
					break;
				case SDL_KEYUP:
					switch(e.key.keysym.sym)
					{
						case SDLK_r: randomizePoints(); break;
						case SDLK_g: draw_grid = !draw_grid; break;
						case SDLK_i:
						{
							insertPointsIntoQT(game.qt, points);
							getFRect(&qt_frect_vec, game.qt);
							updateQTGrid(&qt_rect_vec, &qt_frect_vec);

							break;
						}
						default: break;
					}
					break;
				case SDL_MOUSEMOTION: mouse = (SDL_Point){e.motion.x, e.motion.y}; break;
				default: break;
			}
		}

		// Handle WASD / Arrow Keys
		const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
		if(currentKeyStates[SDL_SCANCODE_UP])
		{
			printf("UP!\n");
		}
		else if(currentKeyStates[SDL_SCANCODE_DOWN])
		{
			printf("DOWN!\n");
		}
		else if(currentKeyStates[SDL_SCANCODE_LEFT])
		{
			printf("LEFT!\n");
		}
		else if(currentKeyStates[SDL_SCANCODE_RIGHT])
		{
			printf("RIGHT!\n");
		}

		if(draw_grid)
		{
			SDL_Rect rect = (SDL_Rect){1 + GRID_W * (mouse.x / GRID_W),
									   1 + GRID_H * (mouse.y / GRID_H),
									   GRID_W - 1,
									   GRID_H - 1};

			SDL_SetRenderDrawColor(render, 72U, 82U, 64U, 255U);
			SDL_RenderFillRect(render, &rect);
		}

		for(uint16_t i = 0U; i < POINT_COUNT; ++i)
		{
			filledCircleRGBA(render, points[i].x, points[i].y, 3U, 255U, 24U, 24U, 255U);
		}

		if(draw_grid)
		{
			SDL_SetRenderDrawColor(render, 183U, 183U, 64U, 255U);
			SDL_RenderDrawRects(render, game.grid.rect_array, GRID_LENGTH);
		}

		// QT Grid Stuff
		SDL_SetRenderDrawColor(render, 183U, 183U, 64U, 255U);
		SDL_RenderDrawRects(render, qt_rect_vec.data, qt_rect_vec.length);

		SDL_RenderPresent(render);
	}

	vec_deinit(&qt_rect_vec);

	SDL_DestroyRenderer(render);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}
