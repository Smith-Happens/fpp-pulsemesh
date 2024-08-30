#!/bin/bash

sudo systemctl disable pulsemesh-connector.service
sudo apt-get purge pulsemesh-connector.service
sudo rm /etc/apt/sources.list.d/pulsemsh.list
