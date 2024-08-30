#!/bin/bash

sudo systemctl disable pulsemesh-connector.service
sudo apt-get purge pulsemesh-connector
sudo rm /etc/apt/sources.list.d/pulsemsh.list
