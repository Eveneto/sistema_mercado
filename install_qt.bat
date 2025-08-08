@echo off
echo === Instalando Qt6 para Windows ===

:: Criar diretório temporário
mkdir temp_qt 2>nul
cd temp_qt

:: Baixar Qt6 Online Installer
echo Baixando Qt6 Online Installer...
curl -L -o qt-online-installer-windows-x64.exe "https://download.qt.io/official_releases/online_installers/qt-unified-windows-x64-online.exe"

if exist qt-online-installer-windows-x64.exe (
    echo.
    echo Qt6 Online Installer baixado com sucesso!
    echo.
    echo EXECUTANDO INSTALADOR...
    echo.
    echo INSTRUCOES:
    echo 1. Crie uma conta Qt (gratuita)
    echo 2. Selecione Qt 6.5 ou superior
    echo 3. Inclua: Qt Creator, CMake, MinGW
    echo 4. Instale em C:\Qt
    echo.
    
    :: Executar instalador
    start qt-online-installer-windows-x64.exe
    
    echo Instalador iniciado. Após a instalação:
    echo 1. Reinicie este terminal
    echo 2. Execute novamente: .\build.bat
    echo.
) else (
    echo ERRO: Falha no download do Qt6
    echo Tente baixar manualmente de: https://www.qt.io/download-open-source
)

pause
