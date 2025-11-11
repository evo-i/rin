#!/usr/bin/env pwsh
# Build all CPack packages for Windows

param(
    [string]$BuildDir = "build",
    [string]$Config = "Release"
)

Write-Host "Building SWAE packages..." -ForegroundColor Green

# Ensure build exists
if (-not (Test-Path $BuildDir)) {
    Write-Host "Build directory not found. Please build the project first." -ForegroundColor Red
    exit 1
}

try {
    # Copy runtime dependencies first
    Write-Host "`nCopying runtime dependencies..." -ForegroundColor Cyan
    & "$PSScriptRoot\copy-deps.ps1" -BuildDir $BuildDir
    if (-not $?) {
        Write-Host "Failed to copy dependencies" -ForegroundColor Red
        exit 1
    }
} catch {
    Write-Host "Failed to copy dependencies: $($_.Exception.Message)" -ForegroundColor Red
    exit 1
}

Push-Location $BuildDir

# Create packages directory
$PackagesDir = "packages"
if (-not (Test-Path $PackagesDir)) {
    New-Item -ItemType Directory -Path $PackagesDir | Out-Null
}

# Create ZIP package
Write-Host "`nCreating ZIP package..." -ForegroundColor Cyan
cpack -G ZIP -C $Config
if ($LASTEXITCODE -eq 0) {
    Move-Item -Force swae-*.zip $PackagesDir/ -ErrorAction SilentlyContinue
    Write-Host "✓ ZIP package created" -ForegroundColor Green
} else {
    Write-Host "✗ ZIP package failed" -ForegroundColor Red
}

# Create 7Z package (more compressed)
Write-Host "`nCreating 7Z package..." -ForegroundColor Cyan
cpack -G 7Z -C $Config
if ($LASTEXITCODE -eq 0) {
    Move-Item -Force swae-*.7z $PackagesDir/ -ErrorAction SilentlyContinue
    Write-Host "✓ 7Z package created" -ForegroundColor Green
} else {
    Write-Host "✗ 7Z package failed" -ForegroundColor Red
}

# Create NSIS installer (if NSIS is installed)
if (Get-Command makensis -ErrorAction SilentlyContinue) {
    Write-Host "`nCreating NSIS installer..." -ForegroundColor Cyan
    cpack -G NSIS -C $Config
    if ($LASTEXITCODE -eq 0) {
        Move-Item -Force swae-*.exe $PackagesDir/ -ErrorAction SilentlyContinue
        Write-Host "✓ NSIS installer created" -ForegroundColor Green
    } else {
        Write-Host "✗ NSIS installer failed" -ForegroundColor Red
    }
} else {
    Write-Host "`nSkipping NSIS installer (NSIS not found)" -ForegroundColor Yellow
    Write-Host "  Download NSIS from: https://nsis.sourceforge.io/" -ForegroundColor Gray
}

# Create WIX installer (if WiX is installed)
if (Get-Command candle -ErrorAction SilentlyContinue) {
    Write-Host "`nCreating WiX MSI installer..." -ForegroundColor Cyan
    cpack -G WIX -C $Config
    if ($LASTEXITCODE -eq 0) {
        Move-Item -Force swae-*.msi $PackagesDir/ -ErrorAction SilentlyContinue
        Write-Host "✓ WiX MSI installer created" -ForegroundColor Green
    } else {
        Write-Host "✗ WiX MSI installer failed" -ForegroundColor Red
    }
} else {
    Write-Host "`nSkipping WiX installer (WiX Toolset not found)" -ForegroundColor Yellow
}

Pop-Location

Write-Host "`nPackages created in: $BuildDir/$PackagesDir" -ForegroundColor Green
Get-ChildItem "$BuildDir/$PackagesDir/swae-*" | ForEach-Object {
    $size = [math]::Round($_.Length / 1MB, 2)
    Write-Host "  - $($_.Name) ($size MB)" -ForegroundColor Cyan
}
