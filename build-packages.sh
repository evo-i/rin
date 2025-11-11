#!/bin/bash
# Build all CPack packages for Linux

BUILD_DIR="${1:-build}"
CONFIG="${2:-Release}"

echo "Building SWAE packages..."

# Ensure build exists
if [ ! -d "$BUILD_DIR" ]; then
    echo "Build directory not found. Please build the project first."
    exit 1
fi

cd "$BUILD_DIR"

# Create packages directory
PACKAGES_DIR="packages"
mkdir -p "$PACKAGES_DIR"

# Create TGZ package
echo ""
echo "Creating TGZ package..."
if cpack -G TGZ -C "$CONFIG"; then
    mv -f swae-*.tar.gz "$PACKAGES_DIR/"
    echo "✓ TGZ package created"
else
    echo "✗ TGZ package failed"
fi

# Create DEB package
echo ""
echo "Creating DEB package..."
if cpack -G DEB -C "$CONFIG"; then
    mv -f swae-*.deb "$PACKAGES_DIR/"
    echo "✓ DEB package created"
else
    echo "✗ DEB package failed"
fi

# Create RPM package (if rpmbuild is available)
if command -v rpmbuild &> /dev/null; then
    echo ""
    echo "Creating RPM package..."
    if cpack -G RPM -C "$CONFIG"; then
        mv -f swae-*.rpm "$PACKAGES_DIR/"
        echo "✓ RPM package created"
    else
        echo "✗ RPM package failed"
    fi
else
    echo ""
    echo "Skipping RPM package (rpmbuild not found)"
fi

echo ""
echo "Packages created in: $BUILD_DIR/$PACKAGES_DIR"
ls -lh "$PACKAGES_DIR"/swae-* 2>/dev/null | awk '{print "  - " $9 " (" $5 ")"}'
