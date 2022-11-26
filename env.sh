#!/bin/bash
ROOT=`pwd`
ROOT=$ROOT/install
echo "BIRA Install Directory: $ROOT"
export PATH=$ROOT/bin:$PATH
export LD_LIBRARY_PATH=$ROOT/lib:$LD_LIBRARY_PATH
