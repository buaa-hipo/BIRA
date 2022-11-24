#include <cstdint>
#include <iostream>
#include <string>
#include <vector>
#include "pmu_collector.h"

thread_local bool init = false;

thread_local uint64_t depth, lr;
thread_local uint64_t pmu_enter, pmu_exit;
thread_local std::vector<uint64_t> saved_lr;

void trace_init(void) __attribute__((constructor));
void trace_fini(void) __attribute__((destructor));
void trace_init(void)
{                       
    if (!pmu_collector_init()) {
	std::cerr << "PMU collector initialization failed!\n" << std::endl;
    }
}
void trace_fini(void) 
{                         
    if (!pmu_collector_fini()) {
	std::cerr << "PMU collector finalization failed!\n" << std::endl;
    }
}


extern "C" void trace_entry_func(uint64_t lr) {
    pmu_enter = pmu_collector_get(0);
    saved_lr.push_back(lr);
    depth++;
    return;
}

extern "C" uint64_t trace_exit_func() {
    pmu_exit = pmu_collector_get(0);
    std::cout << "pmu = " << pmu_exit - pmu_enter << std::endl; 
    depth--;
    lr = saved_lr[depth];
    saved_lr.pop_back();
    return lr;
}
