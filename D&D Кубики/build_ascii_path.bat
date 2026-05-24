@echo off
chcp 65001 >nul
REM If build still fails: copies project to C:\DndDice (ASCII path) and builds there
setlocal
set "DST=C:\DndDice"
echo Copying to %DST% ...
if not exist "%DST%" mkdir "%DST%"
xcopy /y /q /e /i "%~dp0src" "%DST%\src\"
xcopy /y /q /e /i "%~dp0resources" "%DST%\resources\"
copy /y "%~dp0CMakeLists.txt" "%DST%\"
copy /y "%~dp0build_qt.bat" "%DST%\"
cd /d "%DST%"
call build_qt.bat
echo.
echo If OK, run: C:\DndDice\deploy\DndDice.exe
pause
