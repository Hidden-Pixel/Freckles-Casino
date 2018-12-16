IF EXIST ..\build RMDIR ..\build /S /Q
IF NOT EXIST ..\build MKDIR ..\build
PUSHD ..\build
IF NOT EXIST .\win32 MKDIR win32
PUSHD win32
dir

cl /GS /TC /MT /O2 /FC /nologo ..\..\src\main.c ..\..\src\sound.c ..\..\src\scene.c ..\..\src\game-screen.c ..\..\src\poker.c ..\..\src\sprite-animation.c ..\..\src\buffer.c ..\..\src\input.c /I ..\..\src\include /link /ignore:4099 /incremental:no ..\..\src\libs\win32\msvc\raylib_static.lib ..\..\src\libs\win32\msvc\glfw3.lib legacy_stdio_definitions.lib kernel32.lib user32.lib gdi32.lib winspool.lib shell32.lib ole32.lib oleaut32.lib uuid.lib comdlg32.lib advapi32.lib 

IF NOT EXIST .\assets mkdir .\assets

XCOPY /S /I /Y /Q ..\..\assets .\assets

PUSHD ..\..\src
