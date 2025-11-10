#include "Texture.h"
#include "../deps/SDL2_image-2.8.8/include/SDL_image.h"
#include <stdio.h>

// Define las texturas globales
SDL_Texture* roomtex = nullptr;
SDL_Texture* walltex = nullptr;
SDL_Texture* npctex = nullptr;
SDL_Texture* finishtex = nullptr;
SDL_Texture* deadtex = nullptr;
SDL_Texture* transitabletex = nullptr;
SDL_Texture* untransitabletex = nullptr;
SDL_Texture* victorytex = nullptr;
SDL_Texture* spawntex = nullptr;

void InitTextures(SDL_Renderer* renderer) {
  SDL_Surface* surface;

  surface = IMG_Load("../assets/Sprites/Wall.png");
  if (!surface) {
    printf("\nError loading room texture");
  }
  roomtex = SDL_CreateTextureFromSurface(renderer, surface);
  SDL_FreeSurface(surface);

  surface = IMG_Load("../assets/Sprites/Marioblack.png");
  if (!surface) {
    printf("\nError loading npc texture");
  }
  npctex = SDL_CreateTextureFromSurface(renderer, surface);
  SDL_FreeSurface(surface);

  surface = IMG_Load("../assets/Sprites/Finish.png");
  if (!surface) {
    printf("\nError loading finish line texture");
  }
  finishtex = SDL_CreateTextureFromSurface(renderer, surface);
  SDL_FreeSurface(surface);

  surface = IMG_Load("../assets/Sprites/Dead.png");
  if (!surface) {
    printf("\nError loading dead texture");
  }
  deadtex = SDL_CreateTextureFromSurface(renderer, surface);
  SDL_FreeSurface(surface);

  surface = IMG_Load("../assets/Sprites/Level.png");
  if (!surface) {
    printf("\nError loading wall texture");
  }
  walltex = SDL_CreateTextureFromSurface(renderer, surface);
  SDL_FreeSurface(surface);

  surface = IMG_Load("../assets/Sprites/Transitable.png");
  if (!surface) {
    printf("\nError loading transitable texture");
  }
  transitabletex = SDL_CreateTextureFromSurface(renderer, surface);
  SDL_FreeSurface(surface);

  surface = IMG_Load("../assets/Sprites/Untransitable.png");
  if (!surface) {
    printf("\nError loading untransitable texture");
  }
  untransitabletex = SDL_CreateTextureFromSurface(renderer, surface);
  SDL_FreeSurface(surface);

  surface = IMG_Load("../assets/Sprites/Victory.png");
  if (!surface) {
    printf("\nError loading victory texture");
  }
  victorytex = SDL_CreateTextureFromSurface(renderer, surface);
  SDL_FreeSurface(surface);

  surface = IMG_Load("../assets/Sprites/Pipe.png");
  if (!surface) {
    printf("\nError loading spawn texture");
  }
  spawntex = SDL_CreateTextureFromSurface(renderer, surface);
  SDL_FreeSurface(surface);
}

void DrawCell(SDL_Renderer* renderer, SDL_Texture* texture, int x, int y, 
              int r, int g, int b, bool flipX, bool flipY) {
  if (!texture) return;

  const float scale = 1.5f;
  const int texsize = 10;

  SDL_SetTextureColorMod(texture, r, g, b);

  SDL_Rect destRect;
  destRect.x = x;
  destRect.y = y;
  destRect.w = static_cast<int>(texsize * scale);
  destRect.h = static_cast<int>(texsize * scale);

  SDL_RendererFlip flip = SDL_FLIP_NONE;
  if (flipX && flipY) {
    flip = (SDL_RendererFlip)(SDL_FLIP_HORIZONTAL | SDL_FLIP_VERTICAL);
  } else if (flipX) {
    flip = SDL_FLIP_HORIZONTAL;
  } else if (flipY) {
    flip = SDL_FLIP_VERTICAL;
  }

  SDL_RenderCopyEx(renderer, texture, nullptr, &destRect, 0.0, nullptr, flip);
  SDL_SetTextureColorMod(texture, 255, 255, 255);
}