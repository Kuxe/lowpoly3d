#!/bin/bash
mkdir debug
cd debug
cmake -G "Ninja" --DCMAKE_BUILD_TYPE=Debug ..
cd ..