#!/usr/bin/env bash
set -e

if [ "$EUID" -ne 0 ]; then
    echo "Relance avec sudo ou en root"
    exit 1
fi

echo "[+] Mise à jour des dépôts"
apt-get update -y
apt-get upgrade -y

echo "[+] Coreutils et outils POSIX de base"
apt-get install -y \
    coreutils \
    util-linux \
    binutils \
    findutils \
    diffutils \
    grep \
    gawk \
    sed \
    tar \
    gzip \
    bzip2 \
    xz-utils \
    zip \
    unzip \
    file \
    less \
    nano \
    vim \
    tree \
    bc \
    time \
    watch \
    procps \
    psmisc \
    lsof \
    pciutils \
    usbutils \
    htop \
    ncdu \
    man-db \
    info \
    whois \
    bsdextrautils

echo "[+] Shell et terminal"
apt-get install -y \
    bash \
    bash-completion \
    zsh \
    tmux \
    screen \
    readline-common \
    libreadline-dev \
    ncurses-bin \
    libncurses-dev

echo "[+] Réseau"
apt-get install -y \
    wget \
    curl \
    net-tools \
    iproute2 \
    iputils-ping \
    dnsutils \
    traceroute \
    nmap \
    netcat-openbsd \
    openssh-client \
    openssh-server \
    ca-certificates \
    openssl

echo "[+] Compilation et dev"
apt-get install -y \
    build-essential \
    gcc \
    g++ \
    make \
    cmake \
    ninja-build \
    autoconf \
    automake \
    libtool \
    pkg-config \
    gdb \
    valgrind \
    strace \
    ltrace \
    libc6-dev \
    libstdc++-12-dev

# kernel headers — pas fatal en container
apt-get install -y linux-headers-$(uname -r) 2>/dev/null || \
    echo "[!] linux-headers ignorés (normal en container)"

echo "[+] Filesystem et système"
apt-get install -y \
    e2fsprogs \
    dosfstools \
    parted \
    rsync \
    attr \
    acl \
    sudo

echo "[+] Scripting et interpréteurs"
apt-get install -y \
    python3 \
    python3-pip \
    python3-venv \
    perl \
    lua5.4 \
    jq \
    xmlstarlet

echo "[+] Git et versionning"
apt-get install -y \
    git \
    git-lfs \
    patch \
    subversion

echo "[+] Libs système"
apt-get install -y \
    libssl-dev \
    zlib1g-dev \
    libbz2-dev \
    liblzma-dev \
    libffi-dev \
    libgmp-dev \
    libncurses-dev \
    libsqlite3-dev \
    libxml2-dev \
    libxslt1-dev \
    libcurl4-openssl-dev \
    libjson-c-dev

echo "[+] Nettoyage"
apt-get autoremove -y
apt-get clean

echo ""
echo "[✔] Environnement complet installé"