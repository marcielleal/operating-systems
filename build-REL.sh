#!/bin/sh

export CPP_FLAGS=" -Wall -Wextra -O2 -c -std=c++11 "
export LINK_FLAGS=" -lcurses "
./util/build.sh $(cat version.txt)