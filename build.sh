#!/bin/bash
python3 codec.py ./ ./
mkdir -p build && cd build
cmake -DCMAKE_INSTALL_PREFIX=`pwd`/../install .. -DDyninst_DIR=${Dyninst_DIR}
make -j
make install

