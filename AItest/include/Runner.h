#pragma once
#include "../deps/SDL2-2.32.8/include/SDL.h"
#include "Config.h"

// Enumeracion de algoritmos de movimiento
enum class MovementAlgorithm {
  RANDOM = 0,
  A_STAR = 1,
  SEEK_DUMB = 2,   // Seek tipo Pac-Man, greedy
  SEEK_SMART = 3    // Seek mejorado (evita reversas)
};

// Estructura de un runner/NPC
struct Runner {
  int x, y;
  int r, g, b;
  int behaviour = 0;
  int state = 1;        // 0: Dead, 1: Alive, 2: Safe
  int direction = 0;
  MovementAlgorithm algorithm = MovementAlgorithm::RANDOM;

  // Ultima posicion para evitar ir hacia atras en el seek listo
  int lastX = 0;
  int lastY = 0;

  float lifeTime;        // Tiempo total de vida
  float currentAlgoTime; // Tiempo con el algoritmo actual

  // Objetivo personalizado
  int goalX = -1;        // -1 significa usar meta por defecto
  int goalY = -1;

  // Para A* pathfinding
  int pathLength = 0;
  int pathIndex = 0;
  int pathX[kMapHeight * kMapWidth];
  int pathY[kMapHeight * kMapWidth];
};

// Array global de runners (definido en Runner.cpp)
extern Runner runners[kRunnerQuantity];

// Inicializa los runners en posiciones de spawn
void InitRunners();

// Dibuja todos los runners en pantalla
void DrawRunners(SDL_Renderer* renderer);

// Dibuja las banderas de objetivos de los runners
void DrawGoalFlags(SDL_Renderer* renderer);

// Actualiza el comportamiento de los runners
void UpdateRunners(float deltaTime, float& currentRunnerTime, float runnerTimer);

// Cuenta cuantos runners estan en un estado especifico
int GetStateMarios(int state);

// Mata runners en una posicion especifica si la celda no es transitable
void KillRunnersAtPosition(int x, int y, bool transitable);

// Cambia el algoritmo de movimiento de un runner especifico
void SetRunnerAlgorithm(int runnerIndex, MovementAlgorithm algorithm);

// Obtiene el algoritmo actual de un runner
MovementAlgorithm GetRunnerAlgorithm(int runnerIndex);

// Establece un objetivo personalizado para un runner
void SetRunnerGoal(int runnerIndex, int goalX, int goalY);

// Resetea el objetivo a la meta por defecto
void ResetRunnerGoal(int runnerIndex);