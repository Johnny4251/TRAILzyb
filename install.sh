#!/bin/bash

if [ "$EUID" -ne 0 ]; then
    echo "The install script must be run as root."
    exit 1
fi

sudo apt update
sudo apt-get install xterm
sudo apt-get install python3-venv

echo "Setting up python venv"
python3 -m venv venv
source venv/bin/activate
pip install opencv-python
deactivate

echo "Making KISSZYB"
cd kisszyb
make
sudo make install
cd ..

echo "Defining Executables"
chmod +x zybd
chmod +x run
chmod +x capture
cd zyb_txrx/cap_image
chmod +x clear_cache
cd ../..

echo "Making files"
cd zyb_txrx
make
cd ..
