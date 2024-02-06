#!/bin/bash

# ---- install docker (arch linux example) ----
# git clone https://aur.archlinux.org/docker-desktop.git
# makepkg

# ---- add user to docker group ----
sudo usermod -aG docker $USER

# ---- daemon commands ----
sudo systemctl restart docker
systemctl status docker

# ---- build docker image ----
docker build --rm -t tiago_lobao/kichentimer:latest .
docker images | grep tiago_lobao/kichentimer

# ---- single command full example ----
# docker run --rm --privileged -v $(pwd):/build petc avr-gcc --version

# ----- creating alias command -----
alias ktenv='docker run --rm --privileged -v $(pwd):/build tiago_lobao/kichentimer:latest'

# ----- search arduino serial port -----
# ls /dev/tty