#!/bin/bash

sudo apt update

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
chmod +x capture
cd cap_image
chmod +x clear_cache
cd ..
