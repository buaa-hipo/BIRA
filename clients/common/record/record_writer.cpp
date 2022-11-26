#include "record/record_type.h"
#include "record/record_writer.h"

thread_local uint8_t* _bira_rec_buffer = NULL;
thread_local int _bira_rec_buffer_size = 0;
thread_local bool _bira_record_inited = false;
thread_local FILE *_bira_record_file;

__attribute__((constructor (RECORD_INIT_PRIORITY)))
void recordWriterCacheInit() {
    BIRA_LOG(BIRALOG_INFO, "Initialize Record Writer Library.\n");
    _bira_rec_buffer_size = 0;
    _bira_rec_buffer = (uint8_t*)malloc(PREALLOCATE_BUFFSIZE);
}

__attribute__((destructor (RECORD_FINI_PRIORITY)))
void recordWriterCacheFinalize() {
    BIRA_LOG(BIRALOG_INFO, "Finalize Record Writer Library.\n");
    RecordWriter::finalize();
    free(_bira_rec_buffer);
}

// TODO: Implement Async IO
void RecordWriter::writeAndClear() {
    fwrite(_bira_rec_buffer, 1, _bira_rec_buffer_size, _bira_record_file);
    _bira_rec_buffer_size = 0;
    return ;
}

