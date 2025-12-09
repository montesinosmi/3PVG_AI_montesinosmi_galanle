#include <cstdlib>
#include <ctime>
#include "Config.h"
#include "Render.h"
#include "Texture.h" 
#include "Maze.h"
#include "Runner.h"
#include "UI.h"
#include "../deps/SDL2-2.32.8/include/SDL.h"
#include "../deps/imgui/backends/imgui_impl_sdl2.h"
#include "../deps/imgui/imgui.h"

// Variables globales del nucleo de la aplicacion
SDL_Renderer* renderer = nullptr;
SDL_Window* window = nullptr;

// Timers del juego
float WorldTimer = kDefaultWorldTimer;
float RunnerTimer = kDefaultRunnerTimer;
float CurrentWorldTime = 0.0f;
float CurrentRunnerTime = 0.0f;

// Variable global para el Mario seleccionado (-1 = ninguno)
int selectedMario = -1;

// Variable global para el brush del editor de mapa
BrushMode currentBrush = BrushMode::NONE;

// Variable global para el Mario en modo teleport (-1 = ninguno)
int teleportMarioIndex = -1;

int main(int argc, char* argv[]) {
  //srand(time(NULL));
  srand(static_cast<unsigned int>(time(NULL)));

  // Inicializacion
  if (!Renderer_Create("AI", kWindowWidth, kWindowHeight, &window, &renderer)) {
    return -1;
  }

  InitTextures(renderer);
  InitImGUI(window, renderer);
  InitSlabs();
  InitRunners();

  // Loop principal
  bool running = true;
  SDL_Event event;
  float DeltaTime = 0.0f, LastTime = 0.0f;

  while (running) {
    // Manejo de eventos
    while (SDL_PollEvent(&event)) {
      ImGui_ImplSDL2_ProcessEvent(&event);

      if (event.type == SDL_QUIT) {
        running = false;
      }

      // Manejo de clicks del raton en el mapa
      if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
        // Verifica si el click NO fue sobre una ventana de ImGui
        ImGuiIO& io = ImGui::GetIO();
        if (!io.WantCaptureMouse) {
          int mouseX = event.button.x;
          int mouseY = event.button.y;

          // Resta el offset del mapa para obtener coordenadas relativas al mapa
          int mapRelativeX = mouseX - kMapOffsetX;
          int mapRelativeY = mouseY;

          // Convierte coordenadas de pantalla a coordenadas del grid
          /*int gridX = mapRelativeX / (kTexSize * kScale);
          int gridY = mapRelativeY / (kTexSize * kScale);*/
          int gridX = static_cast<int>(mapRelativeX / (kTexSize * kScale)); // asi es mas seguro
          int gridY = static_cast<int>(mapRelativeY / (kTexSize * kScale));

          // Valida que las coordenadas esten dentro del mapa
          if (gridX >= 0 && gridX < kMapWidth && gridY >= 0 && gridY < kMapHeight) {

            // PRIORIDAD 1: Modo Teleport activo
            if (teleportMarioIndex >= 0 && teleportMarioIndex < kRunnerQuantity) {
              TeleportRunner(teleportMarioIndex, gridX, gridY);
              teleportMarioIndex = -1;  // Desactivar modo teleport
            }
            // PRIORIDAD 2: Editor de mapa (si hay brush seleccionado)
            else if (currentBrush != BrushMode::NONE) {
              int type = static_cast<int>(currentBrush);
              bool transitable = true;

              // Lava/Rejilla siempre empieza abierta (transitable = true)
              if (currentBrush == BrushMode::LAVA) {
                type = 3;
                transitable = true;
              }

              SetCellType(gridX, gridY, type, transitable);
            }
            // PRIORIDAD 3: Establecer objetivo de Mario (si hay Mario seleccionado)
            else if (selectedMario >= 0 && selectedMario < kRunnerQuantity) {
              // Solo permite cambiar objetivo si esta VIVO (state == 1)
              if (runners[selectedMario].state == 1) {
                SetRunnerGoal(selectedMario, gridX, gridY);
                selectedMario = -1;
              }
            }
          }
        }
      }
    }

    // Limpia pantalla
    SDL_RenderClear(renderer);

    // Actualiza logica del juego
    UpdateRunners(DeltaTime, CurrentRunnerTime, RunnerTimer);
    UpdateWorld(DeltaTime, CurrentWorldTime, WorldTimer);

    // Renderizado
    DrawMaze(renderer);
    DrawGoalFlags(renderer);  // Dibuja banderas ANTES de los runners para que aparezcan debajo
    DrawRunners(renderer);
    RenderImGUI(renderer, WorldTimer, RunnerTimer, CurrentWorldTime, CurrentRunnerTime,
      selectedMario, currentBrush, teleportMarioIndex);

    SDL_RenderPresent(renderer);
    SDL_Delay(10);

    // Actualiza delta time
    Uint32 currentTime = SDL_GetTicks();
    DeltaTime = currentTime - LastTime;
    //LastTime = currentTime;
    LastTime = static_cast<float>(currentTime);
  }

  // Limpieza
  ShutdownImGUI();
  Renderer_Destroy(window, renderer);
  SDL_Quit();

  return 0;
}