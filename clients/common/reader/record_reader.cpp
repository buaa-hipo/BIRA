#include "record/record_type.h"
#include "record/record_reader.h"
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>

// Using <filesystem> introduced in C++17
#include <filesystem>
namespace fs = std::filesystem;

RecordTrace::RecordTrace(void* trace_mmap, int rank, int start, int end, int size, int cache_size) {
    _rank = rank;
    _trace_mmap = trace_mmap;
    _trace_view_start = trace_mmap;
    _trace_view_end = (void*)(((char*)trace_mmap) + size);
    _start = start;
    _end = end;
    _size = size;
    _cache_size = cache_size;
}

RecordTrace::~RecordTrace() {
    munmap(_trace_mmap, _size);
}

void RecordTrace::detect_size() {
    _start = 0;
    _end   = 0;
    for(RecordTraceIterator it=begin();
        it!=end();
        it=it.next(),++_end) {}
}

RecordTraceIterator RecordTrace::get(int i)  {
    ASSERT_MSG(i>=0 && i<size(), "RecordReader::get Illegal arguments");
    RecordTraceIterator it=begin();
    while(i!=0) { it=it.next(); --i; }
    return it;
}

bool RecordTrace::zoom(uint64_t ts_s, uint64_t ts_e) {
    BIRA_ERROR("RecordTrace::zoom Not Implemented!\n");
}

RecordTrace* RecordTrace::filter(int MsgType) {
    BIRA_ERROR("RecordTrace::filter Not Implemented!\n");
}

RecordReader::RecordReader(const char* dir) {
    load(dir);
}

RecordReader::~RecordReader() { 
    for(auto it=_collection.begin();it!=_collection.end();++it) {
        delete it->second;
    }
    _collection.clear();
}

void RecordReader::load(const char* dir) {
    auto trace_ext = fs::path(BIRA_TRACE_FILE_EXT);
    // walk through the directory
    for (const auto& dirEntry : fs::directory_iterator(dir)) {
        //std::cout << dirEntry << std::endl;
        if(dirEntry.is_regular_file()) {
            const auto& path = dirEntry.path();
            if(path.extension()==trace_ext) {
                const char* rank_str = path.stem().extension().c_str()+1;
                int rank = std::stoi(rank_str);
                BIRA_INFO("RecordReader::load(): %d\n", rank);
                _load_trace(path.c_str(), rank);
            } else {
                BIRA_WARN("RecordReader::load>> Ignore file %s with unknown extension.\n", path.c_str());
            }
        }
    }
}

void RecordReader::_load_trace(const char* fn, int rank) {
    // open the file, read only
    int fd = open(fn, O_RDONLY);
    off_t fsize = lseek(fd, 0, SEEK_END);
    lseek(fd, 0, SEEK_SET); // reset
    // load trace
    void* trace_mmap = mmap(0, fsize, PROT_READ, MAP_PRIVATE/*may use MAP_SHARED?*/, fd, 0);
    _collection[rank] = new RecordTrace(trace_mmap, rank, -1, -1, fsize);
    // close the file
    close(fd);
}

RecordTrace& RecordReader::get_trace(int rank) {
    return *(_collection[rank]);
}

RecordTraceCollection& RecordReader::get_all_traces() {
    return _collection;
}

