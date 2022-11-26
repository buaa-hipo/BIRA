#ifndef __JSI_RECORD_READER_H__
#define __JSI_RECORD_READER_H__
#include "record/record_type.h"
#include "record/record_defines.h"
#include "utils/jsi_log.h"
#include <unordered_map>

struct RecordHelper {
    /* TODO: implement record name lookup function via MsgType */
    static inline __attribute__((always_inline))
    std::string get_record_name(record_t* r) {
        if(JSI_MSG_IS_FUNC(r->MsgType)) {
            return std::string("Function");
        }
        switch(r->MsgType) {
            __RECORD_MAP_IMPL;
            case JSI_PROCESS_START:
                return std::string("PROCESS_START");
            case JSI_PROCESS_EXIT:
                return std::string("PROCESS_EXIT");
            case JSI_TOOL_IO_MSGTYPE:
                return std::string("Profiler IO");
            default:
                return std::string("Unknown");
        }
        return std::string("Unknown");
    }
    static inline __attribute__((always_inline))
    size_t get_record_size(record_t* r) {
        switch (r->MsgType) {
            // TODO: query based on wrapper defines
            case event_MPI_Send:
            case event_MPI_Recv:
                return sizeof(record_comm_t);
            case event_MPI_Isend:
            case event_MPI_Irecv:
                return sizeof(record_comm_async_t);
            case event_MPI_Wait:
                return sizeof(record_comm_wait_t);
            case event_MPI_Alltoall:
            case event_MPI_Alltoallv:
                return sizeof(record_all2all_t);
            case event_MPI_Allreduce:
                return sizeof(record_allreduce_t);
            case event_MPI_Reduce:
                return sizeof(record_reduce_t);
            case event_MPI_Bcast:
                return sizeof(record_bcast_t);
            default:
                return sizeof(record_t);
        }
        JSI_ERROR("Should not reach here!\n");
        return 0;
    }
    /* always inlined boolean helper functions to 
     * determine the record type via header msg */
    static inline __attribute__((always_inline))
    bool is_function(record_t* r) {
        return JSI_MSG_IS_FUNC(r->MsgType);
    }
    static inline __attribute__((always_inline))
    bool is_profiler(record_t* r) {
        return r->MsgType == JSI_TOOL_IO_MSGTYPE;
    }
    static inline __attribute__((always_inline))
    bool is_process_start(record_t* r) {
        return r->MsgType == JSI_PROCESS_START;
    }
    static inline __attribute__((always_inline))
    bool is_process_exit(record_t* r) {
        return r->MsgType == JSI_PROCESS_EXIT;
    }
    static inline __attribute__((always_inline))
    bool is_event(record_t* r, int16_t event_id) {
        return r->MsgType == event_id;
    }
    // TODO: list all message types with specific record type other than record_t
    // ...
    // Dump
    static inline __attribute__((always_inline))
    std::string dump_string(record_t* r) {
        std::string desc;
        desc = std::string("Event Name: ") + get_record_name(r) + std::string("\n");
        desc+= std::string("Start Time: ") + std::to_string(r->timestamps.enter) + std::string("\n");
        desc+= std::string("End Time: ") + std::to_string(r->timestamps.exit) + std::string("\n");
        desc+= std::string("Duration: ") + std::to_string(r->timestamps.exit-r->timestamps.enter) + std::string("\n");
	if(is_function(r)) {
	    desc += std::string("Count: ") + std::to_string(r->counter.exit-r->counter.enter) + std::string("\n");
	}
        switch (r->MsgType) {
            // TODO: query based on wrapper defines
            case event_MPI_Send:
            case event_MPI_Recv:
            {
                record_comm_t* rd = (record_comm_t*)r;
                desc += std::string("Data type: ") + std::to_string(rd->datatype) + std::string("\n");
                desc += std::string("Dest/Src: ") + std::to_string(rd->dest) + std::string("\n");
                desc += std::string("Tag: ") + std::to_string(rd->tag) + std::string("\n");
                desc += std::string("Count: ") + std::to_string(rd->count) + std::string("\n");
                desc += std::string("MPI Comm: ") + std::to_string(rd->comm) + std::string("\n");
                break;
            }
            case event_MPI_Isend:
            case event_MPI_Irecv:
            {
                record_comm_async_t* rd = (record_comm_async_t*)r;
                desc += std::string("Data type: ") + std::to_string(rd->datatype) + std::string("\n");
                desc += std::string("Dest/Src: ") + std::to_string(rd->dest) + std::string("\n");
                desc += std::string("Tag: ") + std::to_string(rd->tag) + std::string("\n");
                desc += std::string("Count: ") + std::to_string(rd->count) + std::string("\n");
                desc += std::string("MPI Comm: ") + std::to_string(rd->comm) + std::string("\n");
                desc += std::string("MPI Request: ") + std::to_string(rd->request) + std::string("\n");
                break;
            }
            case event_MPI_Wait:
            {
                record_comm_wait_t* rd = (record_comm_wait_t*)r;
                desc += std::string("MPI Request: ") + std::to_string(rd->request) + std::string("\n");
                break;
            }
            case event_MPI_Alltoall:
            case event_MPI_Alltoallv:
            {
                record_all2all_t* rd = (record_all2all_t*)r;
                desc += std::string("Data type: ") + std::to_string(rd->datatype) + std::string("\n");
                desc += std::string("Send count: ") + std::to_string(rd->sendcnt) + std::string("\n");
                desc += std::string("Recv count: ") + std::to_string(rd->recvcnt) + std::string("\n");
                desc += std::string("MPI Comm: ") + std::to_string(rd->comm) + std::string("\n");
                break;
            }
            case event_MPI_Allreduce:
            {
                record_allreduce_t* rd = (record_allreduce_t*)r;
                desc += std::string("Data type: ") + std::to_string(rd->datatype) + std::string("\n");
                desc += std::string("Count: ") + std::to_string(rd->count) + std::string("\n");
                desc += std::string("MPI Op: ") + std::to_string(rd->op) + std::string("\n");
                desc += std::string("MPI Comm: ") + std::to_string(rd->comm) + std::string("\n");
                break;
            }
            case event_MPI_Reduce:
            {
                record_reduce_t* rd = (record_reduce_t*)r;
                desc += std::string("Data type: ") + std::to_string(rd->datatype) + std::string("\n");
                desc += std::string("Count: ") + std::to_string(rd->count) + std::string("\n");
                desc += std::string("Root: ") + std::to_string(rd->root) + std::string("\n");
                desc += std::string("MPI Op: ") + std::to_string(rd->op) + std::string("\n");
                desc += std::string("MPI Comm: ") + std::to_string(rd->comm) + std::string("\n");
                break;
            }
            case event_MPI_Bcast:
            {
                record_bcast_t* rd = (record_bcast_t*)r;
                desc += std::string("Data type: ") + std::to_string(rd->datatype) + std::string("\n");
                desc += std::string("Count: ") + std::to_string(rd->count) + std::string("\n");
                desc += std::string("Root: ") + std::to_string(rd->root) + std::string("\n");
                desc += std::string("MPI Comm: ") + std::to_string(rd->comm) + std::string("\n");
                break;
            }
        }
        return desc;
    }
};

class RecordTraceIterator {
    private:
    record_t* _cur;
    public:
    record_t* val() { return _cur; }
    RecordTraceIterator next() {
        return RecordTraceIterator((void*)(((uint8_t*)_cur)+RecordHelper::get_record_size(_cur)));
    }
    RecordTraceIterator(void* cur) { _cur=(record_t*)cur; }
    RecordTraceIterator(const RecordTraceIterator& prev) { _cur = prev._cur; }
    bool operator==(RecordTraceIterator const & r) {return _cur == r._cur;}
    bool operator!=(RecordTraceIterator const & r) {return _cur != r._cur;}
    static bool is_invalid(RecordTraceIterator const & r) {return r._cur==NULL; }
    static RecordTraceIterator invalid() { return RecordTraceIterator(0); }
};

class RecordTrace {
    private:
    int _rank;
    int _start, _end;
    int _size;
    int _cache_size;
    // the trace is ordered in time series
    // TODO: cachable record trace reading
    void* _trace_mmap;

    void* _trace_view_start;
    void* _trace_view_end;

    void detect_size();
    
    public:
    uint64_t offset;
    /* rfile: Trace File,  */
    RecordTrace(void* trace_mmap, int rank, int start, int end, int size, int cache_size=4096);
    ~RecordTrace();
    inline int size() {
        if(_end==-1) {
            detect_size();
        }
        return _end-_start; 
    }
    inline RecordTraceIterator begin() { 
        return RecordTraceIterator(_trace_view_start);
    }
    inline RecordTraceIterator end() { 
        return RecordTraceIterator(_trace_view_end);
    }
    /* return the first found record with given MsgType */
    inline RecordTraceIterator find(int MsgType, bool is_global) {
        RecordTraceIterator it(0);
        RecordTraceIterator ie(0);
        if(is_global) {
            it = RecordTraceIterator(_trace_mmap);
            ie = RecordTraceIterator((void*)(((char*)_trace_mmap) + _size));
            
        } else {
            it = begin();
            ie = end();
        }
        while(it!=ie) {
            if(it.val()->MsgType==MsgType) {
                return it;
            }
            it = it.next();
        }
        return RecordTraceIterator::invalid();
    }
    /* get i-th record */
    inline RecordTraceIterator get(int i);
    /* filter the trace from timestamp ts_s to timestamp ts_e
     * modify: _start, _end
     * return: whether success or not
     */
    bool zoom(uint64_t ts_s, uint64_t ts_e);
    /* filter out all events with MsgType and return the filtered trace */
    RecordTrace* filter(int MsgType);
};

/* Collection of MPI & Function Traces */
typedef std::unordered_map<int,RecordTrace*> RecordTraceCollection;

class RecordReader {
    private:
    RecordTraceCollection _collection;
    void _load_trace(const char* fn, int rank);
    void _load_meta(const char* fn, int rank);
    
    public:
    RecordReader(const char* dir);
    ~RecordReader();
    void load(const char* dir);
    RecordTrace& get_trace(int rank);
    RecordTraceCollection& get_all_traces();
    MetaDataMap& get_meta_map(int rank);
    int get_all_meta_maps(MetaDataMap* mlist);
};

#endif
