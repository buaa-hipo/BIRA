#include <cstdint>
#include <iostream>
#include <vector>
#include <omp.h>

thread_local uint64_t depth, lr;
thread_local std::vector<uint64_t> saved_lr;

extern "C" void trace_entry_func(uint64_t lr, uint64_t thread_id) {
    saved_lr.push_back(lr);
    depth++;
    return;
}

extern "C" uint64_t trace_exit_func() {
    depth--;
    lr = saved_lr[depth];
    saved_lr.pop_back();
    return lr;
}
