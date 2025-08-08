@echo off
echo === Build Simplificado - Console ===
echo.

REM Limpar build anterior
if exist build rmdir /s /q build
mkdir build
cd build

echo Configurando projeto simplificado...
cmake .. -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release
if errorlevel 1 (
    echo ERRO: Falha na configuracao
    pause
    exit /b 1
)

echo Compilando...
mingw32-make
if errorlevel 1 (
    echo ERRO: Falha na compilacao
    pause
    exit /b 1
)

echo.
echo === COMPILACAO CONCLUIDA ===
echo Executavel: build\SistemaMercado.exe
echo.
echo Para executar: run_app.bat
echo.
