#!/bin/bash

# Check if root
if [ "$EUID" -ne 0 ]; then
    printf "Please run this script as root.\n"
    exit
fi

# Install libraries
sudo apt-get install libudev-dev \
    libfreetype6-dev fonts-freefont-ttf ttf-bitstream-vera \
    autoconf automake libtool autotools-dev build-essential pkg-config

# Install bcm2835 library
wget http://www.airspayce.com/mikem/bcm2835/bcm2835-1.56.tar.gz
tar -zxf bcm2835-1.56.tar.gz
cd bcm2835-1.56
./configure
sudo make check
sudo make install
cd ..
rm -rf bcm2835-1.56 bcm2835-1.56.tar.gz

# Copy RC522 conf in /etc
sudo cp ./lib/rc522/RC522.conf /etc/
