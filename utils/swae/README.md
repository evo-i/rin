# SWAE - Soul Worker Archive Extractor

Standalone utility for extracting game archives with plugin support.

## Binary Packages

Pre-built packages are available via CPack:

### Creating Packages

**Important**: Before creating packages, copy runtime dependencies:
```bash
# Windows (PowerShell):
./copy-deps.ps1

# Then build packages:
./build-packages.ps1
```

The `copy-deps.ps1` script copies required DLLs (libzip, zlib, etc.) to the build directory.

Manual package creation:
```bash
# Build the project
cmake -B build -S .
cmake --build build

# Copy dependencies (Windows)
./copy-deps.ps1

# Create ZIP package (all platforms)
cd build
cpack -G ZIP -C Release
```
# Create 7Z package (smaller, better compression)
cpack -G 7Z -C Release

# Windows: Create NSIS installer (requires NSIS installed)
cpack -G NSIS -C Release

# Windows: Create MSI installer (requires WiX Toolset)
cpack -G WIX -C Release

# Linux: Create DEB package
cpack -G DEB -C Release

# Linux: Create RPM package
cpack -G RPM -C Release

# macOS: Create DMG package
cpack -G DragNDrop -C Release

# Or use the helper script to build all available packages:
# Windows:
./build-packages.ps1

# Linux/macOS:
./build-packages.sh
```

Packages will be created in the build directory:
- `swae-1.0.0-win64.zip` (Windows, ~829 KB)
- `swae-1.0.0-win64.7z` (Windows, ~633 KB, requires 7-Zip)
- `swae-1.0.0-win64.exe` (Windows NSIS installer)
- `swae-1.0.0-win64.msi` (Windows MSI installer)
- `swae-1.0.0-Linux.deb` (Debian/Ubuntu)
- `swae-1.0.0-Linux.rpm` (Fedora/RHEL)
- `swae-1.0.0-Darwin.dmg` (macOS)

Each package includes:
- `swae` executable
- Soul Worker plugin (`plugins/soul-worker/libsoul_worker.dll`)
- Runtime dependencies in `deps/` subdirectory (`libzip.dll`, `zlib1.dll`, `libbz2-1.dll`, `liblzma-5.dll`, `libzstd.dll`)

## Installation

### From Binary Package (Recommended)

#### Windows
1. Download `swae-1.0.0-win64.zip` or `swae-1.0.0-win64.exe` (NSIS installer)
2. Extract ZIP or run installer
3. Add `bin` directory to PATH (optional)

Using ZIP:
```powershell
Expand-Archive swae-1.0.0-win64.zip -DestinationPath C:\Tools\
# Optionally add C:\Tools\swae-1.0.0-win64\bin to PATH
```

#### Linux (DEB)
```bash
# Debian/Ubuntu
sudo dpkg -i swae-1.0.0-Linux.deb

# If dependencies missing:
sudo apt-get install -f
```

#### Linux (RPM)
```bash
# Fedora/RHEL
sudo rpm -i swae-1.0.0-Linux.rpm
```

#### macOS
```bash
# Mount DMG and copy to Applications or extract
open swae-1.0.0-Darwin.dmg
```

### Using CMake Install

```bash
# Build the project
cmake -B build -S .
cmake --build build

# Install to default location (/usr/local on Unix, C:/Program Files on Windows)
cmake --install build --component swae

# Or install to custom location
cmake --install build --component swae --prefix /path/to/install
```

This will install:
- `swae` executable to `<prefix>/bin/swae`
- Plugin libraries to `<prefix>/bin/plugins/<plugin-name>/`

### Manual Installation

Copy the following files to your desired location:

```
your-install-dir/
├── swae.exe (or swae on Unix)
└── plugins/
    └── soul-worker/
        ├── libsoul_worker.dll (or .so on Unix)
        └── deps/
            ├── libzip.dll (Windows runtime dependency)
            ├── zlib1.dll
            ├── libbz2-1.dll
            ├── liblzma-5.dll
            └── libzstd.dll
```

**Note**: Runtime dependencies are placed in the `deps/` subdirectory. The plugin loader automatically adds this directory to the DLL search path on Windows (using `AddDllDirectory`) and to `LD_LIBRARY_PATH` on Linux.

Example on Windows:
```powershell
# Copy swae executable
Copy-Item build\utils\swae\swae.exe C:\Tools\swae\

# Copy plugins directory
Copy-Item -Recurse build\plugins C:\Tools\swae\
```

Example on Linux/macOS:
```bash
# Copy swae executable
cp build/utils/swae/swae /usr/local/bin/

# Copy plugins directory
mkdir -p /usr/local/bin/plugins
cp -r build/plugins/* /usr/local/bin/plugins/
```

## Usage

```bash
# Interactive mode - shows list of .v files
swae /path/to/game -i

# Extract specific range
swae /path/to/game -s 1 -e 5

# Extract single file
swae /path/to/game -s 11 -e 11

# Custom output directory
swae /path/to/game -o /output/path -i

# Show help
swae -h
```

## Plugin Directory Structure

The `swae` utility searches for plugins in the `plugins/` directory relative to the executable location. The Soul Worker plugin is included by default with all its dependencies in a `deps/` subdirectory:

```
plugins/
└── soul-worker/
    ├── libsoul_worker.dll     # Soul Worker game plugin
    └── deps/                   # Runtime dependencies (not loaded directly)
        ├── libzip.dll          # ZIP archive support
        ├── zlib1.dll           # Compression
        ├── libbz2-1.dll        # BZip2 compression
        ├── liblzma-5.dll       # LZMA compression
        └── libzstd.dll         # Zstandard compression
```

The plugin loader automatically adds the `deps/` directory to the DLL search path:
- **Windows**: Uses `AddDllDirectory()` and `LoadLibraryEx()` with `LOAD_LIBRARY_SEARCH_USER_DIRS`
- **Linux**: Temporarily adds to `LD_LIBRARY_PATH` during plugin load

This keeps dependencies isolated and prevents conflicts with system libraries.

## Developing Plugins

Plugins are shared libraries (.dll on Windows, .so on Linux/macOS) that implement the Rin Plugin API. See the existing plugins in `plugins/` for examples.

Required API:
- `rin_plugin_get_info()` - Returns plugin metadata
- `rin_plugin_create()` - Creates plugin instance
- `rin_plugin_destroy()` - Destroys plugin instance

## Dependencies

- **swae**: Only standard C library (statically links cargs)
- **soul-worker plugin**: libzip library

On Windows with vcpkg:
```bash
vcpkg install libzip:x64-windows
```

On Linux:
```bash
# Debian/Ubuntu
sudo apt-get install libzip-dev

# Fedora/RHEL
sudo dnf install libzip-devel

# Arch
sudo pacman -S libzip
```
