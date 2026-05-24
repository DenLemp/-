@echo off
setlocal
set "SRC=%~dp0"
set "DST=C:\DndDice"
set "QT=C:\Qt\6.8.3\mingw_64"

echo ========== DndDice project check ==========
echo.
echo Project: %SRC%
echo.

if exist "%QT%\bin\qmake.exe" (echo [OK] Qt: %QT%) else (echo [!!] Qt MISSING: %QT%)
if exist "C:\Qt\Tools\mingw1310_64\bin\g++.exe" (echo [OK] MinGW g++) else (echo [!!] MinGW MISSING)
if exist "C:\Qt\Tools\CMake_64\bin\cmake.exe" (echo [OK] CMake) else (echo [!!] CMake MISSING)
echo.

echo --- Source files ---
if exist "%SRC%src\qt\MainWindow.cpp" (echo [OK] Qt UI) else (echo [!!] MainWindow.cpp)
if exist "%SRC%src\DiceRoller.cpp" (echo [OK] DiceRoller) else (echo [!!] DiceRoller.cpp)
if exist "%SRC%CMakeLists.txt" (echo [OK] CMakeLists.txt) else (echo [!!] CMakeLists.txt)
if exist "%SRC%build.bat" (echo [OK] build.bat) else (echo [!!] build.bat)
echo.

echo --- Build output ---
if exist "%DST%\deploy\DndDice.exe" (echo [OK] C:\DndDice\deploy\DndDice.exe) else (echo [--] Not built in C:\DndDice yet)
if exist "%SRC%deploy\DndDice.exe" (echo [OK] Project deploy\DndDice.exe) else (echo [--] No deploy\DndDice.exe here)
if exist "%SRC%build_qt" (echo [WARN] Old build_qt folder - safe to delete, use build.bat)
echo.

if exist "%DST%\build_log.txt" (
    echo Last lines of build_log.txt:
    powershell -NoProfile -Command "Get-Content '%DST%\build_log.txt' -Tail 8"
) else (
    echo No build_log.txt - run build.bat first
)
echo.
echo To build: double-click build.bat or SOBRAT.bat
echo.
pause
