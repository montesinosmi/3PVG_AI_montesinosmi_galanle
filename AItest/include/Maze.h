#pragma once
#include "../deps/SDL2-2.32.8/include/SDL.h"
#include "Config.h"

// Estructura de una celda del laberinto
struct slab {
  int x, y;
  int r, g, b;
  int id;
  int type = 0;
  bool transitable = true;
};

// Array global del laberinto (definido en Maze.cpp)
extern slab slabs[kMapHeight][kMapWidth];

// Inicializa el laberinto desde el archivo map_1.txt
void InitSlabs();

// Dibuja todo el laberinto en pantalla
void DrawMaze(SDL_Renderer* renderer);

// Actualiza el estado del mundo (celdas transitables/no transitables)
void UpdateWorld(float deltaTime, float& currentWorldTime, float worldTimer);

// Resetea el mapa al estado original del archivo
void ResetMap();

// Cambia el tipo de una celda del mapa
void SetCellType(int x, int y, int type, bool transitable);

// Fuerza recalculo de todos los paths de los runners vivos
void ForceRecalculateAllPaths();

// Reinicia el juego completo (mapa + runners)
void RestartGame();