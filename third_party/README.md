# Tools implementation based on DynamoRIO and RetroWrite

## Build

### Dependencies

To build these tools, you need the following dependencies installed:
- [DynamoRIO](https://github.com/DynamoRIO/dynamorio)  (tested on tag:cronbuild-9.0.19314)
- [RetroWrite](https://github.com/HexHive/retrowrite) (tested on branch:09d76d69caafd81974eb94d5357a7f1b0c9ac398)

### Build & Install

1. Put the `dr_func_wrap` under /path/to/dynamorio/clients.

2. Put the `func_wrapper` under /path/to/retrowrite/rwtools_arm64/. Replace the retrowrite in original repo with `retrowrite` in this dir.

3. Build dynamorio and retrowrite following the instructions on their repo.

### How to use these tools

1. `dr_func_wrap`

```
$ /path/to/bin64/drrun -t dr_func_wrap -f <FUNC_NAME> [-n <PARAMS_NUM>] -- <EXE>

- -f: Input target function name.
- -n: Target function params num. (for trace_params)
- <EXE>: Input binary.
```

2. `func_wrapper`

```
$ /path/to/retrowrite/retrowrite -m func_wrapper -f <FUNC_NAME> [-p <PARAMS_NUM>] <EXE> <OUTPUT_ASM>
$ echo // DEPENDENCY: <LIB> >> <OUTPUT_ASM>
$ recompile <OUTPUT_ASM> e.g. g++ <OUTPUT_ASM> -lxx -Lxx -march=xx -o <EXE-NEW>


- -f: Input target function name.
- -p: Target function params num. (for trace_params)
- <EXE>: Input binary.
```
