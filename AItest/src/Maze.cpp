#include "Maze.h"
#include "Texture.h"
#include "Runner.h"
#include <stdio.h>
#include <stdlib.h>

// Define el array global del laberinto
slab slabs[kMapHeight][kMapWidth];

void InitSlabs() {
  //FILE* map_file = fopen("../assets/Maps/map_1.txt", "r");
  FILE* map_file = nullptr;
  fopen_s(&map_file, "../assets/Maps/map_1.txt", "r");

  if (map_file == nullptr) {
    printf("\nMap file error");
    return;
  }
  
  int id = 0;
  for (int i = 0; i < kMapHeight; i++) {
    for (int j = 0; j < kMapWidth; ) {
      int c = fgetc(map_file);
      if (c == EOF) {
        break;
      }
      if (c == '0' || c == '1'|| c == '2' || c == '3' || c == '4' || c == '5') {
        slabs[i][j].x = j;
        slabs[i][j].y = i;
        slabs[i][j].id = id;
        slabs[i][j].r = 200 + rand() % 20;
        slabs[i][j].g = 200 + rand() % 20;
        slabs[i][j].b = 200 + rand() % 20;
        
        if (c == '0') {
          slabs[i][j].type = 0;
        } else if (c == '1') {
          slabs[i][j].type = 1;
        } else if (c == '2') {
          slabs[i][j].type = 2;
        } else if (c == '3') {
          slabs[i][j].type = 3;
          slabs[i][j].transitable = true;
        } else if (c == '4') {
          slabs[i][j].type = 3;
          slabs[i][j].transitable = false;
        } else if (c == '5') {
          slabs[i][j].type = 5;
        }
        id++;
        j++;
      }
    }
  }
  
  if (map_file) {
    fclose(map_file);
  }
}

void DrawMaze(SDL_Renderer* renderer) {
  for (int i = 0; i < kMapHeight; i++) {
    for (int j = 0; j < kMapWidth; j++) {
      switch(slabs[i][j].type) {
        case 0:
          DrawCell(renderer, walltex, j * kTexSize * kScale, i * kTexSize * kScale, 
                   slabs[i][j].r, slabs[i][j].g, slabs[i][j].b);
          break;
        case 1:
          DrawCell(renderer, roomtex, j * kTexSize * kScale, i * kTexSize * kScale, 
                   slabs[i][j].r, slabs[i][j].g, slabs[i][j].b);
          break;
        case 2:
          DrawCell(renderer, finishtex, j * kTexSize * kScale, i * kTexSize * kScale);
          break;
        case 3:
          DrawCell(renderer, untransitabletex, j * kTexSize * kScale, i * kTexSize * kScale, 
                   215 + rand() % 40, 215 + rand() % 40, 215 + rand() % 40);
          if (slabs[i][j].transitable) {
            DrawCell(renderer, transitabletex, j * kTexSize * kScale, i * kTexSize * kScale);
          }
          break;
        case 5:
          DrawCell(renderer, roomtex, j * kTexSize * kScale, i * kTexSize * kScale, 
                   slabs[i][j].r, slabs[i][j].g, slabs[i][j].b);
          DrawCell(renderer, spawntex, j * kTexSize * kScale, i * kTexSize * kScale);
          break;
      }
    }
  }
}

void UpdateWorld(float deltaTime, float& currentWorldTime, float worldTimer) {
  currentWorldTime += deltaTime;
  
  if (currentWorldTime >= worldTimer) {
    for (int i = 0; i < kMapHeight; i++) {
      for (int j = 0; j < kMapWidth; j++) {
        if (slabs[i][j].type == 3) {
          slabs[i][j].transitable = !slabs[i][j].transitable;
          
          // Mata a los runners que esten en celdas que se vuelven intransitables
          KillRunnersAtPosition(slabs[i][j].x, slabs[i][j].y, slabs[i][j].transitable);
        }
      }
    }
    currentWorldTime = 0.0f;
  }
}