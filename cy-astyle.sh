#!/bin/bash

set -e
cd "$(dirname "$0")"

# Download astyle tool
TOOLS_DIR="$(pwd)/tools"
cd "$TOOLS_DIR"
case "$(uname)" in
  Darwin*)
    # macOS
    ASTYLE="$TOOLS_DIR/astyle-3.1-macos-x86_64/bin/astyle"
    if [ ! -x "$ASTYLE" ]; then
      rm -rf astyle-3.1-macos-x86_64
      ASTYLE_URI=http://lassen.aus.cypress.com/~repoman/astyle/astyle-3.1-macos-x86_64.tar.bz2
      echo "Downloading $ASTYLE_URI"
      curl -fso astyle-3.1-macos-x86_64.tar.bz2 $ASTYLE_URI
      echo "f328164c3d2b90b1927d404a198534050915c28ae4327c8fe7d7f9123d29f9c9  astyle-3.1-macos-x86_64.tar.bz2" | shasum -a 256 -c
      tar -xf astyle-3.1-macos-x86_64.tar.bz2
    fi
    ;;
  MSYS*|CYGWIN*|MINGW*)
    # Windows
    ASTYLE="$TOOLS_DIR/Astyle/bin/AStyle.exe"
    if [ ! -f "$ASTYLE" ]; then
      rm -rf Astyle
      ASTYLE_URI=http://lassen.aus.cypress.com/~repoman/astyle/AStyle_3.1_windows.zip
      echo "Downloading $ASTYLE_URI"
      curl -fso Astyle_3.1_windows.zip $ASTYLE_URI
      echo "0759d0a3b3ad768e0f1984b6faf22c5d9d53c2e081d1cfbd1cc7d6e955e995cb  AStyle_3.1_windows.zip" | sha256sum -c
      unzip -q Astyle_3.1_windows.zip
    fi
    ;;
  Linux*)
    # Linux
    ASTYLE="$TOOLS_DIR/astyle-3.1-linux-x86_64/bin/astyle"
    if [ ! -x "$ASTYLE" ]; then
      rm -rf astyle-3.1-linux-x86_64
      ASTYLE_URI=http://lassen.aus.cypress.com/~repoman/astyle/astyle-3.1-linux-x86_64.tar.bz2
      echo "Downloading $ASTYLE_URI"
      curl -fso astyle-3.1-linux-x86_64.tar.bz2 $ASTYLE_URI
      echo "40d6c942da31eef5581b83ee8c858d9ee86c96e963eca2af45b891734f2200a9 *astyle-3.1-linux-x86_64.tar.bz2" | sha256sum -c
      tar -xf astyle-3.1-linux-x86_64.tar.bz2
    fi
    ;;
  *)
    echo "Failed to find astyle"
    exit 1
    ;;
esac
cd ..

# See .travis-ci.yml, astyle job
"$ASTYLE" -n --options=.astylerc targets/TARGET_Cypress/TARGET_PSOC6/*.*
git diff --exit-code --diff-filter=d --color
