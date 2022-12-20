#!/bin/bash

CPLUS_INCLUDE_PATH="../../lib/:$CPLUS_INCLUDE_PATH"
export CPLUS_INCLUDE_PATH

g++ -std=c++17 -I../../lib main_pt1.cpp -o pt1
chmod +x pt1
#
#g++ -std=c++17 main_pt2.cpp -o pt2
#chmod +x pt2
