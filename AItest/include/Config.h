#pragma once

// Constantes de ventana
const float kScale = 1.5f;
const int kTexSize = 10;

// Espacio para UI a ambos lados del mapa
const int kUILeftWidth = 350;   // Espacio izquierdo para UI
const int kUIRightWidth = 350;  // Espacio derecho para UI

// Tamaño del mapa en pixeles
const int kMapPixelWidth = 750;   // 50 * 10 * 1.5 = 750
const int kMapPixelHeight = 750;  // 50 * 10 * 1.5 = 750

// Tamaño total de ventana
const int kWindowWidth = kUILeftWidth + kMapPixelWidth + kUIRightWidth;   // 350 + 750 + 350 = 1450
const int kWindowHeight = kMapPixelHeight;  // 750

// Constantes del mapa
const int kMapHeight = 50;
const int kMapWidth = 50;

// Offset del mapa (empieza despues del UI izquierdo)
const int kMapOffsetX = kUILeftWidth;  // 350

// Constantes de runners
const int kRunnerQuantity = 8;

// Timers del juego (en milisegundos)
const float kDefaultWorldTimer = 3000.0f;
const float kDefaultRunnerTimer = 250.0f;

// Enum para modos de pincel del editor de mapa
enum class BrushMode {
  NONE = -1,
  WALL = 0,
  ROOM = 1,
  GOAL = 2,
  LAVA = 3,     // Rejilla/Lava (empieza abierta, se alterna)
  SPAWN = 5
};

// Enum para modos de interaccion con el mapa
enum class InteractionMode {
  NONE,        // Sin modo activo
  EDIT_MAP,    // Editando el mapa con brush
  MOVE_GOAL,   // Moviendo objetivo de un Mario
  TELEPORT     // Teletransportando un Mario
};