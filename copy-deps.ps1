#!/usr/bin/env pwsh
# Copy runtime dependencies for swae package

param(
  [string]$BuildDir = "build",
  [string]$MsysRoot = "F:\msys64\ucrt64"
)

# Place DLLs in deps subdirectory next to the plugin
$PluginDir = Join-Path $BuildDir "plugins\soul-worker"
$DepsDir = Join-Path $PluginDir "deps"

if (-not (Test-Path $PluginDir)) {
  Write-Host "Plugin directory not found: $PluginDir" -ForegroundColor Red
  Write-Host "Please build the project first." -ForegroundColor Red
  exit 1
}

# Create deps directory
if (-not (Test-Path $DepsDir)) {
  New-Item -ItemType Directory -Path $DepsDir -Force | Out-Null
}

# List of required DLLs from MSYS2
$RequiredDlls = @(
  "libzip.dll",
  "libbz2-1.dll",
  "liblzma-5.dll",
  "zlib1.dll",
  "libzstd.dll"
)

Write-Host "Copying runtime dependencies to deps directory..." -ForegroundColor Green

foreach ($dll in $RequiredDlls) {
  $source = Join-Path $MsysRoot "bin\$dll"
  if (Test-Path $source) {
    Copy-Item -Path $source -Destination $DepsDir -Force
    Write-Host "  ✓ Copied $dll" -ForegroundColor Green
  } else {
    Write-Host "  ✗ Not found: $dll" -ForegroundColor Red
  }
}

Write-Host "`nDependencies copied to: $DepsDir" -ForegroundColor Cyan
