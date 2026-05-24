@echo off
if exist "%~dp0deploy\DndDice.exe" (
    start "" "%~dp0deploy\DndDice.exe"
    exit /b 0
)
if exist "C:\DndDice\deploy\DndDice.exe" (
    start "" "C:\DndDice\deploy\DndDice.exe"
    exit /b 0
)
echo Game not built. Run build.bat first.
pause
