@echo off
echo === Execucao Rapida - Interface Simplificada ===
echo.

REM Verificar se executavel existe
if not exist "build\SistemaMercado.exe" (
    echo Executavel nao encontrado. Compilando...
    echo.
    call build_simple.bat
    if errorlevel 1 (
        echo ERRO: Falha na compilacao
        pause
        exit /b 1
    )
)

echo Executando Sistema Mercado...
echo.
cd build
SistemaMercado.exe

echo.
echo === Sistema Finalizado ===
pause
