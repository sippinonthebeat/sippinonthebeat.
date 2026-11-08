SIPPINONTHEBEAT VST3 - COMPILACION EN LA NUBE
==============================================

IMPORTANTE
Este proyecto aun es codigo fuente. Este flujo de GitHub Actions lo compila
en un servidor Windows x64 de GitHub; no instala herramientas en tu PC.
El archivo VST3 aparecera como artefacto descargable cuando la compilacion termine.

PASOS (solo necesitas navegador e Internet)
--------------------------------------------

1. Crea una cuenta gratuita en https://github.com/ si aun no tienes una.
2. Inicia sesion y abre https://github.com/new
3. En "Repository name" escribe: SIPPINONTHEBEAT
4. Selecciona "Public" (necesario para usar GitHub Actions gratis sin limites
   de minutos privados en este flujo de proyecto personal), deja las demas
   opciones como estan y pulsa "Create repository".
5. En el repositorio, pulsa "Add file" > "Upload files".
6. Extrae este ZIP en tu PC. En el Explorador de Windows activa
   Vista > Mostrar > Elementos ocultos para poder ver la carpeta .github.
7. Arrastra al navegador TODO el contenido de la carpeta
   SIPPINONTHEBEAT_0_1, incluida la carpeta oculta .github.
   Asegurate de que CMakeLists.txt quede en la raiz del repositorio y que
   tambien aparezca .github/workflows/build-windows.yml.
8. Pulsa "Commit changes".
9. Abre la pestaña "Actions". Si aparece una pantalla de confirmacion,
   pulsa "I understand my workflows, go ahead and enable them".
10. Entra al flujo "Build SIPPINONTHEBEAT VST3 (Windows x64)" y espera
    a que el trabajo termine con un visto bueno verde.
11. Abre la ejecucion completada, baja hasta "Artifacts" y descarga
    "SIPPINONTHEBEAT-Windows-x64".
12. Extrae el ZIP descargado. Dentro estara la carpeta
    SIPPINONTHEBEAT.vst3.

INSTALACION EN FL STUDIO
------------------------
Copia SIPPINONTHEBEAT.vst3 a:
C:\\Program Files\\Common Files\\VST3\\

Luego abre FL Studio > Options > Manage plugins > Find installed plugins.
Busca SIPPINONTHEBEAT en la lista de instrumentos.

PRIVACIDAD
----------
El repositorio se configura como publico para facilitar la compilacion gratuita.
No subas contraseñas, datos personales ni archivos privados.

NOTA
----
La compilacion puede revelar errores adicionales de C++ que deban corregirse.
El ZIP descargable solo aparecera si el proceso compila correctamente.
