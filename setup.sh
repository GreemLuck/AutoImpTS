#!/bin/bash

# Update package list
sudo apt-get update

# Install C++ packages
sudo apt-get install -y libarmadillo-dev

# Install Python packages
pip install -r requirements.txt
