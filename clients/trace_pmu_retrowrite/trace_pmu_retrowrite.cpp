#include <cstdint>
#include <iostream>
#include <string>
#include <vector>
#include <omp.h>
#include "pmu_collector.h"
#include "utils/tsc_timer.h"
#include "utils/bira_log.h"
#include "record/record_type.h"
#include "record_writer.h"

thread_local uint64_t depth;

thread_local record_pmu_t* rec;
thread_local std::vector<record_pmu_t*> rec_list;

thread_local uint8_t* _bira_rec_buffer = NULL;
thread_local int _bira_rec_buffer_size = 0;
thread_local bool _bira_record_inited = false;
thread_local FILE *_bira_record_file;

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
    BIRA_LOG(BIRALOG_INFO, "Finalize Record Writer Library.\n");

    RecordWriter::finalize();
    free(_bira_rec_buffer);
}


void RecordWriter::writeAndClear() {
    fwrite(_bira_rec_buffer, 1, _bira_rec_buffer_size, _bira_record_file);
    _bira_rec_buffer_size = 0;
    return ;
}

extern "C" void trace_entry_func() {
    if (!_bira_record_inited) {
        uint64_t thread_id = omp_get_thread_num();    
        RecordWriter::init(thread_id);
    }

    rec = (record_pmu_t*)RecordWriter::allocate(sizeof(record_pmu_t));
    rec_list.push_back(rec);
    rec[0].record.type = BIRA_COLLECTED_PMU;
    rec[0].counter.enter = pmu_collector_get(0);
    depth++;
    rec[0].record.timestamps.enter = get_tsc_raw();
    return;
}

extern "C" void trace_exit_func() {
    rec[0].record.timestamps.exit = get_tsc_raw();
    depth--;
    rec = rec_list[depth];
    rec[0].counter.exit = pmu_collector_get(0);
    rec_list.pop_back();

    return;
}
