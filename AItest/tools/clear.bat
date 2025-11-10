@echo off
setlocal

rem === Rutas base, siempre absolutas respecto a este .bat ===
set ROOT=%~dp0..
set BIN=%ROOT%\bin
set BUILD=%ROOT%\build
set SRC=%ROOT%\src
set TOOLS=%ROOT%\tools

echo Limpieza...

rem BIN: exe, simbolos, logs, ini
if exist "%BIN%" (
  del /q "%BIN%\AItest.exe" >nul 2>&1
  del /q "%BIN%\*.pdb" >nul 2>&1
  del /q "%BIN%\*.ilk" >nul 2>&1
  del /q "%BIN%\compile_log.txt" >nul 2>&1
  del /q "%BIN%\imgui.ini" >nul 2>&1
)

rem BUILD: intermedios
if exist "%BUILD%" (
  del /q "%BUILD%\*.obj" >nul 2>&1
  del /q "%BUILD%\*.pdb" >nul 2>&1
  del /q "%BUILD%\*.ilk" >nul 2>&1
  del /q "%BUILD%\*.log" >nul 2>&1
  del /q "%BUILD%\*.tlog" >nul 2>&1
)

rem TOOLS: restos antiguos (por si acaso)
del /q "%TOOLS%\*.obj" >nul 2>&1
del /q "%TOOLS%\*.pdb" >nul 2>&1
del /q "%TOOLS%\*.ilk" >nul 2>&1
del /q "%TOOLS%\compile_log.txt" >nul 2>&1

rem SRC: PDB suelto si /Fd no se aplicó en compilaciones previas
del /q "%SRC%\vc140.pdb" >nul 2>&1

rem Borrar DLLs solo en limpieza profunda
if /i "%~1"=="all" (
  del /q "%BIN%\SDL2.dll" >nul 2>&1
  del /q "%BIN%\SDL2_image.dll" >nul 2>&1
)

echo Listo.
endlocal
