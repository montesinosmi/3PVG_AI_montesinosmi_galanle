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

void RenderImGUI(SDL_Renderer* renderer,
                 float& worldTimer, float& runnerTimer,
                 float currentWorldTime, float currentRunnerTime) {
  ImGui_ImplSDL2_NewFrame();
  ImGui_ImplSDLRenderer2_NewFrame();
  ImGui::NewFrame();

  float worldSeconds = worldTimer / 1000.0f;
  float runnerSeconds = runnerTimer / 1000.0f;

  ImGui::Begin("Control panel");
  ImGui::PushItemWidth(50);
  
  ImGui::InputFloat("Seconds to update world", &worldSeconds, 0.0f, 10.0f, "%.2f");
  ImGui::Text("Current World Step: %.2f", currentWorldTime / 1000);
  
  ImGui::InputFloat("Seconds to update AI", &runnerSeconds, 0.0f, 10.0f, "%.2f");
  ImGui::Text("Current AI Step: %.2f", currentRunnerTime / 1000);
  
  ImGui::Separator();
  
  ImGui::Text("Current Marios: %d", GetStateMarios(1));
  ImGui::Text("Dead Marios: %d", GetStateMarios(0));
  ImGui::Text("Safe Marios: %d", GetStateMarios(2));
  
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