@echo off
setlocal EnableExtensions
set "SRC=%~dp0"
set "DST=C:\DndDice"
set "QT=C:\Qt\6.8.3\mingw_64"
set "PATH=C:\Qt\Tools\mingw1310_64\bin;C:\Qt\Tools\CMake_64\bin;C:\Qt\Tools\Ninja;%QT%\bin;%PATH%"
set "LOG=%DST%\build_log.txt"

echo ======================================== > "%LOG%"
echo DndDice build >> "%LOG%"
echo ======================================== >> "%LOG%"

if not exist "%QT%\bin\qmake.exe" (
    echo ERROR: Qt not found at %QT%
    echo ERROR: Qt not found >> "%LOG%"
    goto err
)

echo [1/5] Copy to C:\DndDice ...
if not exist "%DST%" mkdir "%DST%"
xcopy /E /I /Y /Q "%SRC%src" "%DST%\src\" >> "%LOG%" 2>&1
xcopy /E /I /Y /Q "%SRC%resources" "%DST%\resources\" >> "%LOG%" 2>&1
copy /Y "%SRC%CMakeLists.txt" "%DST%\CMakeLists.txt" >> "%LOG%" 2>&1
copy /Y "%SRC%build.bat" "%DST%\build.bat" >> "%LOG%" 2>&1

cd /d "%DST%"
if exist build_qt rmdir /s /q build_qt
mkdir build_qt
cd build_qt

echo [2/5] CMake configure ...
cmake .. -G "MinGW Makefiles" -DCMAKE_PREFIX_PATH="%QT%" -DCMAKE_BUILD_TYPE=Release >> "%LOG%" 2>&1
if errorlevel 1 goto err

echo [3/5] Compile ...
cmake --build . --parallel >> "%LOG%" 2>&1
if errorlevel 1 goto err

cd /d "%DST%"
echo [4/5] windeployqt ...
"%QT%\bin\windeployqt.exe" --no-translations --compiler-runtime "%DST%\build_qt\DndDice.exe" >> "%LOG%" 2>&1

if exist "%DST%\deploy" rmdir /s /q "%DST%\deploy"
mkdir "%DST%\deploy"
xcopy /E /I /Y /Q "%DST%\build_qt" "%DST%\deploy\" >> "%LOG%" 2>&1

echo [5/5] Copy deploy to project ...
if exist "%SRC%deploy" rmdir /s /q "%SRC%deploy"
mkdir "%SRC%deploy"
xcopy /E /I /Y /Q "%DST%\deploy" "%SRC%deploy\" >> "%LOG%" 2>&1

if not exist "%DST%\deploy\DndDice.exe" goto err

echo.
echo ========================================
echo   SUCCESS
echo   Run: C:\DndDice\deploy\DndDice.exe
echo   Or:  deploy\DndDice.exe in this folder
echo ========================================
echo Log: %LOG%
pause
exit /b 0

:err
echo.
echo *** BUILD FAILED ***
echo See log: %LOG%
pause
exit /b 1
