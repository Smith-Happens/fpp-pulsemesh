#!/bin/bash

sudo systemctl disable pulsemesh-connector.service
sudo apt-get purge -y pulsemesh-connector
sudo rm /etc/apt/sources.list.d/pulsemsh.list
