RMDIR ..\build /S /Q
CALL build.bat
devenv ..\build\win32\main.exe
