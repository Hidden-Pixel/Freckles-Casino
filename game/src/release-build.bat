@echo off

IF EXIST ..\release-build RMDIR ..\release-build /S /Q
IF NOT EXIST ..\release-build MKDIR ..\release-build
PUSHD ..\release-build
IF NOT EXIST .\win32 MKDIR win32
PUSHD win32
dir

set BuildNumber=v0.1.3-Alpha
set PreprocessorFlags=-DGAME_SOUND_ENABLED

cl /MDd /GS /TC /O2 /FC /Fe"Freckles-Casino-%BuildNumber%.exe" /nologo^
 ..\..\src\main.c^
 ..\..\src\sound.c^
 ..\..\src\scene.c^
 ..\..\src\game-screen.c^
 ..\..\src\poker.c^
 ..\..\src\sprite-animation.c^
 ..\..\src\buffer.c^
 ..\..\src\input.c^
 ..\..\src\ai-behavior.c^
 ..\..\src\commands.c^
 %PreprocessorFlags%^
 /I ..\..\src\include /link /ignore:4099 /incremental:no^
 ..\..\src\libs\win32\msvc\raylib.lib ..\..\src\libs\win32\msvc\glfw3.lib kernel32.lib user32.lib gdi32.lib winspool.lib shell32.lib ole32.lib oleaut32.lib uuid.lib comdlg32.lib advapi32.lib

DEL .\*.obj

IF NOT EXIST .\assets mkdir .\assets

XCOPY /S /I /Y /Q ..\..\assets .\assets

7z a Freckles-Casino-%BuildNumber%.7z

PUSHD ..\..\src
