@echo off
echo === Sistema Mercado - Build Script ===

:: Verificar se Qt esta disponivel
echo Verificando dependencias...

:: Tentar encontrar Qt6
set QT_DIR=
for /d %%i in (C:\Qt\6*) do (
    for /d %%j in (%%i\mingw*) do (
        set QT_DIR=%%j
        goto found_qt6
    )
)

:: Tentar encontrar Qt5
for /d %%i in (C:\Qt\5*) do (
    for /d %%j in (%%i\mingw*) do (
        set QT_DIR=%%j
        goto found_qt6
    )
)

:found_qt6
if exist "%QT_DIR%" (
    echo Qt encontrado em: %QT_DIR%
    set CMAKE_PREFIX_PATH=%QT_DIR%\lib\cmake
    set PATH=%QT_DIR%\bin;%PATH%
    goto :build_qt
)

:: Tentar vcpkg
if exist "C:\vcpkg\installed\x64-windows\lib\cmake\Qt6" (
    echo Qt6 encontrado via vcpkg
    set CMAKE_PREFIX_PATH=C:\vcpkg\installed\x64-windows
    goto :build_qt
)

:: Verificar se Qt está no PATH
where qmake >nul 2>&1
if %errorlevel% == 0 (
    echo Qt encontrado no PATH do sistema
    goto :build_qt
)

echo Qt nao encontrado - construindo versao simplificada
echo.
echo 💡 Para instalar Qt6 automaticamente, execute:
echo    .\install_qt6_auto.bat
echo    ou
echo    .\install_qt6_auto.ps1
echo.
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
