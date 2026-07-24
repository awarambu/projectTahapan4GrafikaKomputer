@echo off
echo ========================================
echo  Compiling Kompleks Candi 3D
echo  Geby Rambu Awa - 248111064
echo ========================================

set SOURCE=*.cpp
set OUTPUT=candi.exe
set LIBS=-lfreeglut -lopengl32 -lglu32 -lwinmm -lgdi32

echo Compiling %SOURCE% into %OUTPUT% ...
g++ -Wall -O2 -o %OUTPUT% %SOURCE% %LIBS%

if %errorlevel% == 0 (
    echo.
    echo ========================================
    echo  Compilation SUCCESSFUL!
    echo  Run %OUTPUT% to start the program.
    echo ========================================
) else (
    echo.
    echo ========================================
    echo  Compilation FAILED! Check errors above.
    echo ========================================
)

pause