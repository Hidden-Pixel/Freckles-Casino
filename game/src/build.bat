IF EXIST ..\build RMDIR ..\build /S /Q
IF NOT EXIST ..\build MKDIR ..\build
PUSHD ..\build
IF NOT EXIST .\win32 MKDIR win32
PUSHD win32
dir
cl /MDd /Zi /FC /nologo ..\..\src\game-screen.c ..\..\src\main.c /I ..\..\src\include /link /ignore:4099 /incremental:no ..\..\src\libs\win32\msvc\raylib.lib ..\..\src\libs\win32\msvc\glfw3.lib ..\..\src\libs\win32\msvc\OpenAL32.lib gdi32.lib user32.lib shell32.lib

IF NOT EXIST .\assets mkdir .\assets

XCOPY /S /I /Y /Q ..\..\assets .\assets

COPY /Y ..\..\src\libs\win32\msvc\OpenAL32.dll .\

PUSHD ..\..\src
