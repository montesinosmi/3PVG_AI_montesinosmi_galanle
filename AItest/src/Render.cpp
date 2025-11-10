#include "../deps/SDL2-2.32.8/include/SDL.h"
#include "../include/Render.h"
#include <cstdio>
#include <stdio.h>


bool Renderer_Create(const char* title, int width, int height,
                     SDL_Window** outWindow, SDL_Renderer** outRenderer)
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		printf("\nError initializing SDL");
		return false;
	}

	SDL_Window* win = SDL_CreateWindow(
		title,
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		width, height,
		SDL_WINDOW_SHOWN
	);
	if (!win) {
		printf("\nError creating SDL_Window");
		SDL_Quit();
		return false;
	}

	SDL_Renderer* ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
	if (!ren) {
		printf("\nError creating SDL_Renderer");
		SDL_DestroyWindow(win);
		SDL_Quit();
		return false;
	}

	*outWindow = win;
	*outRenderer = ren;
	return true;
}

void Renderer_Destroy(SDL_Window* window, SDL_Renderer* renderer)
{
	if (renderer) SDL_DestroyRenderer(renderer);
	if (window)   SDL_DestroyWindow(window);
	SDL_Quit();
}
