#include "game.h"

int main(void)
{
	game_t game;
	initGame(&game);
	generatePoints();

	bool draw_grid = true;

	if(SDL_Init(SDL_INIT_VIDEO))
	{
		fprintf(stderr, "Couldn't initialize SDL: %s\n", SDL_GetError());
		exit(1);
	}

	SDL_Window* window =
		SDL_CreateWindow("Grid Particle System",
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
		/* SDL_SetRenderDrawColor(render, 0U, 0U, 0U, 255U); */
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
						default: break;
					}
					break;
				case SDL_MOUSEMOTION:
					mouse = (SDL_Point){e.motion.x, e.motion.y};
					break;
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

		/* SDL_SetRenderDrawColor(render, 255U, 24U, 24U, 255U); */
		/* SDL_RenderDrawPoints(render, points, POINT_COUNT); */
		for(uint16_t i = 0U; i < POINT_COUNT; ++i)
		{
			filledCircleRGBA(
				render, points[i].x, points[i].y, 3U, 255U, 24U, 24U, 255U);
		}

		if(draw_grid)
		{
			SDL_SetRenderDrawColor(render, 183U, 183U, 64U, 255U);
			SDL_RenderDrawRects(render, game.grid.rect_array, GRID_LENGTH);
		}

		SDL_RenderPresent(render);
	}

	SDL_DestroyRenderer(render);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}
