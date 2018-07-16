#!/bin/bash

# Name of build's directory
BUILD="build"

# Create obj/ directory if none
if [ ! -d $BUILD ]; then
	mkdir $BUILD
fi

cd $BUILD

cmake ..

make