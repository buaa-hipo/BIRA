#include <cstdint>
#include <iostream>
#include <string>
#include <vector>
#include <omp.h>
#include "utils/tsc_timer.h"
#include "utils/bira_log.h"
#include "record/record_type.h"
#include "record_writer.h"

thread_local uint64_t depth, lr;
thread_local std::vector<uint64_t> saved_lr;

thread_local record_params_t* rec;

thread_local uint8_t* _bira_rec_buffer = NULL;
thread_local int _bira_rec_buffer_size = 0;
thread_local bool _bira_record_inited = false;
thread_local FILE *_bira_record_file;

void trace_init(void) __attribute__((constructor));
void trace_fini(void) __attribute__((destructor));
void trace_init(void)
{                       
}

void trace_fini(void) 
{                         
    BIRA_LOG(BIRALOG_INFO, "Finalize Record Writer Library.\n");

    RecordWriter::finalize();
    free(_bira_rec_buffer);
}


void RecordWriter::writeAndClear() {
    fwrite(_bira_rec_buffer, 1, _bira_rec_buffer_size, _bira_record_file);
    _bira_rec_buffer_size = 0;
    return ;
}

extern "C" void trace_entry_func(uint64_t lr, uint64_t params_0, uint64_t params_1) {
    if (!_bira_record_inited) {
        uint64_t thread_id = omp_get_thread_num();    
        RecordWriter::init(thread_id);
    }

    rec = (record_params_t*)RecordWriter::allocate(sizeof(record_params_t));
    rec[0].record.type = BIRA_COLLECTED_PARAMS;
    rec[0].record.timestamps.enter = get_tsc_raw();
    rec[0].params_list[0] = params_0;
    rec[0].params_list[1] = params_1;
    saved_lr.push_back(lr);
    depth++;
    return;
}

extern "C" uint64_t trace_exit_func() {
    rec[0].record.timestamps.exit = get_tsc_raw();
    depth--;
    lr = saved_lr[depth];
    saved_lr.pop_back();
    return lr;
}
