@echo off

REM Change directory to the script's location
cd /d %~dp0

REM Define the include and source directories
set INCLUDE_DIR=include
set SRC_DIR=src

REM Define the output executable name
set OUTPUT=Expense_Tracker.exe

REM Compile the project
g++ -I%INCLUDE_DIR% %SRC_DIR%\Expense.cpp %SRC_DIR%\Expense_Tracker.cpp %SRC_DIR%\main.cpp -o %OUTPUT%

REM Check if the compilation was successful
if %errorlevel% neq 0 (
    echo Compilation failed!
    pause
    exit /b %errorlevel%
)

echo Compilation successful! Executable created: %OUTPUT%
pause