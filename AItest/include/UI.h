#pragma once
#include "../deps/SDL2-2.32.8/include/SDL.h"

// Inicializa ImGUI
void InitImGUI(SDL_Window* window, SDL_Renderer* renderer);

// Posicion y tamaño de ventana
void PositionSizeImGUI(float pos_x, float pos_y, float size_x, float size_y);

// Renderiza el panel de control de ImGUI
void RenderImGUI(SDL_Renderer* renderer, 
                 float& worldTimer, float& runnerTimer,
                 float currentWorldTime, float currentRunnerTime);

// Limpia recursos de ImGUI
void ShutdownImGUI();