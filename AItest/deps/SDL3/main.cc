#include <iostream>
#include <stdio.h>
#include <time.h>

#include "../Libs/SDL3-3.2.24/include/SDL3/SDL.h"
#include "../Libs/SDL3_image-3.2.4/include/SDL3_image/SDL_image.h"
#include "../Libs/imgui/imgui.h"
#include "../Libs/imgui/backends/imgui_impl_sdl3.h"
#include "../Libs/imgui/backends/imgui_impl_sdlrenderer3.h"

const float scale = 1.5f;
const int kWindowHeight = 500 * scale, kWindowWidth = 500 * scale;
const int kMapHeight = 50, kMapWidth = 50;
const int texsize = 10;
const int kRunnerQuantity = 8;

SDL_Texture* roomtex = nullptr;
SDL_Texture* walltex = nullptr;
SDL_Texture* npctex = nullptr;
SDL_Texture* finishtex = nullptr;
SDL_Texture* deadtex = nullptr;
SDL_Texture* transitabletex = nullptr;
SDL_Texture* untransitabletex = nullptr;
SDL_Texture* victorytex = nullptr;
SDL_Texture* spawntex = nullptr;

SDL_Renderer* renderer = nullptr;
SDL_Window* window = nullptr;

FILE* map_file = nullptr;

struct slab{
  int x, y;
  int r, g, b;
  int id;
  int type = 0;
  bool transitable = true;
};
slab slabs[kMapHeight][kMapWidth];

struct runners{
  int x, y;
  int r, g, b;
  int behaviour = 0;
  int state = 1;
  int direction = 0;
};

runners runners[kRunnerQuantity];

int SpawnCellQuantity = 0;
float WorldTimer = 3000.0f, RunnerTimer = 250.0f; 
float CurrentWorldTime = 0.0f, CurrentRunnerTime = 0.0f;

void InitRunners(){
  slab spawnSlabs[kMapHeight * kMapWidth];
  SpawnCellQuantity = 0;
  for (int i = 0; i < kMapHeight; i++){
    for (int j = 0; j < kMapWidth; j++){
      if(slabs[i][j].type == 5){
        spawnSlabs[SpawnCellQuantity] = slabs[i][j];
        SpawnCellQuantity++;
      }
    }
  }
  for(int i = 0; i < kRunnerQuantity; i++){
    runners[i].behaviour = 1;
    runners[i].direction = 2;
    runners[i].state = 1;
    int spawnPos = rand() % SpawnCellQuantity;
    runners[i].x = spawnSlabs[spawnPos].x;
    runners[i].y = spawnSlabs[spawnPos].y;
    runners[i].r = rand() % 256;
    runners[i].g = rand() % 256;
    runners[i].b = rand() % 256;
  }
}

void InitSlabs(){
  map_file = fopen("../../Resources/map_1.txt", "r");
  if(map_file == nullptr){
    printf("\nMap file error");
  }
  int id = 0;
  for (int i = 0; i < kMapHeight; i++) {
    for (int j = 0; j < kMapWidth; ) {
      int c = fgetc(map_file);
      if (c == EOF){
        break;
      }
      if(c == '0' || c == '1'|| c == '2' || c == '3' || c == '4' || c == '5'){
        slabs[i][j].x = j;
        slabs[i][j].y = i;
        slabs[i][j].id = id;
        slabs[i][j].r = 200 + rand() % 20;
        slabs[i][j].g = 200 + rand() % 20;
        slabs[i][j].b = 200 + rand() % 20;
        if (c == '0') {
          slabs[i][j].type = 0;
        }else if(c == '1'){
          slabs[i][j].type = 1;
        }else if(c == '2'){
          slabs[i][j].type = 2;
        }else if(c == '3'){
          slabs[i][j].type = 3;
          slabs[i][j].transitable = true;
        }else if(c == '4'){
          slabs[i][j].type = 3;
          slabs[i][j].transitable = false;
        }else if(c == '5'){
          slabs[i][j].type = 5;
        }
        id++;
        j++;
      }
    }
  }
  if(map_file){
    fclose(map_file);
  }
}

void InitTextures() {

  SDL_Surface* surface;

  surface = IMG_Load("../../Resources/Sprites/Wall.png");
  if (!surface) {
    printf("\nError loading room texture");
  }
  roomtex = SDL_CreateTextureFromSurface(renderer, surface);
  SDL_DestroySurface(surface);

  surface = IMG_Load("../../Resources/Sprites/Marioblack.png");
  if (!surface) {
    printf("\nError loading npc texture");
  }
  npctex = SDL_CreateTextureFromSurface(renderer, surface);
  SDL_DestroySurface(surface);

  surface = IMG_Load("../../Resources/Sprites/Finish.png");
  if (!surface) {
    printf("\nError loading finish line texture");
  }
  finishtex = SDL_CreateTextureFromSurface(renderer, surface);
  SDL_DestroySurface(surface);

  surface = IMG_Load("../../Resources/Sprites/Dead.png");
  if (!surface) {
    printf("\nError loading dead texture");
  }
  deadtex = SDL_CreateTextureFromSurface(renderer, surface);
  SDL_DestroySurface(surface);

  surface = IMG_Load("../../Resources/Sprites/Level.png");
  if (!surface) {
    printf("\nError loading wall texture");
  }
  walltex = SDL_CreateTextureFromSurface(renderer, surface);
  SDL_DestroySurface(surface);

  surface = IMG_Load("../../Resources/Sprites/Transitable.png");
  if (!surface) {
    printf("\nError loading transitable texture");
  }
  transitabletex = SDL_CreateTextureFromSurface(renderer, surface);
  SDL_DestroySurface(surface);

  surface = IMG_Load("../../Resources/Sprites/Untransitable.png");
  if (!surface) {
    printf("\nError loading untransitable texture");
  }
  untransitabletex = SDL_CreateTextureFromSurface(renderer, surface);
  SDL_DestroySurface(surface);

  surface = IMG_Load("../../Resources/Sprites/Victory.png");
  if (!surface) {
    printf("\nError loading victory texture");
  }
  victorytex = SDL_CreateTextureFromSurface(renderer, surface);
  SDL_DestroySurface(surface);

  surface = IMG_Load("../../Resources/Sprites/Pipe.png");
  if (!surface) {
    printf("\nError loading spawn texture");
  }
  spawntex = SDL_CreateTextureFromSurface(renderer, surface);
  SDL_DestroySurface(surface);
}

void InitSDL(){
  //if (SDL_Init(SDL_INIT_VIDEO) == false) {
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    printf("\nError initializing SDL: %s\n", SDL_GetError());
    return;
  }
  
  window = SDL_CreateWindow(
    "AI",
    kWindowWidth,
    kWindowHeight,
    0
  );
  
  renderer = SDL_CreateRenderer(window, "renderer");
  if (!renderer) {
    printf("\nError creating renderer: %s", SDL_GetError());
    return;
  }
  InitTextures();
}

void DrawCell(SDL_Renderer* renderer, SDL_Texture* texture, int x, int y, int r = 255, int g = 255, int b = 255, bool flipX = false, bool flipY = false) {
    if (!texture) return;

    SDL_SetTextureColorMod(texture, r, g, b);

    SDL_Rect destRect;
    destRect.x = x;
    destRect.y = y;
    destRect.w = static_cast<int>(texsize * scale);
    destRect.h = static_cast<int>(texsize * scale);

    SDL_FlipMode flip = SDL_FLIP_NONE;
    if (flipX && flipY) {
        flip = (SDL_FlipMode)(SDL_FLIP_HORIZONTAL | SDL_FLIP_VERTICAL);
    } else if (flipX) {
        flip = SDL_FLIP_HORIZONTAL;
    } else if (flipY) {
        flip = SDL_FLIP_VERTICAL;
    }
    bool SDL_RenderTextureRotated(
      SDL_Renderer* renderer,
      SDL_Texture* texture,
      const SDL_FRect* src,
      const SDL_FRect* dst,
      double angle,
      const SDL_FPoint* center,
      SDL_FlipMode flip
    );
    SDL_SetTextureColorMod(texture, 255, 255, 255);
}


int GetStateMarios(int state){
  int count = 0;
  for(int i = 0; i < kRunnerQuantity; i++){
    if(runners[i].state == state){
      count++;
    }
  }
  return count;
}

void DrawMaze(){
  for (int i = 0; i < kMapHeight; i++)
  {
    for (int j = 0; j < kMapWidth; j++){
      switch(slabs[i][j].type){
        case 0:
        DrawCell(renderer, walltex, j * texsize * scale, i * texsize * scale, slabs[i][j].r, slabs[i][j].g, slabs[i][j].b);
        break;
        case 1:
        DrawCell(renderer, roomtex, j * texsize * scale, i * texsize * scale, slabs[i][j].r, slabs[i][j].g, slabs[i][j].b);
        break;
        case 2:
        DrawCell(renderer, finishtex, j * texsize * scale, i * texsize * scale);
        break;
        case 3:
        DrawCell(renderer, untransitabletex, j * texsize * scale, i * texsize * scale, 215 + rand() % 40, 215 + rand() % 40, 215 + rand() % 40);
        if(slabs[i][j].transitable){
          DrawCell(renderer, transitabletex, j * texsize * scale, i * texsize * scale);
        }
        break;
        case 5:
        DrawCell(renderer, roomtex, j * texsize * scale, i * texsize * scale, slabs[i][j].r, slabs[i][j].g, slabs[i][j].b);
        DrawCell(renderer, spawntex, j * texsize * scale, i * texsize * scale);
        break;
      }
    }
  }
}

void DrawRunners(){
  for(int i = 0; i < kRunnerQuantity; i++){
    switch(runners[i].state){
      case 0:
      DrawCell(renderer, deadtex, runners[i].x * texsize * scale, runners[i].y * texsize * scale);
      break;

      case 1:
        switch(runners[i].direction){
          case 2:
          DrawCell(renderer, npctex, runners[i].x * texsize * scale, runners[i].y * texsize * scale, runners[i].r, runners[i].g, runners[i].b, true);
          break;
          case 3:
          DrawCell(renderer, npctex, runners[i].x * texsize * scale, runners[i].y * texsize * scale, runners[i].r, runners[i].g, runners[i].b, false);
          break;
        }
      
      break;

      case 2:
      DrawCell(renderer, victorytex, runners[i].x * texsize * scale, runners[i].y * texsize * scale, runners[i].r, runners[i].g, runners[i].b);
      break;
    }
  }
}
void UpdateRunners(){
  if(CurrentRunnerTime >= RunnerTimer){
    for(int i = 0; i < kRunnerQuantity; i++){
      if(runners[i].state == 1){
        switch(runners[i].behaviour){
        case 0:
        break;
        case 1:

          int x = runners[i].x;
          int y = runners[i].y;
          slab top = slabs[y - 1][x];
          slab bottom = slabs[y + 1][x];
          slab left = slabs[y][x - 1];
          slab right = slabs[y][x + 1];
          int choices = 0;
          slab possibleslabs[4];
          int dir[4];
          if(top.type != 0 && top.transitable){
            possibleslabs[choices] = top;
            dir[choices] = 0;
            choices++;
          }
          if(bottom.type != 0 && bottom.transitable){
            possibleslabs[choices] = bottom;
            dir[choices] = 1;
            choices++;
          }
          if(left.type != 0 && left.transitable){
            possibleslabs[choices] = left;
            dir[choices] = 2;
            choices++;
          }
          if(right.type != 0 && right.transitable){
            possibleslabs[choices] = right;
            dir[choices] = 3;
            choices++;
          }
          int nextSlab = 0;
          if(choices != 0){
            nextSlab = rand() % choices;
            runners[i].x = possibleslabs[nextSlab].x;
            runners[i].y = possibleslabs[nextSlab].y;
            if(dir[nextSlab] > 1){
              runners[i].direction = dir[nextSlab];
            }
            if(possibleslabs[nextSlab].type == 2){
              runners[i].state = 2;
            }
          }

          break;
        }

      }
    }
    CurrentRunnerTime = 0.0f;
  }
}

void UpdateWorld(){
  if(CurrentWorldTime >= WorldTimer){
    for (int i = 0; i < kMapHeight; i++){
      for (int j = 0; j < kMapWidth; j++){
        if(slabs[i][j].type == 3){
          slabs[i][j].transitable = !slabs[i][j].transitable;
          for(int k = 0; k < kRunnerQuantity; k++){
            if(runners[k].x == slabs[i][j].x && runners[k].y == slabs[i][j].y && !slabs[i][j].transitable){
              runners[k].state = 0;
            }
          }
        }
      }
    }
    CurrentWorldTime = 0.0f;
  }
}

void InitImGUI() {
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO(); (void)io;
  ImGui::StyleColorsDark();

  ImGui_ImplSDL3_InitForSDLRenderer(window, renderer);
  ImGui_ImplSDLRenderer3_Init(renderer);
}

void ImGUI() {
  ImGui_ImplSDL3_NewFrame();
  ImGui_ImplSDLRenderer3_NewFrame();
  ImGui::NewFrame();

  float worldSeconds = WorldTimer / 1000.0f;
  float runnerSeconds = RunnerTimer / 1000.0f;

  ImGui::Begin("Control panel");
  ImGui::PushItemWidth(50);
  ImGui::InputFloat("Seconds to update world", &worldSeconds, 0.0f, 10.0f, "%.2f");
  ImGui::Text("Current World Step: %.2f", CurrentWorldTime / 1000);
  ImGui::InputFloat("Seconds to update AI", &runnerSeconds, 0.0f, 10.0f, "%.2f");
  ImGui::Text("Current AI Step: %.2f", CurrentRunnerTime / 1000);
  ImGui::Text("Current Marios: %d", GetStateMarios(1));
  ImGui::Text("Dead Marios: %d", GetStateMarios(0));
  ImGui::Text("Safe Marios: %d", GetStateMarios(2));
  ImGui::End();

  WorldTimer = worldSeconds * 1000.0f;
  RunnerTimer = runnerSeconds * 1000.0f; 

  ImGui::Render();
  ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), renderer);
}

int main(int argc, char* argv[]) {
  srand (time(NULL));
  InitSDL();
  InitImGUI();
  InitSlabs();
  InitRunners();


  bool running = true;
  SDL_Event event;

  float DeltaTime = 0.0f, LastTime = 0.0f;

  while (running) {
    while (SDL_PollEvent(&event)) {
      ImGui_ImplSDL3_ProcessEvent(&event);
      if (event.type == SDL_EVENT_QUIT){
        running = false;
      }
    }
    
    SDL_RenderClear(renderer);
    
    
    UpdateRunners();
    UpdateWorld();
    
    DrawMaze();
    DrawRunners();
    ImGUI();
      
    SDL_RenderPresent(renderer);

    SDL_Delay(10);

    Uint32 currentTime = SDL_GetTicks();
    DeltaTime = currentTime - LastTime;
    LastTime = currentTime;

    CurrentRunnerTime += DeltaTime;
    CurrentWorldTime += DeltaTime;
  }

  ImGui_ImplSDLRenderer3_Shutdown();
  ImGui_ImplSDL3_Shutdown();
  ImGui::DestroyContext();

  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}



