SIPPINONTHEBEAT VST3 - PROYECTO 0.1
===================================

Objetivo
--------
Instrumento virtual VST3 para FL Studio en Windows x64, con una interfaz Y2K y motor híbrido sample + síntesis.

Contenido de esta primera base
------------------------------
- Proyecto C++ con JUCE 8.0.8 mediante CMake.
- Plugin VST3 llamado SIPPINONTHEBEAT.
- Motor de voces MIDI con oscilador y reproducción de samples WAV integrados.
- Tres samples originales de demostración: Velvet Pluck, Chrome Bell y Dark Keys.
- Controles: mezcla sample/synth, filtro, ADSR, reverb, delay y salida.
- Cuatro presets iniciales editables.
- Interfaz Y2K oscura, cromada y con acentos rojos.

Importante
----------
Este ZIP contiene el PROYECTO FUENTE, no un VST3 ya compilado. Para generar el .vst3 hace falta compilarlo en Windows con CMake, JUCE y un compilador C++ compatible (por ejemplo Visual Studio Build Tools 2022). FL Studio no compila proyectos C++.

Compilación en Windows
----------------------
1. Instalar Visual Studio Build Tools 2022 con el componente "Desktop development with C++".
2. Instalar CMake.
3. Abrir PowerShell en esta carpeta.
4. Ejecutar:
   cmake -S . -B build -A x64
   cmake --build build --config Release
5. El plugin VST3 se generará en la carpeta build (ruta exacta según CMake/JUCE).
6. Copiar SIPPINONTHEBEAT.vst3 a:
   C:\Program Files\Common Files\VST3\
7. En FL Studio, abrir Manage plugins y realizar un escaneo.

Nota de desarrollo
------------------
Esta es una primera base funcional para continuar probando y refinando. Antes de distribuirla como producto final se debe compilar, probar en FL Studio y corregir cualquier incidencia específica del entorno.


NOTA DE COMPILACION (IMPORTANTE)
El archivo BUILD_WINDOWS.bat utiliza el generador correcto de Visual Studio 2022
para Windows x64. No utiliza NMake, porque NMake no admite el argumento -A x64.

Para compilar se necesita:
- CMake instalado.
- Visual Studio Build Tools 2022 con "Desarrollo para el escritorio con C++".
- Conexion a Internet durante la primera compilacion para descargar JUCE.

No es necesario instalar el IDE completo de Visual Studio. Sin embargo, si no
hay un compilador C++ instalado, no es posible generar el archivo VST3 localmente.
