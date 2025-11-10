#pragma once
#include "../deps/SDL2-2.32.8/include/SDL.h"

// Crea ventana y renderer. Devuelve true si ok.
bool Renderer_Create(const char* title, int width, int height,
                     SDL_Window** outWindow, SDL_Renderer** outRenderer);

// Destruye en orden seguro.
void Renderer_Destroy(SDL_Window* window, SDL_Renderer* renderer);
