#!/bin/bash

# fpp-pulsemesh install script

BASEDIR=$(dirname $0)
cd $BASEDIR
cd ..
make "SRCDIR=${SRCDIR}"

wget -O - https://repo.pulsemesh.io/pulsemsh-repo-key.gpg | sudo apt-key add -
sudo sh -c 'echo "deb [arch=amd64,arm64,armhf] https://repo.pulsemesh.io stable main" > /etc/apt/sources.list.d/pulsemsh.list'
sudo apt update -o Dir::Etc::sourcelist="/etc/apt/sources.list.d/pulsemsh.list" -o Dir::Etc::sourceparts="-" -o APT::Get::List-Cleanup="0"
sudo apt install -y pulsemesh-connector
sudo systemctl enable pulsemesh-connector.service
sudo systemctl start pulsemesh-connector.service

. ${FPPDIR}/scripts/common
setSetting restartFlag 1
