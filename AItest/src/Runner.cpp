#include "Runner.h"
#include "Maze.h"
#include "Texture.h"
#include <stdlib.h>
#include <cmath>
#include <algorithm>
bool CalculateAStarPath(Runner& runner);

// Estructura para nodos del A*
struct AStarNode {
  int x, y;
  int gCost; // Costo desde inicio
  int hCost; // Heuristica hasta meta
  int fCost() const { return gCost + hCost; }
  int parentX, parentY;
  
  bool operator<(const AStarNode& other) const {
    return fCost() < other.fCost();
  }
};

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
        
        // Algoritmo A*
        if (runners[i].algorithm == MovementAlgorithm::A_STAR) {
          // Si no tiene camino o lo completo, recalcula
          if (runners[i].pathLength == 0 || runners[i].pathIndex >= runners[i].pathLength) {
            CalculateAStarPath(runners[i]);
          }
          
          // Sigue el camino
          if (runners[i].pathIndex < runners[i].pathLength) {
            int nextX = runners[i].pathX[runners[i].pathIndex];
            int nextY = runners[i].pathY[runners[i].pathIndex];
            
            // Verifica si la celda sigue siendo transitable
            if (slabs[nextY][nextX].transitable && slabs[nextY][nextX].type != 0) {
              runners[i].x = nextX;
              runners[i].y = nextY;
              
              // Actualiza direccion para animacion
              if (nextX < runners[i].pathX[runners[i].pathIndex - 1]) {
                runners[i].direction = 2; // Izquierda
              } else if (nextX > runners[i].pathX[runners[i].pathIndex - 1]) {
                runners[i].direction = 3; // Derecha
              }
              
              runners[i].pathIndex++;
              
              // ¿Llego a la meta?
              if (slabs[nextY][nextX].type == 2) {
                runners[i].state = 2; // Victoria
              }
            } else {
              // El camino ya no es valido, recalcula
              runners[i].pathLength = 0;
            }
          }
        }
        // Algoritmo aleatorio (original)
        else {
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

void SetRunnerAlgorithm(int runnerIndex, MovementAlgorithm algorithm) {
  if (runnerIndex >= 0 && runnerIndex < kRunnerQuantity) {
    runners[runnerIndex].algorithm = algorithm;
    runners[runnerIndex].pathLength = 0;
    runners[runnerIndex].pathIndex = 0;
  }
}

MovementAlgorithm GetRunnerAlgorithm(int runnerIndex) {
  if (runnerIndex >= 0 && runnerIndex < kRunnerQuantity) {
    return runners[runnerIndex].algorithm;
  }
  return MovementAlgorithm::RANDOM;
}

// Calcula distancia Manhattan (heuristica para A*)
int ManhattanDistance(int x1, int y1, int x2, int y2) {
  return abs(x1 - x2) + abs(y1 - y2);
}

// Encuentra la celda de meta mas cercana
bool FindGoalPosition(int& goalX, int& goalY) {
  for (int i = 0; i < kMapHeight; i++) {
    for (int j = 0; j < kMapWidth; j++) {
      if (slabs[i][j].type == 2) {
        goalX = j;
        goalY = i;
        return true;
      }
    }
  }
  return false;
}

// Implementacion del algoritmo A*
bool CalculateAStarPath(Runner& runner) {
  int goalX, goalY;
  if (!FindGoalPosition(goalX, goalY)) {
    return false;
  }
  
  // Listas para A*
  AStarNode openList[kMapHeight * kMapWidth];
  int openCount = 0;
  bool closedList[kMapHeight][kMapWidth] = {false};
  AStarNode nodeData[kMapHeight][kMapWidth];
  
  // Inicializa nodo de inicio
  AStarNode startNode;
  startNode.x = runner.x;
  startNode.y = runner.y;
  startNode.gCost = 0;
  startNode.hCost = ManhattanDistance(runner.x, runner.y, goalX, goalY);
  startNode.parentX = -1;
  startNode.parentY = -1;
  
  openList[openCount++] = startNode;
  nodeData[runner.y][runner.x] = startNode;
  
  // Busqueda A*
  while (openCount > 0) {
    // Encuentra nodo con menor fCost
    int currentIndex = 0;
    for (int i = 1; i < openCount; i++) {
      if (openList[i].fCost() < openList[currentIndex].fCost()) {
        currentIndex = i;
      }
    }
    
    AStarNode current = openList[currentIndex];
    
    // Remueve de lista abierta
    for (int i = currentIndex; i < openCount - 1; i++) {
      openList[i] = openList[i + 1];
    }
    openCount--;
    
    closedList[current.y][current.x] = true;
    
    // ¿Llegamos a la meta?
    if (current.x == goalX && current.y == goalY) {
      // Reconstruye el camino
      runner.pathLength = 0;
      int px = current.x;
      int py = current.y;
      
      while (px != -1 && py != -1) {
        runner.pathX[runner.pathLength] = px;
        runner.pathY[runner.pathLength] = py;
        runner.pathLength++;
        
        int tempX = nodeData[py][px].parentX;
        int tempY = nodeData[py][px].parentY;
        px = tempX;
        py = tempY;
      }
      
      // Invierte el camino (ahora va de inicio a meta)
      for (int i = 0; i < runner.pathLength / 2; i++) {
        std::swap(runner.pathX[i], runner.pathX[runner.pathLength - 1 - i]);
        std::swap(runner.pathY[i], runner.pathY[runner.pathLength - 1 - i]);
      }
      
      runner.pathIndex = 1; // Empieza en 1 (0 es posicion actual)
      return true;
    }
    
    // Explora vecinos
    int dx[] = {0, 0, -1, 1};
    int dy[] = {-1, 1, 0, 0};
    
    for (int i = 0; i < 4; i++) {
      int nx = current.x + dx[i];
      int ny = current.y + dy[i];
      
      // Verifica limites
      if (nx < 0 || nx >= kMapWidth || ny < 0 || ny >= kMapHeight) continue;
      
      // Verifica si es transitable
      if (slabs[ny][nx].type == 0 || !slabs[ny][nx].transitable) continue;
      
      // Verifica si ya esta en lista cerrada
      if (closedList[ny][nx]) continue;
      
      int newGCost = current.gCost + 1;
      
      // Busca si ya esta en lista abierta
      bool inOpenList = false;
      for (int j = 0; j < openCount; j++) {
        if (openList[j].x == nx && openList[j].y == ny) {
          inOpenList = true;
          if (newGCost < openList[j].gCost) {
            openList[j].gCost = newGCost;
            openList[j].parentX = current.x;
            openList[j].parentY = current.y;
            nodeData[ny][nx] = openList[j];
          }
          break;
        }
      }
      
      if (!inOpenList) {
        AStarNode neighbor;
        neighbor.x = nx;
        neighbor.y = ny;
        neighbor.gCost = newGCost;
        neighbor.hCost = ManhattanDistance(nx, ny, goalX, goalY);
        neighbor.parentX = current.x;
        neighbor.parentY = current.y;
        
        openList[openCount++] = neighbor;
        nodeData[ny][nx] = neighbor;
      }
    }
  }
  
  return false; // No se encontro camino
}