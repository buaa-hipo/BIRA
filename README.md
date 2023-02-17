# BiRFIA

## Build

### Dependencies

To build BiRFIA, you need the following dependencies installed:
- Python 2.7
- CMake >= 3.5
- DynInst >= 10.0.0
- PAPI
- GCC >= 9.0

For convenience, you can build and install the dependencies via spack:
```
git clone https://github.com/spack/spack.git
. ./spack/share/spack/setup-env.sh
spack install dyninst papi
spack load dyninst papi
cd ..
```

### Build & Install

```
git clone git@github.com:buaa-hipo/BIRA.git
# the compiled binaries and libraries are installed in install/
./build.sh
```

Load the environment to use the installed JSI-Toolkit:

```
# make sure we are in the root directory of this repo
cd BIRA
# load the environment variables
source env.sh
```

### How to use BiRFIA
To use BiRFIA, you neet to complete each step of the **Build & Install**
Usage:

```
bira -i <EXE> -f <FUNC> -l <custom external shared library> [--tp]
#
# -i EXE
#   EXE: the input binary file
# --tp: 
#   enable parameters tracing
# -f FUNC: 
#   FUNC: the target function name
# -l: 
#   external shared library for linking
#   
```

### Buint-in clients

#### empty_func
```
Use libempty_func.so as the parameter of bira -l to implement instrumenting empty functions before and after the target function.

$ bira -i <EXE> -f <FUNC> -l /path/to/libempty_func.so
```

#### trace_pmu
```
Use libtrace_pmu.so as the parameter of bira -l to implement a function that is instrumented before and after the target function to collect PMU counter of the target function.

$ bira -i <EXE> -f <FUNC> -l /path/to/libtrace_pmu.so
```

#### trace_params
```

Use libtrace_params.so as the parameter of bira -l to implement a function that is instrumented before and after the target function to trace the parameters of the target function.

$ bira -i <EXE> -f <FUNC> -l /path/to/libtrace_params.so --tp
```
