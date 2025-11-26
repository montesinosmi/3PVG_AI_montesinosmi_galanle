@echo off
setlocal

rem === Definir ruta raíz (asumiendo que este bat está en /tools) ===
set "ROOT=%~dp0.."

echo ===================================================
echo  LIMPIEZA DE PROYECTO (PREMAKE + SDL2)
echo ===================================================

rem --- AVISO DE SEGURIDAD ---
echo IMPORTANTE: Cierra Visual Studio antes de continuar.
echo Si esta abierto, la carpeta .vs no se podra borrar.
echo ===================================================

rem 1. ELIMINAR CARPETA BUILD COMPLETA
if exist "%ROOT%\build" (
    echo [BORRANDO] Carpeta build completa...
    
    rem Intentamos borrar
    rmdir /s /q "%ROOT%\build"
    
    rem Verificamos si se borró o si Windows lo bloqueó
    if exist "%ROOT%\build" (
        echo [ERROR] No se pudo borrar 'build' completamente.
        echo         Probablemente Visual Studio sigue abierto o reteniendo archivos.
        echo         Cierralo y vuelve a intentar.
    ) else (
        echo [OK] Carpeta build eliminada.
    )
) else (
    echo [INFO] La carpeta build no existia.
)

rem 2. LIMPIAR BIN (Solo basura compilada, NO las DLLs)
if exist "%ROOT%\bin" (
    echo [LIMPIEZA] Limpiando ejecutables en bin...
    del /s /q "%ROOT%\bin\*.exe" >nul 2>&1
    del /s /q "%ROOT%\bin\*.pdb" >nul 2>&1
    del /s /q "%ROOT%\bin\*.ilk" >nul 2>&1
    del /s /q "%ROOT%\bin\*.exp" >nul 2>&1
    del /s /q "%ROOT%\bin\*.lib" >nul 2>&1
    del /s /q "%ROOT%\bin\imgui.ini" >nul 2>&1
)

echo.
echo ===================================================
echo  Limpieza terminada.
echo  Ahora ejecuta el Generador (GenProjects.bat)
echo ===================================================
pause