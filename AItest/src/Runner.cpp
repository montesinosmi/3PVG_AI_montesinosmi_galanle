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

  // Busca la meta por defecto (tipo 2) para usarla como referencia
  int defaultGoalX = -1;
  int defaultGoalY = -1;
  for (int y = 0; y < kMapHeight; y++) {
    for (int x = 0; x < kMapWidth; x++) {
      if (slabs[y][x].type == 2) {
        defaultGoalX = x;
        defaultGoalY = y;
        break;
      }
    }
    if (defaultGoalX != -1) break;
  }

  // Inicializa cada runner en una posicion de spawn aleatoria
  for (int i = 0; i < kRunnerQuantity; i++) {
    runners[i].behaviour = 1;
    runners[i].direction = 2;
    runners[i].state = 1;

    runners[i].lifeTime = 0.0f;
    runners[i].currentAlgoTime = 0.0f;

    // Inicializa objetivos ALINEADOS a la derecha de la meta por defecto
    if (defaultGoalX != -1 && defaultGoalY != -1) {
      // Coloca los objetivos en linea horizontal a la derecha
      int offsetX = i;  // Desplazamiento horizontal por indice
      runners[i].goalX = defaultGoalX + offsetX;
      runners[i].goalY = defaultGoalY;

      // Asegura que no se salga del mapa
      if (runners[i].goalX >= kMapWidth) {
        runners[i].goalX = kMapWidth - 1;
      }
    }
    else {
      // Fallback: si no hay meta por defecto, usa coordenadas arbitrarias
      runners[i].goalX = 10 + i;
      runners[i].goalY = 10;
    }

    int spawnPos = rand() % spawnCellQuantity;
    runners[i].x = spawnSlabs[spawnPos].x;
    runners[i].y = spawnSlabs[spawnPos].y;

    runners[i].lastX = runners[i].x;   // Asi el Mario no ira hacia atras
    runners[i].lastY = runners[i].y;

    runners[i].r = rand() % 256;
    runners[i].g = rand() % 256;
    runners[i].b = rand() % 256;
  }
}

void DrawRunners(SDL_Renderer* renderer) {
  for (int i = 0; i < kRunnerQuantity; i++) {
    // Aplica offset X para que los runners se dibujen en el mapa desplazado
    /*int posX = kMapOffsetX + (runners[i].x * kTexSize * kScale);
    int posY = runners[i].y * kTexSize * kScale;*/
    int posX = kMapOffsetX + static_cast<int>(runners[i].x * kTexSize * kScale);
    int posY = static_cast<int>(runners[i].y * kTexSize * kScale);

    switch (runners[i].state) {
    case 0:  // Muerto
      DrawCell(renderer, deadtex, posX, posY);
      break;

    case 1:  // Vivo
      switch (runners[i].direction) {
      case 2:  // Izquierda
        DrawCell(renderer, npctex, posX, posY,
          runners[i].r, runners[i].g, runners[i].b, true);
        break;
      case 3:  // Derecha
        DrawCell(renderer, npctex, posX, posY,
          runners[i].r, runners[i].g, runners[i].b, false);
        break;
      }
      break;

    case 2:  // Victoria
      DrawCell(renderer, victorytex, posX, posY,
        runners[i].r, runners[i].g, runners[i].b);
      break;
    }
  }
}

void DrawGoalFlags(SDL_Renderer* renderer) {
  for (int i = 0; i < kRunnerQuantity; i++) {
    // Solo dibuja bandera si el Mario esta VIVO (no muerto, no llego a meta)
    if (runners[i].goalX != -1 && runners[i].goalY != -1 && runners[i].state == 1) {
      // Aplica offset X para que las banderas se dibujen en el mapa desplazado
      /*int posX = kMapOffsetX + (runners[i].goalX * kTexSize * kScale);
      int posY = runners[i].goalY * kTexSize * kScale;*/
      int posX = kMapOffsetX + static_cast<int>(runners[i].goalX * kTexSize * kScale);
      int posY = static_cast<int>(runners[i].goalY * kTexSize * kScale);

      DrawCell(renderer, finishtex, posX, posY,
        runners[i].r, runners[i].g, runners[i].b);
    }
  }
}

// ----------------------------------------
// Helpers comunes para todos los algoritmos
// ------------------------------------------

static bool IsWalkable(int x, int y)
{
  if (x < 0 || x >= kMapWidth || y < 0 || y >= kMapHeight)
    return false;

  // type == 0 son muros, y tambien respetamos transitable
  return (slabs[y][x].type != 0 && slabs[y][x].transitable);
}

// Random objetivo
static void AssignRandomGoalToRunner(Runner& r)
{
  const int maxTries = 10;	// limite de seguridad
  for (int i = 0; i < maxTries; ++i) {
    int x = rand() % kMapWidth;
    int y = rand() % kMapHeight;
    // Evitamos la casilla del Mario y las no transitables
    if ((x == r.x && y == r.y) /* || !IsWalkable(x, y)*/) {
      continue;
    }

    r.goalX = x;
    r.goalY = y;

    // Reseteamos path para A* por si acaso
    r.pathLength = 0;
    r.pathIndex = 0;
    return;
  }
}


// Mueve al runner a (nx, ny) si es walkable, actualiza direccion y goal
static bool MoveRunnerTo(Runner& r, int nx, int ny)
{
  if (!IsWalkable(nx, ny))
    return false;

  r.lastX = r.x;
  r.lastY = r.y;

  // Direccion solo afecta a la animacion (izquierda/derecha)
  if (nx < r.x)      r.direction = 2; // Izquierda
  else if (nx > r.x) r.direction = 3; // Derecha

  r.x = nx;
  r.y = ny;

  // ha llegado a su objetivo?
  if (r.goalX != -1 && r.goalY != -1 &&
    r.x == r.goalX && r.y == r.goalY)
  {
    r.state = 2; // Victoria
  }

  return true;
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

//////////////////////////////////
/////////// ALGORITMOS ///////////
//////////////////////////////////

// Implementacion del algoritmo A*
bool CalculateAStarPath(Runner& runner) {
  int goalX, goalY;

  // Usa objetivo personalizado (siempre esta definido ahora)
  goalX = runner.goalX;
  goalY = runner.goalY;

  // Si por alguna razon no tiene objetivo, busca meta por defecto
  if (goalX == -1 || goalY == -1) {
    if (!FindGoalPosition(goalX, goalY)) {
      return false;
    }
  }

  // Listas para A*
  AStarNode openList[kMapHeight * kMapWidth];
  int openCount = 0;
  bool closedList[kMapHeight][kMapWidth] = { false };
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

    // Llegamos a la meta?????
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
    int dx[] = { 0, 0, -1, 1 };
    int dy[] = { -1, 1, 0, 0 };

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

// Movimiento RANDOM reutilizado como helper
static void UpdateRunnerRandom(Runner& r)
{
  int x = r.x;
  int y = r.y;

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

    r.lastX = r.x;
    r.lastY = r.y;

    r.x = possibleslabs[nextSlab].x;
    r.y = possibleslabs[nextSlab].y;

    if (dir[nextSlab] > 1) {
      r.direction = dir[nextSlab];
    }

    // Verifica si llego a su objetivo
    if (possibleslabs[nextSlab].x == r.goalX &&
      possibleslabs[nextSlab].y == r.goalY) {
      r.state = 2;  // Victoria
    }
  }
}
// SEEK TONTO: siempre intenta reducir la distancia Manhattan al goal
static void UpdateRunnerSeekDumb(Runner& r)
{
  if (r.goalX == -1 || r.goalY == -1) {
    // si no hay objetivo, se comporta como random
    UpdateRunnerRandom(r);
    return;
  }

  int bestX = r.x;
  int bestY = r.y;
  int bestDist = abs(r.x - r.goalX) + abs(r.y - r.goalY);

  // Vecinos: arriba, abajo, izquierda, derecha
  const int dx[4] = { 0,  0, -1, 1 };
  const int dy[4] = { -1, 1,  0, 0 };

  for (int i = 0; i < 4; ++i) {
    int nx = r.x + dx[i];
    int ny = r.y + dy[i];

    if (!IsWalkable(nx, ny))
      continue;

    int dist = abs(nx - r.goalX) + abs(ny - r.goalY);
    if (dist < bestDist) {
      bestDist = dist;
      bestX = nx;
      bestY = ny;
    }
  }

  if (bestX != r.x || bestY != r.y) {
    // Hemos encontrado una casilla que mejora la distancia
    MoveRunnerTo(r, bestX, bestY);
  }
  else {
    // No mejora nada -> fallback a random para no quedarse bloqueado
    UpdateRunnerRandom(r);
  }
}

// Si flee == false -< SeekSmart (Mario se acerca al goal)
// Si flee == true  -> Flee (Mario huye del goal)
static void UpdateRunnerSeekLike(Runner& r, bool flee)
{
  if (r.goalX == -1 || r.goalY == -1) {
    UpdateRunnerRandom(r);
    return;
  }

  int currentDist = abs(r.x - r.goalX) + abs(r.y - r.goalY);

  const int dx[4] = { 0,  0, -1, 1 };
  const int dy[4] = { -1, 1,  0, 0 };

  // Primera pasada: sin reversa
  int bestX = r.x;
  int bestY = r.y;
  int bestDist = currentDist;
  bool foundBetterNoReverse = false;

  for (int i = 0; i < 4; ++i) {
    int nx = r.x + dx[i];
    int ny = r.y + dy[i];

    if (!IsWalkable(nx, ny))
      continue;

    bool isReverse = (nx == r.lastX && ny == r.lastY);

    int dist = abs(nx - r.goalX) + abs(ny - r.goalY);

    if (!isReverse) {
      if (!flee && dist < bestDist) {          // SEEK+: queremos MAS cerca
        bestDist = dist;
        bestX = nx;
        bestY = ny;
        foundBetterNoReverse = true;
      }
      else if (flee && dist > bestDist) {      // FLEE: queremos MAS lejos
        bestDist = dist;
        bestX = nx;
        bestY = ny;
        foundBetterNoReverse = true;
      }
    }
  }

  if (foundBetterNoReverse) {
    MoveRunnerTo(r, bestX, bestY);
    return;
  }

  // Segunda pasada: aceptamos reversa / empates
  bestX = r.x;
  bestY = r.y;
  bestDist = currentDist;
  bool foundAny = false;

  for (int i = 0; i < 4; ++i) {
    int nx = r.x + dx[i];
    int ny = r.y + dy[i];

    if (!IsWalkable(nx, ny))
      continue;

    int dist = abs(nx - r.goalX) + abs(ny - r.goalY);

    bool betterOrEqual = false;
    if (!flee) {
      betterOrEqual = (dist <= bestDist);      // SEEK+: igual o mas cerca
    }
    else {
      betterOrEqual = (dist >= bestDist);      // FLEE: igual o mas lejos
    }

    if (betterOrEqual) {
      bestDist = dist;
      bestX = nx;
      bestY = ny;
      foundAny = true;
    }
  }

  if (foundAny && (bestX != r.x || bestY != r.y)) {
    MoveRunnerTo(r, bestX, bestY);
  }
  else {
    // Si no hay nada razonable, fallback a random
    UpdateRunnerRandom(r);
  }
}



// SEEK LISTO: evita ir hacia la casilla anterior si puede
/*static void UpdateRunnerSeekSmart(Runner& r)
{
  if (r.goalX == -1 || r.goalY == -1) {
    UpdateRunnerRandom(r);
    return;
  }

  int currentDist = abs(r.x - r.goalX) + abs(r.y - r.goalY);

  const int dx[4] = { 0,  0, -1, 1 };
  const int dy[4] = { -1, 1,  0, 0 };

  // Primera pasada: buscamos vecinos que mejoren la distancia
  // y que NO sean la casilla anterior (lastX,lastY)
  int bestX = r.x;
  int bestY = r.y;
  int bestDist = currentDist;
  bool foundBetterNoReverse = false;

  for (int i = 0; i < 4; ++i) {
    int nx = r.x + dx[i];
    int ny = r.y + dy[i];

    if (!IsWalkable(nx, ny))
      continue;

    // Casilla "reverse" respecto al paso anterior
    bool isReverse = (nx == r.lastX && ny == r.lastY);

    int dist = abs(nx - r.goalX) + abs(ny - r.goalY);
    if (!isReverse && dist < bestDist) {
      bestDist = dist;
      bestX = nx;
      bestY = ny;
      foundBetterNoReverse = true;
    }
  }

  if (foundBetterNoReverse) {
    MoveRunnerTo(r, bestX, bestY);
    return;
  }

  // Segunda pasada: si no hemos encontrado nada mejor sin reversa,
  // aceptamos reversa o empates de distancia
  bestX = r.x;
  bestY = r.y;
  bestDist = currentDist;
  bool foundAny = false;

  for (int i = 0; i < 4; ++i) {
    int nx = r.x + dx[i];
    int ny = r.y + dy[i];

    if (!IsWalkable(nx, ny))
      continue;

    int dist = abs(nx - r.goalX) + abs(ny - r.goalY);
    if (dist <= bestDist) {
      bestDist = dist;
      bestX = nx;
      bestY = ny;
      foundAny = true;
    }
  }

  if (foundAny && (bestX != r.x || bestY != r.y)) {
    MoveRunnerTo(r, bestX, bestY);
  }
  else {
    // Si aun asi no hay nada razonable, se comporta como random
    UpdateRunnerRandom(r);
  }
}*/


void UpdateRunnerSeekSmart(Runner& r)
{
  UpdateRunnerSeekLike(r, false); // acercarse al goal
}

void UpdateRunnerFlee(Runner& r)
{
  UpdateRunnerSeekLike(r, true);  // huir del goal
}


void UpdateRunners(float deltaTime, float& currentRunnerTime, float runnerTimer) {
  currentRunnerTime += deltaTime;

  // Actualizar contadores de tiempo fuera del if del timer de movimiento
  for (int i = 0; i < kRunnerQuantity; i++) {
    if (runners[i].state == 1) { // Solo si esta VIVO
      float dtSeconds = deltaTime / 1000.0f;
      runners[i].lifeTime += dtSeconds;
      runners[i].currentAlgoTime += dtSeconds;
    }
  }

  if (currentRunnerTime >= runnerTimer) {
    for (int i = 0; i < kRunnerQuantity; i++) {
      if (runners[i].state == 1 && !runners[i].isPaused) {

        switch (runners[i].algorithm) {

        case MovementAlgorithm::A_STAR:
        {
          // codigo A*
          if (runners[i].pathLength == 0 || runners[i].pathIndex >= runners[i].pathLength) {
            CalculateAStarPath(runners[i]);
          }

          if (runners[i].pathIndex < runners[i].pathLength) {
            int nextX = runners[i].pathX[runners[i].pathIndex];
            int nextY = runners[i].pathY[runners[i].pathIndex];

            if (slabs[nextY][nextX].transitable && slabs[nextY][nextX].type != 0) {

              runners[i].lastX = runners[i].x;
              runners[i].lastY = runners[i].y;

              runners[i].x = nextX;
              runners[i].y = nextY;

              if (runners[i].pathIndex > 0) {
                if (nextX < runners[i].pathX[runners[i].pathIndex - 1]) {
                  runners[i].direction = 2; // Izquierda
                }
                else if (nextX > runners[i].pathX[runners[i].pathIndex - 1]) {
                  runners[i].direction = 3; // Derecha
                }
              }

              runners[i].pathIndex++;

              if (nextX == runners[i].goalX && nextY == runners[i].goalY) {
                runners[i].state = 2; // Victoria
              }
            }
            else {
              runners[i].pathLength = 0;
            }
          }
        }
        break;

        case MovementAlgorithm::RANDOM:
          UpdateRunnerRandom(runners[i]);
          break;

        case MovementAlgorithm::SEEK_DUMB:
          UpdateRunnerSeekDumb(runners[i]);
          break;

        case MovementAlgorithm::SEEK_SMART:
				case MovementAlgorithm::SCATTER:
          UpdateRunnerSeekSmart(runners[i]);
          break;

        case MovementAlgorithm::FLEE:
          UpdateRunnerFlee(runners[i]);
          break;
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
    // Si el algoritmo es diferente al actual, reseteamos el tiempo
    if (runners[runnerIndex].algorithm != algorithm) {
      runners[runnerIndex].currentAlgoTime = 0.0f;
    }
		if (algorithm == MovementAlgorithm::SCATTER) {
			// Asigna un objetivo aleatorio al cambiar a SCATTER
			AssignRandomGoalToRunner(runners[runnerIndex]);
		}
    runners[runnerIndex].algorithm = algorithm;
    runners[runnerIndex].pathLength = 0;  // Reset path por si venia de A* o lo usamos despues
    runners[runnerIndex].pathIndex = 0;
  }
}

MovementAlgorithm GetRunnerAlgorithm(int runnerIndex) {
  if (runnerIndex >= 0 && runnerIndex < kRunnerQuantity) {
    return runners[runnerIndex].algorithm;
  }
  return MovementAlgorithm::RANDOM;
}

void SetRunnerGoal(int runnerIndex, int goalX, int goalY) {
  if (runnerIndex >= 0 && runnerIndex < kRunnerQuantity) {
    runners[runnerIndex].goalX = goalX;
    runners[runnerIndex].goalY = goalY;
    // Forzar recalculo del camino
    runners[runnerIndex].pathLength = 0;
    runners[runnerIndex].pathIndex = 0;
  }
}

void ResetRunnerGoal(int runnerIndex) {
  if (runnerIndex >= 0 && runnerIndex < kRunnerQuantity) {
    // Busca la meta por defecto (tipo 2)
    for (int y = 0; y < kMapHeight; y++) {
      for (int x = 0; x < kMapWidth; x++) {
        if (slabs[y][x].type == 2) {
          runners[runnerIndex].goalX = x;
          runners[runnerIndex].goalY = y;
          break;
        }
      }
      if (runners[runnerIndex].goalX != -1) break;
    }
    // Forzar recalculo del camino
    runners[runnerIndex].pathLength = 0;
    runners[runnerIndex].pathIndex = 0;
  }
}


void SetRunnerPaused(int runnerIndex, bool paused) {
  if (runnerIndex >= 0 && runnerIndex < kRunnerQuantity) {
    runners[runnerIndex].isPaused = paused;
  }
}

bool IsRunnerPaused(int runnerIndex) {
  if (runnerIndex >= 0 && runnerIndex < kRunnerQuantity) {
    return runners[runnerIndex].isPaused;
  }
  return false;
}

void TeleportRunner(int runnerIndex, int x, int y) {
  if (runnerIndex < 0 || runnerIndex >= kRunnerQuantity) return;

  // Validar que la posicion este dentro del mapa
  if (x < 0 || x >= kMapWidth || y < 0 || y >= kMapHeight) return;

  // Teleportar
  runners[runnerIndex].x = x;
  runners[runnerIndex].y = y;

  // Resetear "memoria" para algoritmos Seek
  runners[runnerIndex].lastX = x;
  runners[runnerIndex].lastY = y;

  // Resetear path de A* para forzar recalculo
  runners[runnerIndex].pathLength = 0;
  runners[runnerIndex].pathIndex = 0;
}