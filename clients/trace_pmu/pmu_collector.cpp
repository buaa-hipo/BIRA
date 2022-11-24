#include "pmu_collector.h"

#include <papi.h>

#include <cstdlib>

#include "pthread.h"

namespace {
    bool _pmu_initialized = false;
    int _eventset = PAPI_NULL;
    int _event_code = PAPI_NULL;
}

// Load PMU collection from environment variable: JSI_COLLECT_PMU_EVENT
// Currently, only support collection of 1 event, return false when failed
// The initialization need to do:
// 1) read the configured pmu event and registered it via PAPI
// 2) clear the pmu counter to make it count from 0
// Here we assume we are in 64-bit platforms, and the pmu counter is also 64-bit,
// thus the pmu counter can be considered as never overflowed by the 64-bit counter
bool pmu_collector_init() {
    if (PAPI_library_init(PAPI_VER_CURRENT) != PAPI_VER_CURRENT) {
        return false;
    }
    if (PAPI_thread_init(pthread_self) != PAPI_OK) {
        return false;
    }

    _eventset = PAPI_NULL;

    if (PAPI_create_eventset(&_eventset) != PAPI_OK) {
        return false;
    }

    const char *event_name = getenv("JSI_COLLECT_PMU_EVENT");
    if (event_name == nullptr) {
        return false;
    }

    int r = PAPI_event_name_to_code(event_name, &_event_code);
    if (r != PAPI_OK) {
        return false;
    }

    r = PAPI_add_event(_eventset, _event_code);
    if (r != PAPI_OK) {
        return false;
    }

    PAPI_start(_eventset);
    PAPI_reset(_eventset);

    _pmu_initialized = true;
    return true;
}

// finalize and free the workspace, return false when failed
bool pmu_collector_fini() {
    long long dummy;
    int r = PAPI_stop(_eventset, &dummy);
    if (r != PAPI_OK) {
        return false;
    }

    r = PAPI_cleanup_eventset(_eventset);
    if (r != PAPI_OK) {
        return false;
    }

    r = PAPI_destroy_eventset(&_eventset);
    if (r != PAPI_OK) {
        return false;
    }

    PAPI_shutdown();

    _event_code = PAPI_NULL;
    _eventset = PAPI_NULL;
    _pmu_initialized = false;
    return true;
}

// read the counter value of the i-th registered event
uint64_t pmu_collector_get(int i) {
    if (!_pmu_initialized) {
	return 0;
    }

    if (i != 0) {
	return 0;
    }

    long long val;
    int r = PAPI_read(_eventset, &val);
    if (r != PAPI_OK) {
	return 0;
    }

    return val;
}

/*
// read all counter values of the registered events
// return the number of read events.
int pmu_collector_get_all(uint64_t *ptr) {
    if (!_pmu_initialized) {
        JSI_ERROR("PMU collector not initialized. Aborting...\n");
    }

    int r = PAPI_read(_eventset, reinterpret_cast<long long *>(ptr));
    if (r != PAPI_OK) {
        JSI_WARN("PAPI_read failed with error code: %d (%s:%d)\n", r, __FILE__, __LINE__);
    }

    return 1;
}
*/
