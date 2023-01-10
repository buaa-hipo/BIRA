#include <cstdint>
#include <cstdarg>
#include <iostream>
#include <string>
#include <vector>
#include <omp.h>
#include "utils/tsc_timer.h"
#include "utils/bira_log.h"
#include "record/record_type.h"
#include "record_writer.h"

thread_local uint64_t depth;

thread_local record_params_t* rec_p;
thread_local record_t* rec;
thread_local std::vector<record_t*> rec_list;

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

extern "C" void trace_entry_func(uint64_t num, ...) {
    if (!_bira_record_inited) {
        uint64_t thread_id = omp_get_thread_num();    
        RecordWriter::init(thread_id);
    }

    rec = (record_t*)RecordWriter::allocate(sizeof(record_t));
    rec_list.push_back(rec);
    rec[0].type = BIRA_DEFAULT;

    va_list valist;
    va_start(valist, num);
    for (int i = 0; i < num; i++) {
        rec_p = (record_params_t*)RecordWriter::allocate(sizeof(record_params_t));
        rec_p[0].params = va_arg(valist, uint64_t);
        rec_p[0].type = BIRA_COLLECTED_PARAMS;
    }
    va_end(valist);

    depth++;
    rec[0].timestamps.enter = get_tsc_raw();
    return;
}

extern "C" void trace_exit_func() {
    rec[0].timestamps.exit = get_tsc_raw();
    depth--;

    rec = rec_list[depth];
    rec_list.pop_back();

    return;
}
