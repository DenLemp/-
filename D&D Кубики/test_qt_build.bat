@echo off
setlocal
cd /d "%~dp0"
set PATH=C:\Qt\Tools\mingw1310_64\bin;C:\Qt\Tools\CMake_64\bin;C:\Qt\Tools\Ninja;C:\Qt\6.8.3\mingw_64\bin;%PATH%
set LOG=%~dp0qt_build_test.log
(
echo === Qt verification %date% %time% ===
echo.
echo [qmake]
qmake -v
echo.
echo [g++]
g++ --version
echo.
echo [cmake]
cmake --version
echo.
echo [ninja]
ninja --version
echo.
echo [windeployqt]
if exist "C:\Qt\6.8.3\mingw_64\bin\windeployqt.exe" (echo OK) else (echo MISSING)
echo.
echo [Qt6Widgets]
if exist "C:\Qt\6.8.3\mingw_64\lib\cmake\Qt6Widgets\Qt6WidgetsConfig.cmake" (echo OK) else (echo MISSING)
) > "%LOG%" 2>&1
type "%LOG%"
