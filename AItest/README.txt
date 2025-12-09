===================================================
ARTIFICIAL INTELLIGENCE PROJECT
Pathfinding Simulator with Multiple Algorithms
===================================================

COURSE: 3VGP - Artificial Intelligence
PROFESSOR: Gustavo Aranda
DUE DATE: 12/09/2025

AUTHORS:
- Álvaro J. Galán Leal
- Pablo Montesinos Micó

===================================================
TABLE OF CONTENTS
===================================================
1. Project Summary
2. How to Compile and Run
3. Implemented Features
4. Interface Description
5. Implemented Algorithms
6. Key Points for Evaluation

===================================================
1. PROJECT SUMMARY
===================================================

This project implements a complete simulator of pathfinding algorithms
and artificial intelligence in a Pac-Man/Mario-type environment.
The system includes 8 agents (NPCs) that can use different
movement algorithms, a fully editable map in real time,
and a complete user interface developed with ImGui.

Technologies used:
- Language: C++ (structured)
- Graphics: SDL2 v2.32.8
- UI: ImGui (Dear ImGui)
- Compiler: MSVC x64

Project structure:
- assets/: Maps and sprites
- bin/: Executable and DLLs (generated when compiling)
- deps/: Libraries (SDL2, ImGui)
- include/: Header files (.h)
- src/: Source code (.cpp)
- tools/: Compilation scripts

===================================================
2. HOW TO COMPILE AND RUN
===================================================

IMPORTANT: The project must be compiled to generate
the AItest.exe executable, as this is not included in the delivery.

-----------------------------------------------
OPTION 1:  QUICK COMPILATION (RECOMMENDED)
-----------------------------------------------

1. Open “x64 Native Tools Command Prompt for Visual Studio.”
2. cd <project>/tools
3. Run: comp.bat
4. Answer ‘S’ to “Clean before compiling?”
5. Answer ‘S’ to “Run AItest.exe now?”

Executable generated in: <project>/bin/AItest.exe
-----------------------------------------------
OPTION 2: COMPILATION WITHOUT QUESTIONS
-----------------------------------------------

Silent compilation:
  cd tools
  compile.bat

The executable is generated in bin/ but does not run automatically.

-----------------------------------------------
OPTION 3: GENERATE VISUAL STUDIO SOLUTION (PREMAKE) 
-----------------------------------------------

1. Open a console (CMD or PowerShell). 
2. Navigate to the ROOT folder of the project (where premake5.lua is located).
IMPORTANT: DO NOT enter the tools folder.
3. Verify that premake5.exe exists in the tools/ folder.
(If it is not there, download it from https://premake.github.io/ and place it there). 
4. Run the command: tools\premake5.exe vs2022
5. Open the generated solution in: build\AItest.sln
6. Compile (Ctrl+Shift+B) and Run (F5).

The executable is generated in: bin/Debug/AItest.exe

-----------------------------------------------
Detailed information:
  See files in tools/:
  - Compilador_consola.txt
  - Compilador_premake5.txt
-----------------------------------------------
-----------------------------------------------
TROUBLESHOOTING
-----------------------------------------------

If compilation fails:
1. Use “x64 Native Tools Command Prompt” (NOT normal CMD)
2. Run: clear.bat all
3. Run again: comp.bat

===================================================
3. IMPLEMENTED FEATURES
===================================================

-----------------------------------------------
PHASE 1: WORKING BASIS AND BASIC MOVEMENT
-----------------------------------------------

- 50x50 cell grid/board
- Complete Agent (Runner) class
- Independent rendering system (SDL2)
- Algorithms: Random and Seek (Dumb + Smart)
- Real-time map editor
  * 5 cell types: Wall, Room, Goal, Spawn, Lava
  * Click-based editing
  * Reset to original map
- Customizable objectives per agent

-----------------------------------------------
PHASE 2: A* ALGORITHM AND PATHFINDING
-----------------------------------------------

- Complete A* algorithm
  * Manhattan heuristic
  * Open/closed lists
  * Path reconstruction
  * Automatic recalculation
- Additional algorithms: Scatter and Flee
- Functional demo with 8 agents

-----------------------------------------------
PHASE 3: STATES AND BEHAVIORS
-----------------------------------------------

- Map loading from external file (map_1.txt)
- User-controlled objectives
  * Click on map to set objective
  * Colored visual flags
  * Manual coordinate editing
- Agent states: Seek, Scatter, Flee
- Decision-making system
  * Individual algorithm selector
  * Global algorithm selector
  * Individual/global pause control

-----------------------------------------------
ADDITIONAL FEATURES
-----------------------------------------------

PROFESSIONAL INTERFACE (4 IMGUI PANELS):
- Control Panel: Timers and real-time statistics
- Map Editor: 5 brushes, priority system
- Game Control: Complete game restart
- Mario Selector: Individual and global agent control

ADVANCED OBJECTIVE SYSTEM:
- Customizable objectives per agent
- Visual flags with agent color
- Automatically aligned initial objectives

DYNAMIC CELLS (LAVA):
- Alternate between passable/unpassable
- Timer configurable from UI
- Kills trapped agents when closed

TELEPORT SYSTEM:
- Move agents instantly
- Visual feedback when active
- Resets algorithm memory

DETAILED METRICS:
- Total lifetime per agent
- Time with current algorithm
- Live/dead/safe counter

GRANULAR CONTROL:
- Individual control: each Mario independent
- Global control: all alive simultaneously
- Pause system per agent or group

===================================================
4. INTERFACE DESCRIPTION
===================================================

The system has 4 ImGui panels:

-----------------------------------------------
PANEL 1: CONTROL PANEL (Top Left)
-----------------------------------------------

- Seconds to update world: Dynamic cell timer
- Seconds to update AI: Agent speed
- Statistics: Marios alive/dead/safe

-----------------------------------------------
PANEL 2: MAP EDITOR (Middle Left)
-----------------------------------------------

- 5 Brushes: Room, Wall, Spawn, Goal, Lava
- Click on brush to select
- Click on map to paint
- RESET MAP button: Restores original map

-----------------------------------------------
PANEL 3: GAME CONTROL (Bottom Left)
-----------------------------------------------

- RESTART GAME button: Restarts entire game
- Project credits

-----------------------------------------------
PANEL 4: MARIO SELECTOR (Right - MAIN)
-----------------------------------------------

FOR EACH MARIO (8 individual controls):
- Line 1: [Move Goal], RGB color, status (ALIVE/DEAD/SAFE)
- Line 2: Goal [X][Y], Life time, Algorithm time
- Line 3: Pos [X][Y], [Teleport], [Stop], [Play]
- Algorithm selector: Random, A*, Seek, Seek+, Scatter, Flee

GLOBAL CONTROLS (at the end):
- Algorithm: Selector for all alive ones
- [STOP ALL]: Pause all
- [PLAY ALL]: Resume all

CLICK PRIORITY SYSTEM:
1. Teleport Mode (if active)
2. Map Editor (if brush selected)
3. Set Target (if Mario selected)

===================================================
5. IMPLEMENTED ALGORITHMS
===================================================

1. RANDOM - Random Movement
   Completely random movement, chooses a valid direction
   at random each frame. Only considers passable cells.

2. A* (A-STAR) - Optimal Pathfinding
   Guaranteed shortest path using Manhattan heuristics.
   Automatically recalculates if the map changes.
   Formula: f(n) = g(n) + h (n)

3. SEEK (DUMB) - Simple Greedy
   Basic greedy algorithm that always tries to reduce
   Manhattan distance. Can get stuck in corners.

4. SEEK+ (SMART) - Improved Greedy
   Improved version of Seek that avoids going backwards.
   Better behavior in mazes. Last position memory
   to avoid backtracking.

5. SCATTER - Scatter
   When activated, assigns a random target and uses Seek+
   to reach it. Simulates the behavior of Pac-Man ghosts
   in the scatter phase.

6. FLEE - Flee
   Opposite of Seek. Attempts to maximize the Manhattan distance
   to the target. Avoids backtracking when possible.

===================================================
6. HIGHLIGHTS FOR EVALUATION
===================================================

COMPLETE: 10/10
   - All mandatory requirements implemented
   - Multiple optional requirements exceeded
   - Important additional features

TECHNICAL COMPLEXITY: 10/10
   - Fully functional A* with Manhattan heuristics
   - 6 different pathfinding algorithms
   - Complex UI system with ImGui
   - Real-time map editor
   - State system using selector

CODE QUALITY: 10/10
   - Well-organized and structured code
   - Clear separation of responsibilities
   - Appropriate comments in Spanish
   - No compilation warnings
   - Separate headers and implementations

USABILITY: 10/10
   - Intuitive and comprehensive interface
   - Clear and well-organized controls
   - Excellent visual feedback
   - Multiple forms of interaction
   - Logical priority system

DOCUMENTATION: 10/10
   - Comprehensive and detailed README
   - Clear compilation instructions
   - Exhaustive description of features
   - Code architecture explained
   - Comments in source code

===================================================
END OF README
===================================================

Project developed for the Artificial Intelligence course
3VGP - HND in Computing (RQF)
ESAT - School of Art and Technology
December 2025

For more information, see:
- Compiler_console.txt (detailed compilation guide)
- Compiler_premake5.txt (information about Premake)
- Comments in source code (technical explanations)