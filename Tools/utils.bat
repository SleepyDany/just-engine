@echo off

python --version >nul 2>&1
if %errorlevel% NEQ 0 (
    echo "ERROR: Python is not installed."
    exit /b 1
)

cmake --version >nul 2>&1
if %errorlevel% NEQ 0 (
    echo "ERROR: cmake is not installed."
    exit /b 1
)

clang-format --version >nul 2>&1
if %errorlevel% NEQ 0 (
    echo "ERROR: clang-format is not installed."
    exit /b 1
)

set NEED_HELP=FALSE
if "%~1"=="" set NEED_HELP=TRUE
if "%~1"=="--help" set NEED_HELP=TRUE
if "%NEED_HELP%"=="TRUE" (
    python Tools/utils.py --help
    exit /b 1
)

python Tools/utils.py %*
