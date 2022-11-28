#!/bin/bash
python3 codec.py ./ ./
mkdir -p build && cd build
cmake -DCMAKE_INSTALL_PREFIX=`pwd`/../install .. -DDyninst_DIR=/root/spack/opt/spack/linux-ubuntu20.04-aarch64/gcc-9.4.0/dyninst-12.2.0-srzw622srfw6wz5raojdirtbsqdd4k7l/lib/cmake/Dyninst/
make -j
make install

