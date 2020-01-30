#include "game.h"

#define QUERY 1

// Define an SDL_FRect vector
typedef vec_t(SDL_FRect) vec_frect_t;

// Function to get the rectangular boundaries from QT and put them into a
// SDL_FRect vector
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

void insertPointsIntoQT(QuadTree* const qt, const SDL_FPoint* const point_array)
{
	for(uint32_t i = 0U; i < POINT_COUNT; ++i)
	{
		qt_insert(qt, point_array[i]);
	}
}

Uint32 getFPS(Uint32 start, Uint32 end)
{
	return 1000 / (end - start);
}

int main(void)
{
	printf("\033[3J\033[2J\033[H");

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
	vec_reserve(&qt_frect_vec, 1024U);

	vec_p_t queried_points;
	vec_init(&queried_points);
	vec_reserve(&queried_points, 64U);

	Uint16 fps = 0U;
	Uint64 frame_count = 0ULL;
	Uint32 fps_timer = SDL_GetTicks();

	// Query boundary
	Rect query_rect = (Rect){WIDTH / 2.f, HEIGHT / 2.f, 200, 200};
	Circle query_circle = (Circle){WIDTH / 2.f, HEIGHT / 2.f, 125};

	bool draw_grid = true;

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

	SDL_SetRenderDrawBlendMode(render, SDL_BLENDMODE_BLEND);

	while(game.state != QUITTING)
	{
		SDL_SetRenderDrawColor(render, 12U, 12U, 32U, 255U);
		SDL_RenderClear(render);

		fps_timer = SDL_GetTicks();
		++frame_count;

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
							Uint32 start_time = SDL_GetTicks();
							insertPointsIntoQT(game.qt, points);
							printf("\033[1HPoint insertion to qt took: %u milliseconds\n",
								   SDL_GetTicks() - start_time);
							start_time = SDL_GetTicks();
							getFRect(&qt_frect_vec, game.qt);
							printf("\033[2HGetting boundaries took   : %u milliseconds\n",
								   SDL_GetTicks() - start_time);
							break;
						}
						case SDLK_s:
#if !QUERY
							qt_getPointsInRect(game.qt, &query_rect, &queried_points);
#else
							qt_getPointsInCircle(game.qt, &query_circle, &queried_points);
#endif
							printf("\033[3HCount: %u\n", count);
							break;
						default: break;
					}
					break;
				case SDL_MOUSEMOTION: mouse = (SDL_Point){e.motion.x, e.motion.y}; break;
				default: break;
			}
		}

		// Handle WASD / Arrow Keys
		/* const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL); */
		/* if(currentKeyStates[SDL_SCANCODE_UP]) */
		/* { */
		/* 	printf("UP!\n"); */
		/* } */
		/* else if(currentKeyStates[SDL_SCANCODE_DOWN]) */
		/* { */
		/* 	printf("DOWN!\n"); */
		/* } */
		/* else if(currentKeyStates[SDL_SCANCODE_LEFT]) */
		/* { */
		/* 	printf("LEFT!\n"); */
		/* } */
		/* else if(currentKeyStates[SDL_SCANCODE_RIGHT]) */
		/* { */
		/* 	printf("RIGHT!\n"); */
		/* } */

		/* if(draw_grid) */
		/* { */
		/* 	SDL_Rect rect = (SDL_Rect){1 + GRID_W * (mouse.x / GRID_W), */
		/* 							   1 + GRID_H * (mouse.y / GRID_H), */
		/* 							   GRID_W - 1, */
		/* 							   GRID_H - 1}; */

		/* 	SDL_SetRenderDrawColor(render, 72U, 82U, 64U, 255U); */
		/* 	SDL_RenderFillRect(render, &rect); */
		/* } */

		/* SDL_SetRenderDrawColor(render, 255U, 24U, 24U, 254U); */
		/* SDL_RenderDrawPointsF(render, points, POINT_COUNT); */
		for(uint16_t i = 0U; i < POINT_COUNT; ++i)
		{
			filledCircleRGBA(render, points[i].x, points[i].y, 1U, 255U, 24U, 24U, 222U);
		}

		/* if(draw_grid) */
		/* { */
		/* 	SDL_SetRenderDrawColor(render, 183U, 183U, 64U, 255U); */
		/* 	SDL_RenderDrawRects(render, game.grid.rect_array, GRID_LENGTH); */
		/* } */

		if(draw_grid)
		{
			SDL_SetRenderDrawColor(render, 183U, 183U, 64U, 101U);
			SDL_RenderDrawRectsF(render, qt_frect_vec.data, qt_frect_vec.length);
		}

		// Draw a rectangle at the center as the query boundary
#if !QUERY
		query_rect.x = mouse.x - 100.f;
		query_rect.y = mouse.y - 100.f;
		SDL_SetRenderDrawColor(render, 140, 24, 220, 254);
		SDL_RenderDrawRectF(render, &query_rect);
#else
		query_circle.x = mouse.x + 0.f;
		query_circle.y = mouse.y + 0.f;
		circleRGBA(
			render, query_circle.x, query_circle.y, query_circle.r, 140, 24, 220, 254);
#endif

		// Draw the queried points
		SDL_SetRenderDrawColor(render, 2, 255, 3, 254);
		for(uint16_t i = 0U; i < queried_points.length; ++i)
		{
			filledCircleRGBA(render,
							 queried_points.data[i].x,
							 queried_points.data[i].y,
							 1U,
							 3U,
							 255U,
							 4U,
							 254U);
		}

		SDL_RenderPresent(render);

		// Show FPS
		if(frame_count % 6 == 0)
		{
			fps = 1 + getFPS(fps_timer, SDL_GetTicks());
			printf("\033[5HFPS:%3d\n", fps);
		}
	}

	vec_deinit(&qt_frect_vec);
	vec_deinit(&queried_points);

	SDL_DestroyRenderer(render);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}
