@echo off
echo === Sistema Mercado - Build Script ===

:: Verificar se Qt esta disponivel
echo Verificando dependencias...

:: Tentar encontrar Qt6
set QT_DIR=
for /d %%i in (C:\Qt\6*) do set QT_DIR=%%i
if exist "%QT_DIR%" (
    echo Qt6 encontrado em: %QT_DIR%
    set CMAKE_PREFIX_PATH=%QT_DIR%\lib\cmake
    goto :build_qt
)

:: Tentar vcpkg
if exist "C:\vcpkg\installed\x64-windows\lib\cmake\Qt6" (
    echo Qt6 encontrado via vcpkg
    set CMAKE_PREFIX_PATH=C:\vcpkg\installed\x64-windows
    goto :build_qt
)

echo Qt6 nao encontrado - construindo versao simplificada
goto :build_simple

:build_qt
echo === COMPILANDO VERSAO COMPLETA COM QT ===
:: Limpar e criar diretório build
if exist build rmdir /s /q build
mkdir build
cd build

:: Configurar com Qt
cmake .. -G "MinGW Makefiles" -DCMAKE_PREFIX_PATH=%CMAKE_PREFIX_PATH% -DCMAKE_BUILD_TYPE=Release
if errorlevel 1 goto :build_simple

mingw32-make
if errorlevel 1 goto :build_simple

echo === COMPILACAO QT CONCLUIDA ===
echo Executavel: SistemaMercado.exe
goto :end

:build_simple
echo === COMPILANDO VERSAO SIMPLIFICADA ===
cd /d "%~dp0"
if exist build rmdir /s /q build
mkdir build
cd build

cmake .. -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release
if errorlevel 1 (
    echo ERRO: Falha na configuracao
    pause
    exit /b 1
)

mingw32-make
if errorlevel 1 (
    echo ERRO: Falha na compilacao
    pause
    exit /b 1
)

echo === COMPILACAO SIMPLIFICADA CONCLUIDA ===
echo Executavel: SistemaMercado.exe

:end
echo.
echo Para executar: .\SistemaMercado.exe
echo.
pause
