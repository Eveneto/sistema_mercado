#!/usr/bin/env pwsh
# Script para instalar Qt6 automaticamente

Write-Host "=== Instalador Qt6 para Sistema Mercado ===" -ForegroundColor Green

# Método 1: Tentar winget (Qt6)
Write-Host "Tentando instalação via winget..." -ForegroundColor Yellow
try {
    winget install --id=Qt.QtDesigner -e --accept-source-agreements --accept-package-agreements
    if ($LASTEXITCODE -eq 0) {
        Write-Host "Qt Designer instalado com sucesso!" -ForegroundColor Green
        exit 0
    }
} catch {
    Write-Host "Winget falhou, tentando alternativas..." -ForegroundColor Yellow
}

# Método 2: Instalar via vcpkg
Write-Host "Tentando instalação via vcpkg..." -ForegroundColor Yellow
if (Test-Path "C:\tools\vcpkg") {
    Set-Location "C:\tools\vcpkg"
    .\vcpkg.exe install qt6-base:x64-windows
    .\vcpkg.exe install qt6-widgets:x64-windows
    if ($LASTEXITCODE -eq 0) {
        Write-Host "Qt6 instalado via vcpkg!" -ForegroundColor Green
        Write-Host "Configure CMAKE_TOOLCHAIN_FILE=C:\tools\vcpkg\scripts\buildsystems\vcpkg.cmake" -ForegroundColor Cyan
        exit 0
    }
}

# Método 3: Download manual do Qt Online Installer
Write-Host "Baixando Qt Online Installer..." -ForegroundColor Yellow
$qtUrl = "https://download.qt.io/official_releases/online_installers/qt-unified-windows-x64-online.exe"
$qtInstaller = "$env:TEMP\qt-installer.exe"

try {
    Invoke-WebRequest -Uri $qtUrl -OutFile $qtInstaller -UseBasicParsing
    Write-Host "Qt Installer baixado. Executando..." -ForegroundColor Green
    
    # Executar installer em modo silencioso (se possível)
    Start-Process -FilePath $qtInstaller -Wait
    
    Write-Host "Instalar Qt6 Base, Widgets e Tools através da interface" -ForegroundColor Cyan
    Write-Host "Localização sugerida: C:\Qt" -ForegroundColor Cyan
    
} catch {
    Write-Host "Erro no download: $($_.Exception.Message)" -ForegroundColor Red
}

Write-Host ""
Write-Host "=== Instruções Manuais ===" -ForegroundColor Cyan
Write-Host "1. Baixe Qt6 de: https://www.qt.io/download-qt-installer" -ForegroundColor White
Write-Host "2. Instale Qt 6.5+ com componentes:" -ForegroundColor White
Write-Host "   - Qt 6.5.x Desktop (MinGW ou MSVC)" -ForegroundColor White
Write-Host "   - Qt Creator" -ForegroundColor White
Write-Host "   - CMake" -ForegroundColor White
Write-Host "3. Após instalação, execute: cmake -DCMAKE_PREFIX_PATH=C:\Qt\6.5.x\mingw_64" -ForegroundColor White
Write-Host ""
Write-Host "Ou use o método alternativo simplificado..." -ForegroundColor Yellow
