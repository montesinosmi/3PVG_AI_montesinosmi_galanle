@echo off
@cls
@echo ---------------------------------------------------
@echo  AI simulation using SDL2 + ImGui
@echo ---------------------------------------------------

@set FILE=%1
@set SDL_PATH=..\Libs\SDL2-2.32.8
@set SDL_IMAGE_PATH=..\Libs\SDL2_image-2.8.8
@set IMGUI_PATH=..\Libs\imgui
@set BUILD_DIR=build

@if not exist "%BUILD_DIR%" mkdir "%BUILD_DIR%"

@cl /nologo /Zi /EHsc /MDd ^
  %FILE% ^
  %IMGUI_PATH%\imgui.cpp ^
  %IMGUI_PATH%\imgui_draw.cpp ^
  %IMGUI_PATH%\imgui_tables.cpp ^
  %IMGUI_PATH%\imgui_widgets.cpp ^
  %IMGUI_PATH%\backends\imgui_impl_sdl2.cpp ^
  %IMGUI_PATH%\backends\imgui_impl_sdlrenderer2.cpp ^
  /I "%SDL_PATH%\include" ^
  /I "%SDL_IMAGE_PATH%\include" ^
  /I "%IMGUI_PATH%" ^
  /I "%IMGUI_PATH%\backends" ^
  /link /LIBPATH:"%SDL_PATH%\lib\x64" ^
        /LIBPATH:"%SDL_IMAGE_PATH%\lib\x64" ^
        SDL2.lib SDL2main.lib SDL2_image.lib shell32.lib ^
  /SUBSYSTEM:CONSOLE ^
  /OUT:"%BUILD_DIR%\AItest.exe" > compile_log.txt 2>&1

@if errorlevel 1 (
  @type compile_log.txt
) else (
  @del compile_log.txt >nul 2>&1
  @del *.obj >nul 2>&1
  @del *.ilk >nul 2>&1
)
@copy "%SDL_PATH%\lib\x64\SDL2.dll" "%BUILD_DIR%\" >nul 2>&1
@copy "%SDL_IMAGE_PATH%\lib\x64\SDL2_image.dll" "%BUILD_DIR%\" >nul 2>&1