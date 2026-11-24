@echo off
setlocal enableextensions

cd /d "%~dp0"

set "PYTHON_EXE=C:\practica\venv\Scripts\python.exe"

echo Configuring CMake...
cmake -S . -B build -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Debug
if %ERRORLEVEL% neq 0 (
    echo [ERROR] CMake configuration failed.
    exit /b %ERRORLEVEL%
)

echo.
echo Building project and tests...
cmake --build build
if %ERRORLEVEL% neq 0 (
    echo [ERROR] Build failed.
    exit /b %ERRORLEVEL%
)

echo.
echo Running unit tests...
ctest --test-dir build --output-on-failure
if %ERRORLEVEL% neq 0 (
    echo [ERROR] Some unit tests failed.
    exit /b %ERRORLEVEL%
)

if exist "%PYTHON_EXE%" (
    echo.
    echo Generating coverage report...
    "%PYTHON_EXE%" -m gcovr -r .. --object-directory build --filter ".*/(task_01|task_02)/src/.*" --exclude-branches-by-pattern ".*" --html-details build/coverage.html
    if exist "build\coverage.html" (
        echo Coverage report saved to: build\coverage.html
        start "" "build\coverage.html"
    ) else (
        echo [WARN] Coverage report was not generated.
    )
) else (
    echo.
    echo [INFO] Python executable not found at "%PYTHON_EXE%", skipping HTML coverage generation.
)

endlocal