#include <stdint.h>

bool pmu_collector_init();
bool pmu_collector_fini();
uint64_t pmu_collector_get(int i);
int pmu_collector_get_all(uint64_t*);

