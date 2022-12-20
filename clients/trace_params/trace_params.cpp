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

thread_local uint64_t depth, lr;
thread_local std::vector<uint64_t> saved_lr;

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

// extern "C" void trace_entry_func(uint64_t lr, uint64_t params_0, uint64_t params_1) {
// extern "C" void trace_entry_func(uint64_t lr, uint64_t num, ...) {
extern "C" void trace_entry_func(uint64_t lr, uint64_t num, uint64_t params_0, uint64_t params_1) {
    if (!_bira_record_inited) {
        uint64_t thread_id = omp_get_thread_num();    
        RecordWriter::init(thread_id);
    }

    rec = (record_t*)RecordWriter::allocate(sizeof(record_t));
    rec_list.push_back(rec);
    rec[0].type = BIRA_DEFAULT;
    rec[0].timestamps.enter = get_tsc_raw();

    /*
    va_list valist;
    va_start(valist, num);
    for (int i = 0; i < num; i++) {
        rec_p = (record_params_t*)RecordWriter::allocate(sizeof(record_params_t));
        rec_p[0].params = va_arg(valist, uint64_t);
        rec_p[0].type = BIRA_COLLECTED_PARAMS;
    }
    va_end(valist);
*/
        rec_p = (record_params_t*)RecordWriter::allocate(sizeof(record_params_t));
        rec_p[0].params = params_0;
        rec_p[0].type = BIRA_COLLECTED_PARAMS;

        rec_p = (record_params_t*)RecordWriter::allocate(sizeof(record_params_t));
        rec_p[0].params = params_1;
        rec_p[0].type = BIRA_COLLECTED_PARAMS;
    saved_lr.push_back(lr);
    depth++;
    return;
}

extern "C" uint64_t trace_exit_func() {
    depth--;

    rec = rec_list[depth];
    rec[0].timestamps.exit = get_tsc_raw();
    rec_list.pop_back();

    lr = saved_lr[depth];
    saved_lr.pop_back();
    return lr;
}
