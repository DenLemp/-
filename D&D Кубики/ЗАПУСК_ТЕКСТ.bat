@echo off
if exist "%~dp0user_text_deploy\UserText.exe" (
    start "" "%~dp0user_text_deploy\UserText.exe"
    exit /b 0
)
if exist "C:\UserText\deploy\UserText.exe" (
    start "" "C:\UserText\deploy\UserText.exe"
    exit /b 0
)
echo Run build_user_text.bat first.
pause
