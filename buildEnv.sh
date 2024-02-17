#!/bin/bash

# ---- install docker (arch linux example) ----
# git clone https://aur.archlinux.org/docker-desktop.git
# makepkg

# ---- add user to docker group ----
sudo usermod -aG docker $USER

# ---- daemon commands ----
sudo systemctl restart docker
# systemctl status docker

# ---- build docker image ----
docker build --rm -t tiago_lobao/kichentimer:latest .
docker images | grep tiago_lobao/kichentimer

# ---- single command full example ----
# docker run --rm --privileged -v $(pwd):/build petc avr-gcc --version

# ----- creating alias command -----
if cat ~/.bashrc | grep ktenv
then
    echo "Alias already created"
else
    echo "Creating alias..."
    echo alias ktenv=\'docker run --rm --privileged -v $(pwd):/build tiago_lobao/kichentimer:latest\' >> ~/.bashrc
fi
source ~/.bashrc

echo "Done!"
# ----- search arduino serial port -----
# ls /dev/tty