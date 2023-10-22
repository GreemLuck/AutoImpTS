#!/bin/bash

# Update package list
sudo apt-get update

# Install C++ packages
sudo apt-get install -y libarmadillo-dev
sudo apt-get install libmlpack-dev

# Install Python packages
pip install -r python_packages.txt
