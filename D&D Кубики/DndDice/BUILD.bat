@echo off
setlocal EnableExtensions
set "DST=C:\DndDice"
set "QT=C:\Qt\6.8.3\mingw_64"
set "PATH=C:\Qt\Tools\mingw1310_64\bin;C:\Qt\Tools\CMake_64\bin;%QT%\bin;%PATH%"
set "LOG=%DST%\build_log.txt"

if not exist "%QT%\bin\qmake.exe" (
    echo ERROR: Install Qt 6.8.3 mingw_64
    pause
    exit /b 1
)

cd /d "%DST%"
if not exist build_qt mkdir build_qt
cd build_qt
cmake .. -G "MinGW Makefiles" -DCMAKE_PREFIX_PATH="%QT%" -DCMAKE_BUILD_TYPE=Release
if errorlevel 1 pause & exit /b 1
cmake --build . --parallel
if errorlevel 1 pause & exit /b 1
cd /d "%DST%"
"%QT%\bin\windeployqt.exe" --no-translations --compiler-runtime "%DST%\build_qt\DndDice.exe"
if exist deploy rmdir /s /q deploy
mkdir deploy
xcopy /Y /Q build_qt\*.exe deploy\
xcopy /Y /Q build_qt\*.dll deploy\
if exist build_qt\platforms xcopy /E /I /Y /Q build_qt\platforms deploy\platforms\
echo OK: C:\DndDice\deploy\DndDice.exe
pause
