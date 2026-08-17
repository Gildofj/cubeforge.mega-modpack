[CmdletBinding()]
param(
    [string]$Config = "Release",
    [switch]$Test,
    [switch]$Clean,
    [switch]$DistOnly
)

$ErrorActionPreference = "Stop"
$RootDir = $PSScriptRoot
$BuildDir = Join-Path $RootDir "build"
$DistDir = Join-Path $RootDir "dist"

Write-Host "=====================================================" -ForegroundColor Cyan
Write-Host " [CubeMegaMod] Build Automation" -ForegroundColor Cyan
Write-Host "=====================================================" -ForegroundColor Cyan

if ($Clean) {
    Write-Host "Limpando diretorios de build e dist..." -ForegroundColor Yellow
    if (Test-Path $BuildDir) { Remove-Item -Recurse -Force $BuildDir }
    if (Test-Path $DistDir) { Remove-Item -Recurse -Force $DistDir }
}

if (-not $DistOnly) {
    if (-not (Test-Path (Join-Path $BuildDir "CMakeCache.txt"))) {
        Write-Host "Configurando CMake (x64)..." -ForegroundColor Cyan
        & cmake -B $BuildDir -A x64
        if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
    }

    Write-Host "Compilando alvos ($Config)..." -ForegroundColor Cyan
    & cmake --build $BuildDir --config $Config
    if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
}

if (-not (Test-Path $DistDir)) {
    New-Item -ItemType Directory -Force -Path $DistDir | Out-Null
}

$DllSource = Join-Path $BuildDir "$Config\*.dll"
Copy-Item -Force $DllSource $DistDir
Write-Host "Mod DLL copiada para: $DistDir" -ForegroundColor Green

if ($Test) {
    Write-Host "Executando testes unitarios..." -ForegroundColor Cyan
    & ctest --test-dir $BuildDir -C $Config --output-on-failure
    if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
}

Write-Host "Concluido com sucesso!" -ForegroundColor Green
