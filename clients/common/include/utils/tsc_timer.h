#pragma once

#include <stdint.h>
#include <sys/time.h>

#ifdef __aarch64__
    static inline uint64_t get_tsc_freq_mhz() {
        uint64_t freq;
        asm volatile("mrs %0, cntfrq_el0"
                    : "=r"(freq));
        return freq;
    }

    static inline uint64_t get_tsc_raw() {
        uint64_t tsc;
        asm volatile("isb; mrs %0, cntvct_el0"
                    : "=r"(tsc));
        return tsc;
    }
#elif __x86_64__
    #include "tsc_freq_x86_cpuinfo.h"

    static inline uint64_t get_tsc_freq_mhz() {
        return get_tsc_freq_x86_cpuinfo_mhz();
    }

    static inline uint64_t get_tsc_raw() {
        uint32_t hi, lo, aux;
        __asm__ volatile("rdtscp"
                        : "=a"(lo), "=d"(hi), "=c"(aux));
        return ((uint64_t) hi << 32) | lo;
    }
#endif

static inline double tsc_duration_seconds(uint64_t tsc_delta, uint64_t tsc_freq_mhz) {
    return (double) (tsc_delta) / (tsc_freq_mhz * 1e6);
}

static inline double tsc_duration_ms(uint64_t tsc_delta, uint64_t tsc_freq_mhz) {
    return (double) (tsc_delta) / (tsc_freq_mhz * 1e3);
}

static inline double tsc_duration_us(uint64_t tsc_delta, uint64_t tsc_freq_mhz) {
    return (double) (tsc_delta) / tsc_freq_mhz;
}

static inline double tsc_duration_ns(uint64_t tsc_delta, uint64_t tsc_freq_mhz) {
    return (double) (tsc_delta) / (tsc_freq_mhz * 1e-3);
}

static inline uint64_t system_clock_duration_us() {
    struct timeval tv;
    struct timezone tz;
    gettimeofday(&tv, &tz);
    return tv.tv_sec * 1000000 + tv.tv_usec;
}
