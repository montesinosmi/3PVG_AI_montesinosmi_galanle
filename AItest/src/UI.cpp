#include "UI.h"
#include "Runner.h"
#include "Maze.h"
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
  ImGui::SetNextWindowPos(ImVec2(pos_x, pos_y), ImGuiCond_Once);
  ImGui::SetNextWindowSize(ImVec2(size_x, size_y), ImGuiCond_Always);
}

void RenderImGUI(SDL_Renderer* renderer,
  float& worldTimer, float& runnerTimer,
  float currentWorldTime, float currentRunnerTime,
  int& selectedMario,
  BrushMode& currentBrush) {
  ImGui_ImplSDL2_NewFrame();
  ImGui_ImplSDLRenderer2_NewFrame();
  ImGui::NewFrame();

  float worldSeconds = worldTimer / 1000.0f;
  float runnerSeconds = runnerTimer / 1000.0f;

  // ==========================================
  // PANEL IZQUIERDO SUPERIOR - Control panel
  // ==========================================
  PositionSizeImGUI(0, 0, kUILeftWidth, 240);

  ImGui::Begin("Control Panel");
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

  // ==========================================
  // PANEL IZQUIERDO MEDIO - Map Editor
  // ==========================================
  PositionSizeImGUI(0, 240, kUILeftWidth, kWindowHeight - 400);

  ImGui::Begin("Map Editor");

  ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "SELECT BRUSH:");
  ImGui::Separator();
  ImGui::Spacing();

  // Indicador de brush actual
  if (currentBrush != BrushMode::NONE) {
    const char* brushNames[] = { "Wall", "Room", "Goal", "Lava/Grid", "", "Spawn" };
    int brushIndex = static_cast<int>(currentBrush);
    if (brushIndex >= 0 && brushIndex <= 5) {
      ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f),
        "Active: %s", brushNames[brushIndex]);
    }
  }
  else {
    ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f), "No brush selected");
  }

  ImGui::Spacing();
  ImGui::Separator();
  ImGui::Spacing();

  // Botones de brush con texto al lado
  ImVec4 wallColor(0.3f, 0.3f, 0.3f, 1.0f);
  ImVec4 roomColor(0.8f, 0.8f, 0.8f, 1.0f);
  ImVec4 goalColor(1.0f, 1.0f, 0.0f, 1.0f);
  ImVec4 lavaColor(1.0f, 0.5f, 0.0f, 1.0f);
  ImVec4 spawnColor(0.0f, 1.0f, 0.0f, 1.0f);

  // Room (camino normal)
  if (currentBrush == BrushMode::ROOM) {
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.5f, 0.0f, 1.0f));
  }
  else {
    ImGui::PushStyleColor(ImGuiCol_Button, roomColor);
  }
  if (ImGui::Button("Room", ImVec2(100, 35))) {
    currentBrush = (currentBrush == BrushMode::ROOM) ? BrushMode::NONE : BrushMode::ROOM;
  }
  ImGui::PopStyleColor();
  ImGui::SameLine();
  ImGui::Text("- Normal walkable path");

  // Wall (pared)
  if (currentBrush == BrushMode::WALL) {
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.5f, 0.0f, 1.0f));
  }
  else {
    ImGui::PushStyleColor(ImGuiCol_Button, wallColor);
  }
  if (ImGui::Button("Wall", ImVec2(100, 35))) {
    currentBrush = (currentBrush == BrushMode::WALL) ? BrushMode::NONE : BrushMode::WALL;
  }
  ImGui::PopStyleColor();
  ImGui::SameLine();
  ImGui::Text("- Solid wall (blocks movement)");

  // Spawn
  if (currentBrush == BrushMode::SPAWN) {
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.5f, 0.0f, 1.0f));
  }
  else {
    ImGui::PushStyleColor(ImGuiCol_Button, spawnColor);
  }
  if (ImGui::Button("Spawn", ImVec2(100, 35))) {
    currentBrush = (currentBrush == BrushMode::SPAWN) ? BrushMode::NONE : BrushMode::SPAWN;
  }
  ImGui::PopStyleColor();
  ImGui::SameLine();
  ImGui::Text("- Mario spawn point");

  // Goal
  if (currentBrush == BrushMode::GOAL) {
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.5f, 0.0f, 1.0f));
  }
  else {
    ImGui::PushStyleColor(ImGuiCol_Button, goalColor);
  }
  if (ImGui::Button("Goal", ImVec2(100, 35))) {
    currentBrush = (currentBrush == BrushMode::GOAL) ? BrushMode::NONE : BrushMode::GOAL;
  }
  ImGui::PopStyleColor();
  ImGui::SameLine();
  ImGui::Text("- Default goal flag");

  // Lava/Grid (un solo boton, empieza abierta)
  if (currentBrush == BrushMode::LAVA) {
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.5f, 0.0f, 1.0f));
  }
  else {
    ImGui::PushStyleColor(ImGuiCol_Button, lavaColor);
  }
  if (ImGui::Button("Lava", ImVec2(100, 35))) {
    currentBrush = (currentBrush == BrushMode::LAVA) ? BrushMode::NONE : BrushMode::LAVA;
  }
  ImGui::PopStyleColor();
  ImGui::SameLine();
  ImGui::Text("- Toggles with world timer");

  ImGui::Spacing();
  ImGui::Separator();
  ImGui::Spacing();

  // Boton de reset del mapa
  ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.2f, 0.2f, 1.0f));
  ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.0f, 0.3f, 0.3f, 1.0f));
  if (ImGui::Button("RESET MAP", ImVec2(335, 45))) {
    ResetMap();
    currentBrush = BrushMode::NONE;  // Deselecciona brush al resetear
  }
  ImGui::PopStyleColor(2);

  ImGui::End();

  // ==========================================
  // PANEL IZQUIERDO INFERIOR - Info
  // ==========================================
  PositionSizeImGUI(0, kWindowHeight - 160, kUILeftWidth, 160);

  ImGui::Begin("Info");

  ImGui::Text("Map: %dx%d cells", kMapWidth, kMapHeight);
  ImGui::Text("Window: %dx%d px", kWindowWidth, kWindowHeight);
  ImGui::Text("Cell: %.0fpx (x%.1f)", kTexSize * kScale, kScale);

  ImGui::Spacing();
  ImGui::Separator();
  ImGui::Spacing();

  // Estadisticas de pathfinding en tiempo real
  int pathsActive = 0;
  for (int i = 0; i < kRunnerQuantity; i++) {
    if (runners[i].state == 1 && runners[i].pathLength > 0) {
      pathsActive++;
    }
  }
  ImGui::Text("Active A* paths: %d/%d", pathsActive, GetStateMarios(1));

  ImGui::Spacing();
  ImGui::Separator();
  ImGui::Spacing();

  ImGui::TextWrapped("Programmers: Alvaro G. & Pablo M.");
  ImGui::Spacing();
  ImGui::TextWrapped("Professor: Gustavo Aranda");

  ImGui::End();

  // ==========================================
  // PANEL DERECHO SUPERIOR - Mario Selector
  // ==========================================
  float rightPanelX = kMapOffsetX + kMapPixelWidth;
  PositionSizeImGUI(rightPanelX, 0, kUIRightWidth, 570);

  ImGui::Begin("Mario Selector");

  // Instrucciones
  if (currentBrush != BrushMode::NONE) {
    ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.0f, 1.0f),
      "MAP EDITOR MODE - Click map to paint");
  }
  else if (selectedMario >= 0 && selectedMario < kRunnerQuantity) {
    if (runners[selectedMario].state == 1) {
      ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f),
        "Mario %d selected - Click on map to set goal",
        selectedMario + 1);
    }
    else if (runners[selectedMario].state == 0) {
      ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f),
        "Mario %d is DEAD - Cannot change goal",
        selectedMario + 1);
    }
    else if (runners[selectedMario].state == 2) {
      ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f),
        "Mario %d reached goal - Cannot change goal",
        selectedMario + 1);
    }
  }
  else {
    ImGui::Text("Select a Mario and click on the map to set goal");
  }
  ImGui::Separator();
  ImGui::Spacing();

  for (int i = 0; i < kRunnerQuantity; i++) {
    ImGui::PushID(i);

    // Determina si este Mario puede ser editado (solo si esta VIVO)
    bool canEdit = (runners[i].state == 1);

    // Boton de seleccion con indicador visual
    bool isSelected = (selectedMario == i);
    if (isSelected) {
      ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.5f, 0.0f, 1.0f));
      ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.0f, 0.6f, 0.0f, 1.0f));
      ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.0f, 0.7f, 0.0f, 1.0f));
    }

    if (ImGui::Button(isSelected ? ">>>" : "Select", ImVec2(60, 0))) {
      if (selectedMario == i) {
        selectedMario = -1; // Deseleccionar si ya estaba seleccionado
      }
      else {
        selectedMario = i;  // Seleccionar este Mario
      }
    }

    if (isSelected) {
      ImGui::PopStyleColor(3);
    }

    ImGui::SameLine();

    // EDITOR DE COLOR 
    float color[3] = {
        runners[i].r / 255.0f,
        runners[i].g / 255.0f,
        runners[i].b / 255.0f
    };

    if (ImGui::ColorEdit3("##Color", color, ImGuiColorEditFlags_NoInputs)) {
      runners[i].r = static_cast<int>(color[0] * 255);
      runners[i].g = static_cast<int>(color[1] * 255);
      runners[i].b = static_cast<int>(color[2] * 255);
    }

    ImGui::SameLine();

    // INFORMACIoN DE ESTADO Y TIEMPOS 
    ImGui::BeginGroup();

    // Estado
    const char* stateText = "???";
    if (runners[i].state == 0) stateText = "DEAD";
    else if (runners[i].state == 1) stateText = "ALIVE";
    else if (runners[i].state == 2) stateText = "SAFE";
    ImGui::Text("Mario %d [%s]", i + 1, stateText);

    // Tiempos
    ImGui::Text("Life: %.2fs | Algo: %.2fs", runners[i].lifeTime, runners[i].currentAlgoTime);

    ImGui::EndGroup();

    ImGui::SameLine();

    // SELECTOR DE ALGORITMO (encogido)
    float availWidth = ImGui::GetContentRegionAvail().x;
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (availWidth - 100));

    MovementAlgorithm currentAlgo = GetRunnerAlgorithm(i);
    const char* algoNames[] = { "Random", "A*", "Seek (dumb)", "Seek (smart)" };  // Nombres mas cortos
    int currentAlgoIndex = static_cast<int>(currentAlgo);

    ImGui::PushItemWidth(100);  // Ancho reducido de 150 a 100
    if (ImGui::Combo("##algo", &currentAlgoIndex, algoNames, 4)) {
      SetRunnerAlgorithm(i, static_cast<MovementAlgorithm>(currentAlgoIndex));
    }
    ImGui::PopItemWidth();

    // OBJETIVO PERSONALIZADO
    ImGui::Indent(25.0f);

    ImGui::Text("Goal:");
    ImGui::SameLine();

    // BLOQUEO: Deshabilita inputs si el Mario no esta vivo
    if (!canEdit) {
      ImGui::BeginDisabled();
    }

    // Inputs para cambiar objetivo - ahora muestran las coordenadas actuales
    int tempGoalX = runners[i].goalX;
    int tempGoalY = runners[i].goalY;

    ImGui::PushItemWidth(40);
    if (ImGui::InputInt("##goalX", &tempGoalX, 0, 0)) {
      // Valida y establece cuando cambia
      if (tempGoalX >= 0 && tempGoalX < kMapWidth && canEdit) {
        SetRunnerGoal(i, tempGoalX, runners[i].goalY);
      }
    }
    ImGui::SameLine();
    ImGui::Text(",");
    ImGui::SameLine();
    if (ImGui::InputInt("##goalY", &tempGoalY, 0, 0)) {
      // Valida y establece cuando cambia
      if (tempGoalY >= 0 && tempGoalY < kMapHeight && canEdit) {
        SetRunnerGoal(i, runners[i].goalX, tempGoalY);
      }
    }
    ImGui::PopItemWidth();

    ImGui::SameLine();

    // Boton para resetear a meta por defecto (tambien deshabilitado si no esta vivo)
    if (ImGui::Button("Reset")) {
      if (canEdit) {
        ResetRunnerGoal(i);
      }
    }

    if (!canEdit) {
      ImGui::EndDisabled();
    }

    ImGui::Unindent(25.0f);

    ImGui::Separator();
    ImGui::PopID();
  }

  ImGui::End();

  // ==========================================
  // PANEL DERECHO INFERIOR - Game Control
  // ==========================================
  PositionSizeImGUI(rightPanelX, 570, kUIRightWidth, kWindowHeight - 570);

  ImGui::Begin("Game Control");

  ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "RESTART GAME:");
  ImGui::Separator();
  ImGui::Spacing();

  ImGui::TextWrapped("Reset map and respawn all Marios at spawn points with initial goals.");

  ImGui::Spacing();
  ImGui::Spacing();

  // Boton grande de restart
  ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.6f, 0.2f, 1.0f));
  ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.8f, 0.3f, 1.0f));
  ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.4f, 1.0f, 0.4f, 1.0f));
  if (ImGui::Button("RESTART GAME", ImVec2(335, 80))) {
    RestartGame();
    currentBrush = BrushMode::NONE;  // Deselecciona brush
    selectedMario = -1;  // Deselecciona Mario
  }
  ImGui::PopStyleColor(3);

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