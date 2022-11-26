#ifndef __BIRA_RECORD_TYPE_H__
#define __BIRA_RECORD_TYPE_H__

// #include <mpi.h>
#include <unordered_map>
#include <vector>
#include <string>

#define BIRA_DEFAULT ((int16_t)(-2))
#define BIRA_COLLECTED_PARAMS ((int16_t)(-1))
#define BIRA_COLLECTED_PMU ((int16_t)(0))

#ifndef BIRA_NO_PACKED
#define __RECORD_ATTR__ __attribute__((__packed__))
#else
#define __RECORD_ATTR__ 
#endif

struct metric_t {
    uint64_t enter, exit;
};

struct __RECORD_ATTR__ record_t {
    int16_t type;
    int count;
    metric_t timestamps;
};

struct __RECORD_ATTR__ record_pmu_t {
    record_t record;
    // TODO: Try to store runtime configurable number of counters
    metric_t counter;
};


struct __RECORD_ATTR__ record_params_t {
    record_t record;
    std::vector<uint64_t> params_list;
};

#endif
