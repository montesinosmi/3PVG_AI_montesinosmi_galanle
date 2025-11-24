#include "UI.h"
#include "Runner.h"
#include "../deps/imgui/imgui.h"
#include "../deps/imgui/backends/imgui_impl_sdl2.h"
#include "../deps/imgui/backends/imgui_impl_sdlrenderer2.h"

void InitImGUI(SDL_Window* window, SDL_Renderer* renderer) {
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO(); 
  (void)io;
  ImGui::StyleColorsDark();

  ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
  ImGui_ImplSDLRenderer2_Init(renderer);
}

void PositionSizeImGUI(float pos_x, float pos_y, float size_x, float size_y) {
    ImGui::SetNextWindowPos(ImVec2(pos_x * kScale, pos_y * kScale), ImGuiCond_Once);
    ImGui::SetNextWindowSize(ImVec2(size_x + kScale, size_y * kScale), ImGuiCond_Always);
}

void RenderImGUI(SDL_Renderer* renderer,
                 float& worldTimer, float& runnerTimer,
                 float currentWorldTime, float currentRunnerTime) {
  ImGui_ImplSDL2_NewFrame();
  ImGui_ImplSDLRenderer2_NewFrame();
  ImGui::NewFrame();

  float worldSeconds = worldTimer / 1000.0f;
  float runnerSeconds = runnerTimer / 1000.0f;

  // Panel de control principal
  PositionSizeImGUI(500, 0, 450, 200);

  ImGui::Begin("Control panel");
  ImGui::PushItemWidth(100);
  
  ImGui::Text("=== WORLD SETTINGS ===");
  ImGui::InputFloat("Seconds to update world", &worldSeconds, 0.0f, 10.0f, "%.2f");
  ImGui::Text("Current World Step: %.2f", currentWorldTime / 1000);
  
  ImGui::Spacing();
  ImGui::Separator();
  ImGui::Spacing();
  
  ImGui::Text("=== AI SETTINGS ===");
  ImGui::InputFloat("Seconds to update AI", &runnerSeconds, 0.0f, 10.0f, "%.2f");
  ImGui::Text("Current AI Step: %.2f", currentRunnerTime / 1000);
  
  ImGui::Spacing();
  ImGui::Separator();
  ImGui::Spacing();
  
  ImGui::Text("=== STATISTICS ===");
  ImGui::Text("Current Marios: %d", GetStateMarios(1));
  ImGui::Text("Dead Marios: %d", GetStateMarios(0));
  ImGui::Text("Safe Marios: %d", GetStateMarios(2));
  
  ImGui::End();

  // Panel de seleccion de Marios
  PositionSizeImGUI(500, 200, 450, 300);

  ImGui::Begin("Mario Selector");
  ImGui::Text("Select a Mario to change its algorithm:");
  ImGui::Spacing();
  
  for (int i = 0; i < kRunnerQuantity; i++) {
    ImGui::PushID(i);
    
    // Color del Mario
    ImGui::ColorButton("Color", ImVec4(
      runners[i].r / 255.0f, 
      runners[i].g / 255.0f, 
      runners[i].b / 255.0f, 
      1.0f
    ), 0, ImVec2(20, 20));
    
    ImGui::SameLine();
    
    // Estado del Mario
    const char* stateText = "???";
    if (runners[i].state == 0) stateText = "DEAD";
    else if (runners[i].state == 1) stateText = "ALIVE";
    else if (runners[i].state == 2) stateText = "SAFE";
    
    ImGui::Text("Mario %d [%s]", i + 1, stateText);
    
    ImGui::SameLine();
    ImGui::SetCursorPosX(200);
    
    // Selector de algoritmo
    MovementAlgorithm currentAlgo = GetRunnerAlgorithm(i);
    const char* algoNames[] = { "Random", "A* Pathfinding" };
    int currentAlgoIndex = static_cast<int>(currentAlgo);
    
    ImGui::PushItemWidth(150);
    if (ImGui::Combo("##algo", &currentAlgoIndex, algoNames, 2)) {
      SetRunnerAlgorithm(i, static_cast<MovementAlgorithm>(currentAlgoIndex));
    }
    ImGui::PopItemWidth();
    
    ImGui::PopID();
  }
  
  ImGui::End();

  // Actualiza los timers con los valores editados
  worldTimer = worldSeconds * 1000.0f;
  runnerTimer = runnerSeconds * 1000.0f;

  ImGui::Render();
  ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), renderer);
}

void ShutdownImGUI() {
  ImGui_ImplSDLRenderer2_Shutdown();
  ImGui_ImplSDL2_Shutdown();
  ImGui::DestroyContext();
}