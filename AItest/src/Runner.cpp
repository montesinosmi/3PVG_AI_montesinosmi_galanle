#include "Runner.h"
#include "Maze.h"
#include "Texture.h"
#include <stdlib.h>

// Define el array global de runners
Runner runners[kRunnerQuantity];

void InitRunners() {
  // Recolecta todas las celdas de spawn
  slab spawnSlabs[kMapHeight * kMapWidth];
  int spawnCellQuantity = 0;
  
  for (int i = 0; i < kMapHeight; i++) {
    for (int j = 0; j < kMapWidth; j++) {
      if (slabs[i][j].type == 5) {
        spawnSlabs[spawnCellQuantity] = slabs[i][j];
        spawnCellQuantity++;
      }
    }
  }
  
  // Inicializa cada runner en una posicion de spawn aleatoria
  for (int i = 0; i < kRunnerQuantity; i++) {
    runners[i].behaviour = 1;
    runners[i].direction = 2;
    runners[i].state = 1;
    
    int spawnPos = rand() % spawnCellQuantity;
    runners[i].x = spawnSlabs[spawnPos].x;
    runners[i].y = spawnSlabs[spawnPos].y;
    
    runners[i].r = rand() % 256;
    runners[i].g = rand() % 256;
    runners[i].b = rand() % 256;
  }
}

void DrawRunners(SDL_Renderer* renderer) {
  for (int i = 0; i < kRunnerQuantity; i++) {
    switch(runners[i].state) {
      case 0:  // Muerto
        DrawCell(renderer, deadtex, runners[i].x * kTexSize * kScale, 
                 runners[i].y * kTexSize * kScale);
        break;

      case 1:  // Vivo
        switch(runners[i].direction) {
          case 2:  // Izquierda
            DrawCell(renderer, npctex, runners[i].x * kTexSize * kScale, 
                     runners[i].y * kTexSize * kScale, 
                     runners[i].r, runners[i].g, runners[i].b, true);
            break;
          case 3:  // Derecha
            DrawCell(renderer, npctex, runners[i].x * kTexSize * kScale, 
                     runners[i].y * kTexSize * kScale, 
                     runners[i].r, runners[i].g, runners[i].b, false);
            break;
        }
        break;

      case 2:  // Victoria
        DrawCell(renderer, victorytex, runners[i].x * kTexSize * kScale, 
                 runners[i].y * kTexSize * kScale, 
                 runners[i].r, runners[i].g, runners[i].b);
        break;
    }
  }
}

void UpdateRunners(float deltaTime, float& currentRunnerTime, float runnerTimer) {
  currentRunnerTime += deltaTime;
  
  if (currentRunnerTime >= runnerTimer) {
    for (int i = 0; i < kRunnerQuantity; i++) {
      if (runners[i].state == 1) {
        switch(runners[i].behaviour) {
          case 0:
            break;
          
          case 1: {
            int x = runners[i].x;
            int y = runners[i].y;
            
            slab top = slabs[y - 1][x];
            slab bottom = slabs[y + 1][x];
            slab left = slabs[y][x - 1];
            slab right = slabs[y][x + 1];
            
            int choices = 0;
            slab possibleslabs[4];
            int dir[4];
            
            if (top.type != 0 && top.transitable) {
              possibleslabs[choices] = top;
              dir[choices] = 0;
              choices++;
            }
            if (bottom.type != 0 && bottom.transitable) {
              possibleslabs[choices] = bottom;
              dir[choices] = 1;
              choices++;
            }
            if (left.type != 0 && left.transitable) {
              possibleslabs[choices] = left;
              dir[choices] = 2;
              choices++;
            }
            if (right.type != 0 && right.transitable) {
              possibleslabs[choices] = right;
              dir[choices] = 3;
              choices++;
            }
            
            if (choices != 0) {
              int nextSlab = rand() % choices;
              runners[i].x = possibleslabs[nextSlab].x;
              runners[i].y = possibleslabs[nextSlab].y;
              
              if (dir[nextSlab] > 1) {
                runners[i].direction = dir[nextSlab];
              }
              
              if (possibleslabs[nextSlab].type == 2) {
                runners[i].state = 2;  // Victoria
              }
            }
            break;
          }
        }
      }
    }
    currentRunnerTime = 0.0f;
  }
}

int GetStateMarios(int state) {
  int count = 0;
  for (int i = 0; i < kRunnerQuantity; i++) {
    if (runners[i].state == state) {
      count++;
    }
  }
  return count;
}

void KillRunnersAtPosition(int x, int y, bool transitable) {
  if (!transitable) {
    for (int k = 0; k < kRunnerQuantity; k++) {
      if (runners[k].x == x && runners[k].y == y) {
        runners[k].state = 0;  // Mata al runner
      }
    }
  }
}