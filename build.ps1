# Script para configurar e compilar o Sistema Mercado no Windows

# Verificar se o Qt está instalado
Write-Host "Verificando instalação do Qt..." -ForegroundColor Yellow

$qtPath = "C:\Qt\6.5.0\msvc2019_64"
if (-not (Test-Path $qtPath)) {
    Write-Host "Qt não encontrado em $qtPath" -ForegroundColor Red
    Write-Host "Por favor, instale o Qt6 antes de continuar." -ForegroundColor Red
    Write-Host "Download: https://www.qt.io/download" -ForegroundColor Blue
    exit 1
}

Write-Host "Qt encontrado em: $qtPath" -ForegroundColor Green

# Verificar se o CMake está instalado
Write-Host "Verificando instalação do CMake..." -ForegroundColor Yellow

try {
    $cmakeVersion = cmake --version
    Write-Host "CMake encontrado: $($cmakeVersion[0])" -ForegroundColor Green
} catch {
    Write-Host "CMake não encontrado. Instalando via Chocolatey..." -ForegroundColor Yellow
    
    if (Get-Command choco -ErrorAction SilentlyContinue) {
        choco install cmake -y
    } else {
        Write-Host "Chocolatey não encontrado. Por favor, instale o CMake manualmente." -ForegroundColor Red
        Write-Host "Download: https://cmake.org/download/" -ForegroundColor Blue
        exit 1
    }
}

# Verificar se o vcpkg está instalado
Write-Host "Verificando instalação do vcpkg..." -ForegroundColor Yellow

$vcpkgPath = "C:\vcpkg"
if (-not (Test-Path $vcpkgPath)) {
    Write-Host "vcpkg não encontrado. Por favor, instale o vcpkg para gerenciar as dependências." -ForegroundColor Yellow
    Write-Host "Instruções: https://vcpkg.io/en/getting-started.html" -ForegroundColor Blue
} else {
    Write-Host "vcpkg encontrado em: $vcpkgPath" -ForegroundColor Green
    
    # Verificar se o Poco está instalado
    Write-Host "Verificando instalação do Poco..." -ForegroundColor Yellow
    
    $pocoPath = "$vcpkgPath\installed\x64-windows\include\Poco"
    if (-not (Test-Path $pocoPath)) {
        Write-Host "Poco não encontrado. Instalando via vcpkg..." -ForegroundColor Yellow
        & "$vcpkgPath\vcpkg.exe" install poco[complete]:x64-windows
    } else {
        Write-Host "Poco encontrado" -ForegroundColor Green
    }
}

# Criar diretório de build
Write-Host "Criando diretório de build..." -ForegroundColor Yellow

if (Test-Path "build") {
    Remove-Item -Recurse -Force "build"
}
New-Item -ItemType Directory -Path "build" | Out-Null

Set-Location "build"

# Configurar o projeto
Write-Host "Configurando o projeto com CMake..." -ForegroundColor Yellow

$cmakeArgs = @(
    ".."
    "-DCMAKE_PREFIX_PATH=$qtPath"
    "-DCMAKE_BUILD_TYPE=Release"
)

if (Test-Path $vcpkgPath) {
    $cmakeArgs += "-DCMAKE_TOOLCHAIN_FILE=$vcpkgPath\scripts\buildsystems\vcpkg.cmake"
}

try {
    & cmake @cmakeArgs
    
    if ($LASTEXITCODE -eq 0) {
        Write-Host "Configuração concluída com sucesso!" -ForegroundColor Green
        
        # Compilar o projeto
        Write-Host "Compilando o projeto..." -ForegroundColor Yellow
        
        & cmake --build . --config Release
        
        if ($LASTEXITCODE -eq 0) {
            Write-Host "Compilação concluída com sucesso!" -ForegroundColor Green
            Write-Host "Executável criado em: build\Release\SistemaMercado.exe" -ForegroundColor Blue
        } else {
            Write-Host "Erro na compilação!" -ForegroundColor Red
        }
    } else {
        Write-Host "Erro na configuração do CMake!" -ForegroundColor Red
    }
} catch {
    Write-Host "Erro ao executar CMake: $_" -ForegroundColor Red
}

Set-Location ".."

Write-Host "`nPara executar o sistema:" -ForegroundColor Cyan
Write-Host "cd build\Release" -ForegroundColor White
Write-Host ".\SistemaMercado.exe" -ForegroundColor White
