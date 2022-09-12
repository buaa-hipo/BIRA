#!/bin/bash
python3 codec.py ./ ./
mkdir -p build && cd build
cmake -DCMAKE_INSTALL_PREFIX=`pwd`/../install .. -DDyninst_DIR=/home/lkl/spack-0.18.0/opt/spack/linux-ubuntu20.04-thunderx2/gcc-9.4.0/dyninst-master-7vgjahn3myaq2xfzhk3bsm2b2sbhkm5v/lib/cmake/Dyninst/
make -j
make install

