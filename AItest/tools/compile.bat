@echo off
@cls
@echo ---------------------------------------------------
@echo  AI simulation using SDL2 + ImGui
@echo ---------------------------------------------------

@set SDL_PATH=..\deps\SDL2-2.32.8
@set SDL_IMAGE_PATH=..\deps\SDL2_image-2.8.8
@set IMGUI_PATH=..\deps\imgui
@set SRC_DIR=..\src
@set INCLUDE_DIR=..\include
@set BUILD_DIR=..\bin
@set OBJ_DIR=..\build

@if not exist "%BUILD_DIR%" mkdir "%BUILD_DIR%"
@if not exist "%OBJ_DIR%" mkdir "%OBJ_DIR%"

@echo Collecting source files...
@set CPP_FILES=
@for %%f in (%SRC_DIR%\*.cpp) do (
  @call set CPP_FILES=%%CPP_FILES%% "%%f"
)

@echo Compiling project...
@cl /nologo /Zi /EHsc /MDd ^
  %CPP_FILES% ^
  "%IMGUI_PATH%\imgui.cpp" ^
  "%IMGUI_PATH%\imgui_draw.cpp" ^
  "%IMGUI_PATH%\imgui_tables.cpp" ^
  "%IMGUI_PATH%\imgui_widgets.cpp" ^
  "%IMGUI_PATH%\backends\imgui_impl_sdl2.cpp" ^
  "%IMGUI_PATH%\backends\imgui_impl_sdlrenderer2.cpp" ^
  /I "%SDL_PATH%\include" ^
  /I "%SDL_IMAGE_PATH%\include" ^
  /I "%IMGUI_PATH%" ^
  /I "%IMGUI_PATH%\backends" ^
  /I "%INCLUDE_DIR%" ^
  /Fo"%OBJ_DIR%\\" ^
  /Fe"%BUILD_DIR%\AItest.exe" ^
  /link ^
  /LIBPATH:"%SDL_PATH%\lib\x64" ^
  /LIBPATH:"%SDL_IMAGE_PATH%\lib\x64" ^
  SDL2.lib SDL2main.lib SDL2_image.lib shell32.lib ^
  /SUBSYSTEM:CONSOLE ^
  /MACHINE:X64 ^
  /DEBUG ^
  /PDB:"%BUILD_DIR%\AItest.pdb" > "%BUILD_DIR%\compile_log.txt" 2>&1

@if errorlevel 1 (
  @echo.
  @echo ========================================
  @echo  COMPILATION FAILED - See errors below:
  @echo ========================================
  @type "%BUILD_DIR%\compile_log.txt"
  @echo.
  @pause
) else (
  @echo Compilation successful!
  @del "%BUILD_DIR%\compile_log.txt" >nul 2>&1
  @del "%OBJ_DIR%\*.obj" >nul 2>&1
  @del *.obj >nul 2>&1
  @del *.ilk >nul 2>&1
  @copy "%SDL_PATH%\lib\x64\SDL2.dll" "%BUILD_DIR%\" >nul 2>&1
  @copy "%SDL_IMAGE_PATH%\lib\x64\SDL2_image.dll" "%BUILD_DIR%\" >nul 2>&1
  @echo DLLs copied to bin folder
  @echo.
  @echo Ready to run: bin\AItest.exe
)