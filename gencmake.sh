#!/bin/sh

mkdir build_release
cd build_release
cmake -DCMAKE_BUILD_TYPE=Release -G KDevelop3 ../
mkdir ../build_debug
cd ../build_debug
cmake -DCMAKE_BUILD_TYPE=Debug -G KDevelop3 ../
