#!/bin/sh

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
SRC_DIR="$SCRIPT_DIR/../src"
MAIN="$SRC_DIR/main.cpp"
SYSTEM="$SRC_DIR/system.cpp"

if [ ! -f "$MAIN" ]; then
    echo "main.cpp introuvable"
    exit 1
fi

if [ ! -f "$SYSTEM" ]; then
    echo "system.cpp introuvable"
    exit 1
fi

g++ "$MAIN" "$SYSTEM" -lreadline -o "$SCRIPT_DIR/../dkpshell.out"
