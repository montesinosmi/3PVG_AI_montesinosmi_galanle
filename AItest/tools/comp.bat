@echo off
@cls
@echo ---------------------------------------------------
@echo  AI simulation using SDL2 + ImGui (x64)
@echo ---------------------------------------------------

rem === Rutas base (relativas a este .bat) ===
set ROOT=%~dp0..
set SRC=%ROOT%\src
set BIN=%ROOT%\bin
set BUILD=%ROOT%\build
set SDL_PATH=%ROOT%\deps\SDL2-2.32.8
set SDL_IMAGE_PATH=%ROOT%\deps\SDL2_image-2.8.8
set IMGUI_PATH=%ROOT%\deps\imgui

rem Si no pasas archivo, usa src\main.cc
set FILE=%1
if "%FILE%"=="" set FILE=%SRC%\main.cc

if not exist "%BUILD%" mkdir "%BUILD%"
if not exist "%BIN%" mkdir "%BIN%"

rem === Preguntar limpiar antes de compilar ===
choice /C SN /N /M "Limpiar antes de compilar? [S/N]: "
if errorlevel 2 goto skip_clean
call "%~dp0clear.bat"
:skip_clean

rem === Entorno VS 2022 x64 ===
@REM call "%ProgramFiles%\Microsoft Visual Studio\2022\Community\Common7\Tools\VsDevCmd.bat" -arch=x64 -host_arch=x64

@echo Compilando...


@cl /nologo /Zi /EHsc /MDd ^
  "%FILE%" ^
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
  /Fo"..\build\\" ^
  /Fe"..\bin\AItest.exe" ^
  /link /MACHINE:X64 /DEBUG /PDB:"..\bin\AItest.pdb" ^
  /link /LIBPATH:"%SDL_PATH%\lib\x64" ^
        /LIBPATH:"%SDL_IMAGE_PATH%\lib\x64" ^
        SDL2.lib SDL2main.lib SDL2_image.lib shell32.lib ^
        /SUBSYSTEM:CONSOLE ^
        /MACHINE:X64 ^
  /OUT:"%BUILD_DIR%\AItest.exe" > "%BUILD_DIR%\compile_log.txt" 2>&1


@if errorlevel 1 (
  @type "%BUILD_DIR%\compile_log.txt"
) else (
  @del "%BUILD_DIR%\compile_log.txt" >nul 2>&1
  @del *.obj >nul 2>&1
  @del *.ilk >nul 2>&1
)

rem Copiar DLLs necesarias al runtime
@copy "%SDL_PATH%\lib\x64\SDL2.dll" "%BUILD_DIR%\" >nul 2>&1
@copy "%SDL_IMAGE_PATH%\lib\x64\SDL2_image.dll" "%BUILD_DIR%\" >nul 2>&1




@echo.
@echo Build OK: "%BIN%\AItest.exe"

rem === Preguntar ejecutar ahora ===
choice /C SN /N /M "Ejecutar AItest.exe ahora? [S/N]: "
if errorlevel 2 goto end
pushd "%BIN%"
".\AItest.exe"
popd

:end
