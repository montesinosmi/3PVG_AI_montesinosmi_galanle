#pragma once
#include "../deps/SDL2-2.32.8/include/SDL.h"

// Declara las texturas globales (definidas en Texture.cpp)
extern SDL_Texture* roomtex;
extern SDL_Texture* walltex;
extern SDL_Texture* npctex;
extern SDL_Texture* finishtex;
extern SDL_Texture* deadtex;
extern SDL_Texture* transitabletex;
extern SDL_Texture* untransitabletex;
extern SDL_Texture* victorytex;
extern SDL_Texture* spawntex;

// Función para inicializar todas las texturas
void InitTextures(SDL_Renderer* renderer);

// Función auxiliar para dibujar una celda con opciones de color y flip
void DrawCell(SDL_Renderer* renderer, SDL_Texture* texture, 
              int x, int y, int r = 255, int g = 255, int b = 255, 
              bool flipX = false, bool flipY = false);