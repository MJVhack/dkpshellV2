#!/bin/sh

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
PROJECT_DIR="$SCRIPT_DIR/.."
SRC_DIR="$PROJECT_DIR/src"
INCLUDE_DIR="$PROJECT_DIR/include"
MAIN="$SRC_DIR/main.cpp"
SYSTEM="$SRC_DIR/system.cpp"
OUTPUT="$PROJECT_DIR/dkpshell.out"


if [ ! -f "$MAIN" ]; then
    echo "Erreur : main.cpp introuvable"
    exit 1
fi

if [ ! -f "$SYSTEM" ]; then
    echo "Erreur : system.cpp introuvable"
    exit 1
fi

g++ "$MAIN" "$SYSTEM" -I"$INCLUDE_DIR" -lreadline -o "$OUTPUT"

# vérification compilation
if [ $? -ne 0 ]; then
    echo "Erreur de compilation"
    exit 1
fi

echo "Compilation réussie : $OUTPUT"
