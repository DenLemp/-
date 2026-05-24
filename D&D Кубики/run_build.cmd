@echo off
cd /d "C:\D&D Кубики"
call build_qt.bat > build_log.txt 2>&1
echo EXIT=%ERRORLEVEL%>> build_log.txt
