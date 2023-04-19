#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static inline uint64_t get_cpu_base_freq_x86_cpuinfo_mhz() {
    FILE* file = fopen("/proc/cpuinfo", "r");
    char line[256];
    while (fgets(line, sizeof(line), file)) {
        char* pos = strstr(line, "model name");
        if (pos == NULL) {
            continue;
        }

        char* pos_freq_begin = strchr(line, '@');
        char* pos_freq_end;
        while ((pos_freq_end = strchr(pos_freq_begin + 1, '@')) != NULL) {
            pos_freq_begin = pos_freq_end;
        }

        pos_freq_begin += 2;

        double freq_ghz = strtod(pos_freq_begin, &pos_freq_end);

        if (pos_freq_end[0] == 'G') {
            return (uint64_t) (freq_ghz * 1000);
        } else {
            // unreachable
            goto error;
        }
        break;
    }

error:
    fprintf(stderr, "Invalid cpuinfo format");// might be replaced with a logger in the future
    exit(-1);

    // unreachable
    return 0;
}

static inline uint64_t get_tsc_freq_x86_cpuinfo_mhz() {
    return get_cpu_base_freq_x86_cpuinfo_mhz();
}