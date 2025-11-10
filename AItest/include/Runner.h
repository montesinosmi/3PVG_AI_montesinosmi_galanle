#pragma once
#include "../deps/SDL2-2.32.8/include/SDL.h"
#include "Config.h"

// Estructura de un runner/NPC
struct Runner {
  int x, y;
  int r, g, b;
  int behaviour = 0;
  int state = 1;
  int direction = 0;
};

// Array global de runners (definido en Runner.cpp)
extern Runner runners[kRunnerQuantity];

// Inicializa los runners en posiciones de spawn
void InitRunners();

// Dibuja todos los runners en pantalla
void DrawRunners(SDL_Renderer* renderer);

// Actualiza el comportamiento de los runners
void UpdateRunners(float deltaTime, float& currentRunnerTime, float runnerTimer);

// Cuenta cuántos runners están en un estado específico
int GetStateMarios(int state);

// Mata runners en una posición específica si la celda no es transitable
void KillRunnersAtPosition(int x, int y, bool transitable);