@echo off
setlocal
title SIPPINONTHEBEAT - Compilacion VST3

where cmake >nul 2>nul
if errorlevel 1 (
  echo ERROR: CMake no esta instalado o no esta agregado al PATH.
  echo Instala CMake para Windows y vuelve a intentarlo.
  pause
  exit /b 1
)

echo.
echo SIPPINONTHEBEAT - Preparando compilacion VST3 x64...
echo IMPORTANTE: Se requieren Visual Studio Build Tools 2022
echo con el componente "Desarrollo para el escritorio con C++".
echo No hace falta instalar el IDE completo de Visual Studio.
echo.

if exist build rmdir /s /q build

cmake -S . -B build -G "Visual Studio 17 2022" -A x64
if errorlevel 1 goto fail

cmake --build build --config Release
if errorlevel 1 goto fail

echo.
echo Compilacion terminada.
echo Busca SIPPINONTHEBEAT.vst3 dentro de la carpeta build.
pause
exit /b 0

:fail
echo.
echo ERROR: No se pudo compilar.
echo Comprueba que esten instalados:
echo 1. Visual Studio Build Tools 2022
echo 2. Desarrollo para el escritorio con C++
echo 3. CMake
echo.
echo La primera compilacion tambien necesita conexion a Internet
echo para descargar JUCE desde su repositorio oficial.
pause
exit /b 1
