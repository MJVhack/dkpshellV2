#!/bin/sh

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
PROJECT_DIR="$SCRIPT_DIR/.."
SRC_DIR="$PROJECT_DIR/src"
INCLUDE_DIR="$PROJECT_DIR/include"

MAIN="$SRC_DIR/main.cpp"
SYSTEM="$SRC_DIR/system.cpp"
DKPNET="$SRC_DIR/dkpnet.cpp"

OUTPUT="$PROJECT_DIR/dkpshell.out"

# Vérifications
for f in "$MAIN" "$SYSTEM" "$DKPNET"; do
    if [ ! -f "$f" ]; then
        echo "Erreur : $(basename $f) introuvable"
        exit 1
    fi
done

g++ "$MAIN" "$SYSTEM" "$DKPNET" \
    -I"$INCLUDE_DIR"             \
    -lreadline                   \
    -std=c++17                   \
    -o "$OUTPUT"

if [ $? -ne 0 ]; then
    echo "Erreur de compilation"
    exit 1
fi

echo "Compilation réussie : $OUTPUT"
