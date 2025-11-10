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

// Variables globales del nucleo de la aplicacion
SDL_Renderer* renderer = nullptr;
SDL_Window* window = nullptr;

// Timers del juego
float WorldTimer = kDefaultWorldTimer;
float RunnerTimer = kDefaultRunnerTimer;
float CurrentWorldTime = 0.0f;
float CurrentRunnerTime = 0.0f;

int main(int argc, char* argv[]) {
  srand(time(NULL));
  
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
    }
    
    // Limpia pantalla
    SDL_RenderClear(renderer);
    
    // Actualiza logica del juego
    UpdateRunners(DeltaTime, CurrentRunnerTime, RunnerTimer);
    UpdateWorld(DeltaTime, CurrentWorldTime, WorldTimer);
    
    // Renderizado
    DrawMaze(renderer);
    DrawRunners(renderer);
    RenderImGUI(renderer, WorldTimer, RunnerTimer, CurrentWorldTime, CurrentRunnerTime);
      
    SDL_RenderPresent(renderer);
    SDL_Delay(10);

    // Actualiza delta time
    Uint32 currentTime = SDL_GetTicks();
    DeltaTime = currentTime - LastTime;
    LastTime = currentTime;
  }

  // Limpieza
  ShutdownImGUI();
  Renderer_Destroy(window, renderer);
  SDL_Quit();
  
  return 0;
}