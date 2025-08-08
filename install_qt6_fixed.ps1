# Instalação Automática Qt6 via PowerShell
# Baseado na documentação oficial: https://doc.qt.io/qt-6/get-and-install-qt-cli.html

Write-Host "===============================================" -ForegroundColor Cyan
Write-Host "       INSTALACAO AUTOMATICA QT6              " -ForegroundColor Cyan  
Write-Host "       Baseado na documentacao oficial        " -ForegroundColor Cyan
Write-Host "===============================================" -ForegroundColor Cyan

Write-Host ""
Write-Host "Baixando Qt6 Online Installer..." -ForegroundColor Yellow

# Criar diretório temporário
$tempDir = "temp_qt"
if (!(Test-Path $tempDir)) {
    New-Item -ItemType Directory -Path $tempDir | Out-Null
}
Set-Location $tempDir

# URL do instalador oficial Qt6
$installerUrl = "https://download.qt.io/official_releases/online_installers/qt-unified-windows-x64-online.exe"
$installerFile = "qt-unified-windows-x64-online.exe"

# Download com progress bar
try {
    Write-Host "Baixando de: $installerUrl" -ForegroundColor Green
    Invoke-WebRequest -Uri $installerUrl -OutFile $installerFile -UseBasicParsing
    Write-Host "Download concluido!" -ForegroundColor Green
} catch {
    Write-Host "ERRO: Falha no download do instalador" -ForegroundColor Red
    Write-Host "Baixe manualmente: https://www.qt.io/download-open-source" -ForegroundColor Yellow
    Read-Host "Pressione Enter para continuar"
    exit 1
}

if (!(Test-Path $installerFile)) {
    Write-Host "ERRO: Arquivo do instalador nao encontrado" -ForegroundColor Red
    exit 1
}

Write-Host ""
Write-Host "Iniciando instalacao automatica do Qt6..." -ForegroundColor Green
Write-Host ""
Write-Host "O que sera instalado:" -ForegroundColor Cyan
Write-Host "   • Qt 6.8+ (latest LTS)" -ForegroundColor White
Write-Host "   • Qt Creator IDE" -ForegroundColor White  
Write-Host "   • MinGW 11.2 compiler" -ForegroundColor White
Write-Host "   • CMake integration" -ForegroundColor White
Write-Host "   • Qt Widgets, Core, SQL modules" -ForegroundColor White
Write-Host ""
Write-Host "Tempo estimado: 10-15 minutos" -ForegroundColor Yellow
Write-Host "Diretorio: C:\Qt" -ForegroundColor Yellow
Write-Host ""

# Comando de instalação baseado na documentação oficial
$installArgs = @(
    "--root", "C:\Qt",
    "--accept-licenses",
    "--accept-obligations", 
    "--default-answer",
    "--confirm-command",
    "--auto-answer", "telemetry-question=No",
    "--auto-answer", "AssociateCommonFiletypes=Yes",
    "install",
    "qt.tools.qtcreator",
    "qt.tools.cmake", 
    "qt.tools.mingw1120",
    "qt.qt6.681.win64_mingw"
)

try {
    Write-Host "Executando instalacao..." -ForegroundColor Yellow
    $process = Start-Process -FilePath ".\$installerFile" -ArgumentList $installArgs -Wait -PassThru
    
    if ($process.ExitCode -eq 0) {
        Write-Host "Qt6 instalado com sucesso!" -ForegroundColor Green
    } else {
        Write-Host "Instalacao automatica pode ter falhado. Tentando instalacao interativa..." -ForegroundColor Yellow
        Start-Process -FilePath ".\$installerFile" -ArgumentList @("--root", "C:\Qt", "install", "qt.qt6.681.win64_mingw") -Wait
    }
} catch {
    Write-Host "Erro durante instalacao: $($_.Exception.Message)" -ForegroundColor Red
    Write-Host "Tentando instalacao interativa..." -ForegroundColor Yellow
    Start-Process -FilePath ".\$installerFile" -ArgumentList @("--root", "C:\Qt", "install", "qt.qt6.681.win64_mingw") -Wait
}

Write-Host ""
Write-Host "Configurando ambiente..." -ForegroundColor Yellow

# Configurar variáveis de ambiente para esta sessão
$qtDir = "C:\Qt\6.8.1\mingw_64"
$env:QT_DIR = $qtDir
$env:CMAKE_PREFIX_PATH = "$qtDir\lib\cmake"
$env:PATH = "$qtDir\bin;C:\Qt\Tools\mingw1120_64\bin;C:\Qt\Tools\CMake_64\bin;$env:PATH"

Write-Host "Ambiente configurado!" -ForegroundColor Green
Write-Host ""
Write-Host "Proximos passos:" -ForegroundColor Cyan
Write-Host "   1. Feche este PowerShell" -ForegroundColor White
Write-Host "   2. Abra novo PowerShell" -ForegroundColor White  
Write-Host "   3. Navegue para: cd d:\sistema_mercado" -ForegroundColor White
Write-Host "   4. Execute: .\build.bat" -ForegroundColor White
Write-Host "   5. Execute: cd build && .\SistemaMercado.exe" -ForegroundColor White
Write-Host ""
Write-Host "Interface Qt estara disponivel!" -ForegroundColor Green

# Limpeza
Set-Location ..
Remove-Item -Recurse -Force $tempDir -ErrorAction SilentlyContinue

# Teste se Qt foi instalado
if (Test-Path "C:\Qt") {
    Write-Host ""
    Write-Host "Qt detectado em C:\Qt" -ForegroundColor Green
    Write-Host "Conteudo:" -ForegroundColor Cyan
    Get-ChildItem "C:\Qt" | Select-Object Name | Format-Table -AutoSize
} else {
    Write-Host ""
    Write-Host "Qt nao detectado. Verifique a instalacao." -ForegroundColor Yellow
}

Read-Host "Pressione Enter para continuar"
