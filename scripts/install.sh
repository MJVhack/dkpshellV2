#!/usr/bin/env bash
set -e

# vérifier que le script est lancé en root
if [ "$EUID" -ne 0 ]; then
    echo "Relance avec sudo ou en root"
    exit 1
fi

echo "[+] Mise à jour des dépôts"
apt update

echo "[+] Installation des outils de compilation"
apt install -y build-essential g++ gcc make

echo "[+] Installation des headers système"
apt install -y libc6-dev

echo "[+] Installation de readline (pour readline/readline.h)"
apt install -y libreadline-dev

echo "[+] Environnement prêt pour compiler"
