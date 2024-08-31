#!/bin/bash

sudo systemctl --now disable pulsemesh-connector.service
sudo apt-get purge -y pulsemesh-connector
sudo rm /etc/apt/sources.list.d/pulsemsh.list

echo "Removed pulsemesh-connector"