@echo off
echo ╔══════════════════════════════════════════════════════════════╗
echo ║              INSTALAÇÃO AUTOMÁTICA QT6                      ║
echo ║              Baseado na documentação oficial                ║
echo ╚══════════════════════════════════════════════════════════════╝

echo.
echo 📥 Baixando Qt6 Online Installer...

:: Criar diretório temporário
if not exist temp_qt mkdir temp_qt
cd temp_qt

:: Download Qt Online Installer para Windows
curl -O https://download.qt.io/official_releases/online_installers/qt-unified-windows-x64-online.exe

if not exist qt-unified-windows-x64-online.exe (
    echo ❌ ERRO: Falha no download do instalador
    echo 💡 Baixe manualmente: https://www.qt.io/download-open-source
    pause
    exit /b 1
)

echo ✅ Download concluído!
echo.
echo 🚀 Iniciando instalação automática do Qt6...
echo.
echo 📋 O que será instalado:
echo    • Qt 6.8+ (latest LTS)
echo    • Qt Creator IDE
echo    • MinGW 11.2 compiler
echo    • CMake integration
echo    • Qt Widgets, Core, SQL modules
echo.
echo ⏱️  Tempo estimado: 10-15 minutos
echo 📁 Diretório: C:\Qt
echo.

:: Instalação automática sem interação do usuário
:: Usando os comandos da documentação oficial Qt6
qt-unified-windows-x64-online.exe ^
    --root C:\Qt ^
    --accept-licenses ^
    --accept-obligations ^
    --default-answer ^
    --confirm-command ^
    --auto-answer telemetry-question=No ^
    --auto-answer AssociateCommonFiletypes=Yes ^
    install qt.tools.qtcreator qt.tools.cmake qt.tools.mingw1120 qt.qt6.681.win64_mingw

if errorlevel 1 (
    echo.
    echo ❌ Instalação automática falhou
    echo 🔧 Tentando instalação interativa...
    qt-unified-windows-x64-online.exe --root C:\Qt install qt.qt6.681.win64_mingw
) else (
    echo.
    echo ✅ Qt6 instalado com sucesso!
)

echo.
echo 🔧 Configurando ambiente...

:: Adicionar Qt ao PATH (temporário para esta sessão)
set QT_DIR=C:\Qt\6.8.1\mingw_64
set CMAKE_PREFIX_PATH=%QT_DIR%\lib\cmake
set PATH=%QT_DIR%\bin;C:\Qt\Tools\mingw1120_64\bin;C:\Qt\Tools\CMake_64\bin;%PATH%

echo ✅ Ambiente configurado!
echo.
echo 📋 Próximos passos:
echo    1. Feche este terminal
echo    2. Abra novo terminal (PowerShell)
echo    3. Navegue para: cd d:\sistema_mercado
echo    4. Execute: .\build.bat
echo    5. Execute: cd build && .\SistemaMercado.exe
echo.
echo 🎉 Interface Qt estará disponível!

cd ..
rmdir /s /q temp_qt

pause
