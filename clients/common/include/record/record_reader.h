#ifndef __BIRA_RECORD_READER_H__
#define __BIRA_RECORD_READER_H__
#include "record/record_type.h"
#include "record/record_defines.h"
#include "utils/bira_log.h"
#include <unordered_map>
#include <sstream>

struct RecordHelper {
    static inline __attribute__((always_inline))
    size_t get_record_size(record_t* r) {
        switch (r->type) {
            // TODO: query based on wrapper defines
	    case BIRA_COLLECTED_PMU:
		return sizeof(record_pmu_t);
	    case BIRA_COLLECTED_PARAMS:
		return sizeof(record_params_t);
            default:
                return sizeof(record_t);
        }
        BIRA_ERROR("Should not reach here!\n");
        return 0;
    }
    // TODO: list all message types with specific record type other than record_t
    // ...
    // Dump
    static inline __attribute__((always_inline))
    std::string dump_string(record_t* r, int &event_id) {
        std::string desc;
        switch (r->type) {
	    case BIRA_COLLECTED_PMU:
            {
		event_id ++;
		desc+= std::string("=== Event ") + std::to_string(event_id) + std::string(" ===\n");
                desc+= std::string("Start Time: ") + std::to_string(r->timestamps.enter) + std::string("\n");
                desc+= std::string("End Time: ") + std::to_string(r->timestamps.exit) + std::string("\n");
                desc+= std::string("Duration: ") + std::to_string(r->timestamps.exit-r->timestamps.enter) + std::string("\n");
                record_pmu_t* rd = (record_pmu_t*)r;
		desc += std::string("PMU Counter: ") + std::to_string(rd->counter.exit-rd->counter.enter) + std::string("\n");
		break;
            }
	    case BIRA_COLLECTED_PARAMS:
	    {
		record_params_t* rd = (record_params_t*)r;
		std::ostringstream ss;
                ss << "0x" << std::hex << rd->params;

		desc += ss.str();
                desc += std::string("\n");
                break;
	    }
	    default:
	    {
		event_id ++;
		desc+= std::string("\n=== Event ") + std::to_string(event_id) + std::string(" ===\n");
                desc+= std::string("Start Time: ") + std::to_string(r->timestamps.enter) + std::string("\n");
                desc+= std::string("End Time: ") + std::to_string(r->timestamps.exit) + std::string("\n");
                desc+= std::string("Duration: ") + std::to_string(r->timestamps.exit-r->timestamps.enter) + std::string("\n");
		desc += std::string("Params List:\n");
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
    inline RecordTraceIterator find(int type, bool is_global) {
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
            if(it.val()->type==type) {
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
};

#endif
