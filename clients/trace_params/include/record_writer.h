#ifndef __RECORD_WRITER_H__
#define __RECORD_WRITER_H__

#include <string>
#include "utils/tsc_timer.h"
#include "utils/bira_log.h"
#include "record/record_defines.h"
// 4KB preallocate buffer size
#define PREALLOCATE_BUFFSIZE 4096

#define RECORD_INIT_PRIORITY 101
#define RECORD_FINI_PRIORITY 101

extern thread_local uint8_t* _bira_rec_buffer;
extern thread_local int _bira_rec_buffer_size;
extern thread_local bool _bira_record_inited;
extern thread_local FILE *_bira_record_file;

struct RecordWriter {
    private:
    static void writeAndClear();
    
    public:
    inline static void init(int thread_id) {
	    /*
        if(_bira_record_inited) {
            return ;
        }
	*/
        _bira_rec_buffer = (uint8_t*)malloc(PREALLOCATE_BUFFSIZE);
        char* envPath = getenv("BIRA_MEASUREMENT_DIR");
        if(!envPath) {
            BIRA_ERROR("(thread_id=%d): BIRA_MEASUREMENT_DIR must be set!\n", thread_id);
        }
        std::string base_dir(envPath);
        std::string fn(DEFAULT_PROFILE_BASE);
        fn = base_dir + std::string("/") + fn + std::to_string(thread_id)+std::string(BIRA_TRACE_FILE_EXT);
        const char* filename = fn.c_str();
        if((_bira_record_file=fopen(filename, "w"))==NULL) {
            BIRA_ERROR("(thread_id=%d): failed to open file: %s\n", thread_id, filename);
        }
        _bira_record_inited = true;
    }
    
    inline static void init() {
        RecordWriter::init(0);
    }
    
    inline static void* allocate(size_t size) {
        if(_bira_rec_buffer_size+size>=PREALLOCATE_BUFFSIZE) {
            RecordWriter::writeAndClear();
        }
        void* p = _bira_rec_buffer + _bira_rec_buffer_size;
        _bira_rec_buffer_size += size;
        return p;
    }
    
    inline static void finalize() {
        if(!_bira_record_inited) {
            return;
        }
        if(_bira_rec_buffer_size != 0) {
            RecordWriter::writeAndClear();
            _bira_rec_buffer_size = 0;
        }
        fclose(_bira_record_file);
        _bira_record_inited = false;
    }
};

#endif
