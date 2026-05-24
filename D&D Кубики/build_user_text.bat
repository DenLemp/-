@echo off
setlocal EnableExtensions
set "SRC=%~dp0user_text"
set "DST=C:\UserText"
set "QT=C:\Qt\6.8.3\mingw_64"
set "PATH=C:\Qt\Tools\mingw1310_64\bin;C:\Qt\Tools\CMake_64\bin;%QT%\bin;%PATH%"
set "LOG=%DST%\build_log.txt"

echo Building UserText... > "%LOG%"

if not exist "%QT%\bin\qmake.exe" (
    echo ERROR: Qt not found
    pause
    exit /b 1
)

if not exist "%DST%" mkdir "%DST%"
xcopy /E /I /Y /Q "%SRC%" "%DST%\" >> "%LOG%" 2>&1

cd /d "%DST%"
if exist build rmdir /s /q build
mkdir build
cd build

cmake .. -G "MinGW Makefiles" -DCMAKE_PREFIX_PATH="%QT%" -DCMAKE_BUILD_TYPE=Release >> "%LOG%" 2>&1
if errorlevel 1 goto err

cmake --build . --parallel >> "%LOG%" 2>&1
if errorlevel 1 goto err

cd /d "%DST%"
"%QT%\bin\windeployqt.exe" --no-translations --compiler-runtime "%DST%\build\UserText.exe" >> "%LOG%" 2>&1

if exist deploy rmdir /s /q deploy
mkdir deploy
xcopy /E /I /Y /Q build deploy\ >> "%LOG%" 2>&1

if exist "%~dp0user_text_deploy" rmdir /s /q "%~dp0user_text_deploy"
mkdir "%~dp0user_text_deploy"
xcopy /E /I /Y /Q deploy "%~dp0user_text_deploy\" >> "%LOG%" 2>&1

if not exist "%DST%\deploy\UserText.exe" goto err

echo.
echo SUCCESS: UserText.exe
echo Run: C:\UserText\deploy\UserText.exe
echo Or:  %~dp0user_text_deploy\UserText.exe
pause
exit /b 0

:err
echo BUILD FAILED - see %LOG%
pause
exit /b 1
