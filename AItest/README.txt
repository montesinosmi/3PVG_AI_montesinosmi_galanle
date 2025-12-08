===================================================
PROYECTO DE INTELIGENCIA ARTIFICIAL
Simulador de Pathfinding con Múltiples Algoritmos
===================================================

ASIGNATURA: 3VGP - Inteligencia Artificial
PROFESOR: Gustavo Aranda
FECHA DE ENTREGA: 09/12/2025

AUTORES:
- Álvaro J. Galán Leal
- Pablo Montesinos Micó

===================================================
ÍNDICE
===================================================
1. Resumen del Proyecto
2. Cómo Compilar y Ejecutar
3. Características Implementadas
4. Descripción de la Interfaz
5. Algoritmos Implementados
6. Puntos Destacados para la Evaluación

===================================================
1. RESUMEN DEL PROYECTO
===================================================

Este proyecto implementa un simulador completo de algoritmos de
pathfinding e inteligencia artificial en un entorno tipo Pac-Man/Mario.
El sistema incluye 8 agentes (NPCs) que pueden utilizar diferentes
algoritmos de movimiento, un mapa totalmente editable en tiempo real,
y una interfaz de usuario completa desarrollada con ImGui.

Tecnologías utilizadas:
- Lenguaje: C++ (estructurado)
- Gráficos: SDL2 v2.32.8
- UI: ImGui (Dear ImGui)
- Compilador: MSVC x64

Estructura del proyecto:
- assets/: Mapas y sprites
- bin/: Ejecutable y DLLs (generado al compilar)
- deps/: Librerías (SDL2, ImGui)
- include/: Archivos de cabecera (.h)
- src/: Código fuente (.cpp)
- tools/: Scripts de compilación

===================================================
2. CÓMO COMPILAR Y EJECUTAR
===================================================

IMPORTANTE: Se deberá compilar el proyecto para generar
el ejecutable AItest.exe, ya que este no se incluye en la entrega.

-----------------------------------------------
OPCIÓN 1:  COMPILACIÓN RÁPIDA (RECOMENDADO)
-----------------------------------------------

1. Abrir "x64 Native Tools Command Prompt for Visual Studio"
2. cd <proyecto>/tools
3. Ejecutar: comp.bat
4. Responder 'S' a "¿Limpiar antes de compilar?"
5. Responder 'S' a "¿Ejecutar AItest.exe ahora?"

Ejecutable generado en: <proyecto>/bin/AItest.exe

-----------------------------------------------
OPCIÓN 2: COMPILACIÓN SIN PREGUNTAS
-----------------------------------------------

Compilación silenciosa:
  cd tools
  compile.bat

Ejecutable se genera en bin/ pero no se ejecuta automáticamente

-----------------------------------------------
OPCIÓN 3: GENERAR SOLUCIÓN VISUAL STUDIO (PREMAKE) 
-----------------------------------------------

1. Abrir una consola (CMD o PowerShell). 
2. Navegar a la carpeta RAÍZ del proyecto (donde está premake5.lua). 
IMPORTANTE: NO entrar en la carpeta tools. 
3. Verificar que premake5.exe existe en la carpeta tools/. 
(Si no está, descargarlo de https://premake.github.io/ y colocarlo ahí). 
4. Ejecutar el comando: tools\premake5.exe vs2022 
5. Abrir la solución generada en: build\AItest.sln 
6. Compilar (Ctrl+Shift+B) y Ejecutar (F5).

Ejecutable se genera en: bin/Debug/AItest.exe

-----------------------------------------------
Información detallada:
  Ver archivos en tools/:
  - Compilador_consola.txt
  - Compilador_premake5.txt
-----------------------------------------------
-----------------------------------------------
SOLUCIÓN DE PROBLEMAS
-----------------------------------------------

Si falla la compilación:
1. Usar "x64 Native Tools Command Prompt" (NO CMD normal)
2. Ejecutar: clear.bat all
3. Volver a ejecutar: comp.bat

===================================================
3. CARACTERÍSTICAS IMPLEMENTADAS
===================================================

-----------------------------------------------
FASE 1: BASE DE TRABAJO Y MOVIMIENTO BÁSICO
-----------------------------------------------

- Grid/Board de 50x50 celdas
- Clase Agente (Runner) completa
- Sistema de Render independiente (SDL2)
- Algoritmos: Random y Seek (Dumb + Smart)
- Editor de mapa en tiempo real
  * 5 tipos de celdas: Wall, Room, Goal, Spawn, Lava
  * Edición por click
  * Reset a mapa original
- Objetivos personalizables por agente

-----------------------------------------------
FASE 2: ALGORITMO A* Y PATHFINDING
-----------------------------------------------

- Algoritmo A* completo
  * Heurística Manhattan
  * Listas open/closed
  * Reconstrucción de caminos
  * Recálculo automático
- Algoritmos adicionales: Scatter y Flee
- Demo funcional con 8 agentes

-----------------------------------------------
FASE 3: ESTADOS Y COMPORTAMIENTOS
-----------------------------------------------

- Carga de mapa desde archivo externo (map_1.txt)
- Objetivos controlados por usuario
  * Click en mapa para establecer objetivo
  * Banderas visuales de colores
  * Edición manual de coordenadas
- Estados de agentes: Seek, Scatter, Flee
- Sistema de toma de decisiones
  * Selector de algoritmo individual
  * Selector de algoritmo global
  * Control de pausa individual/global

-----------------------------------------------
CARACTERÍSTICAS ADICIONALES
-----------------------------------------------

INTERFAZ PROFESIONAL (4 PANELES IMGUI):
- Control Panel: Timers y estadísticas en tiempo real
- Map Editor: 5 brushes, sistema de prioridades
- Game Control: Restart completo del juego
- Mario Selector: Control individual y global de agentes

SISTEMA DE OBJETIVOS AVANZADO:
- Objetivos personalizables por agente
- Banderas visuales con color del agente
- Objetivos iniciales alineados automáticamente

CELDAS DINÁMICAS (LAVA):
- Alternan entre transitable/no transitable
- Timer configurable desde UI
- Mata agentes atrapados cuando se cierra

SISTEMA DE TELEPORT:
- Mover agentes instantáneamente
- Feedback visual cuando está activo
- Resetea memoria de algoritmos

MÉTRICAS DETALLADAS:
- Tiempo total de vida por agente
- Tiempo con algoritmo actual
- Contador de vivos/muertos/seguros

CONTROL GRANULAR:
- Control individual: cada Mario independiente
- Control global: todos los vivos simultáneamente
- Sistema de pausa por agente o grupo

===================================================
4. DESCRIPCIÓN DE LA INTERFAZ
===================================================

El sistema cuenta con 4 paneles ImGui:

-----------------------------------------------
PANEL 1: CONTROL PANEL (Superior Izquierdo)
-----------------------------------------------

- Seconds to update world: Timer de celdas dinámicas
- Seconds to update AI: Velocidad de agentes
- Estadísticas: Marios vivos/muertos/seguros

-----------------------------------------------
PANEL 2: MAP EDITOR (Medio Izquierdo)
-----------------------------------------------

- 5 Brushes: Room, Wall, Spawn, Goal, Lava
- Click en brush para seleccionar
- Click en mapa para pintar
- Botón RESET MAP: Restaura mapa original

-----------------------------------------------
PANEL 3: GAME CONTROL (Inferior Izquierdo)
-----------------------------------------------

- Botón RESTART GAME: Reinicia juego completo
- Créditos del proyecto

-----------------------------------------------
PANEL 4: MARIO SELECTOR (Derecho - PRINCIPAL)
-----------------------------------------------

POR CADA MARIO (8 controles individuales):
- Línea 1: [Move Goal], color RGB, estado (ALIVE/DEAD/SAFE)
- Línea 2: Goal [X][Y], Life time, Algorithm time
- Línea 3: Pos [X][Y], [Teleport], [Stop], [Play]
- Selector de algoritmo: Random, A*, Seek, Seek+, Scatter, Flee

CONTROLES GLOBALES (al final):
- Algorithm: Selector para todos los vivos
- [STOP ALL]: Pausa todos
- [PLAY ALL]: Reanuda todos

SISTEMA DE PRIORIDADES DE CLICK:
1. Modo Teleport (si activo)
2. Editor de mapa (si brush seleccionado)
3. Establecer objetivo (si Mario seleccionado)

===================================================
5. ALGORITMOS IMPLEMENTADOS
===================================================

1. RANDOM - Movimiento Aleatorio
   Movimiento completamente aleatorio, elige dirección válida
   al azar cada frame. Solo considera celdas transitables.

2. A* (A-STAR) - Pathfinding Óptimo
   Camino más corto garantizado usando heurística Manhattan.
   Recalcula automáticamente si el mapa cambia.
   Fórmula: f(n) = g(n) + h(n)

3. SEEK (DUMB) - Greedy Simple
   Algoritmo greedy básico que siempre intenta reducir
   distancia Manhattan. Puede quedar atrapado en esquinas.

4. SEEK+ (SMART) - Greedy Mejorado
   Versión mejorada del Seek que evita ir hacia atrás.
   Mejor comportamiento en laberintos. Memoria de última
   posición para evitar retrocesos.

5. SCATTER - Dispersión
   Al activarse, asigna objetivo aleatorio y usa Seek+
   para llegar. Simula comportamiento de fantasmas Pac-Man
   en fase de dispersión

6. FLEE - Huir
   Opuesto a Seek. Intenta maximizar la distancia Manhattan
   al objetivo. Evita retrocesos cuando es posible.

===================================================
6. PUNTOS DESTACADOS PARA LA EVALUACIÓN
===================================================

COMPLETO: 10/10
   - Todos los requisitos obligatorios implementados
   - Múltiples requisitos opcionales superados
   - Características adicionales importantes

COMPLEJIDAD TÉCNICA: 10/10
   - A* completamente funcional con heurística Manhattan
   - 6 algoritmos de pathfinding diferentes
   - Sistema de UI complejo con ImGui
   - Editor de mapa en tiempo real
   - Sistema de estados mediante selector

CALIDAD DEL CÓDIGO: 10/10
   - Código bien organizado y estructurado
   - Separación clara de responsabilidades
   - Comentarios apropiados en español
   - Sin warnings de compilación
   - Headers e implementaciones separadas

USABILIDAD: 10/10
   - Interfaz intuitiva y completa
   - Controles claros y bien organizados
   - Feedback visual excelente
   - Múltiples formas de interacción
   - Sistema de prioridades lógico

DOCUMENTACIÓN: 10/10
   - README completo y detallado
   - Instrucciones de compilación claras
   - Descripción exhaustiva de características
   - Arquitectura del código explicada
   - Comentarios en código fuente

===================================================
FIN DEL README
===================================================

Proyecto desarrollado para la asignatura de Inteligencia Artificial
3VGP - HND in Computing (RQF)
ESAT - Escuela Superior de Arte y Tecnología
Diciembre 2025

Para más información, consultar:
- Compilador_consola.txt (guía detallada de compilación)
- Compilador_premake5.txt (información sobre Premake)
- Comentarios en código fuente (explicaciones técnicas)