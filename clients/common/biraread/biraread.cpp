#include <iostream>
#include "utils/cxxopts.hpp"
#include "record/record_reader.h"
#include <filesystem>
#include <stdio.h>

namespace fs = std::filesystem;

bool is_dump = false;
bool is_force = false;
bool is_thread = false;
std::string input_dir;
std::string output_dir;

void parse(int argc, char *argv[]) {
    cxxopts::Options options("jsiread", "Read the measurement directory generated by jsirun.");
    options.add_options()
        ("d,dump", "Dump readable outputs")
        ("t,thread", "Dump by thread id")
        ("f,force", "Force to remove the output directory if already exists.")
        ("h,help", "Print help")
        ("i,input", "Input measurement directory generated by jsirun", cxxopts::value<std::string>())
        ("o,output", "Output directory to store the readable outputs",cxxopts::value<std::string>()->default_value("readable"))
        ;

    try {
        auto result = options.parse(argc, argv);
        // print help message if configured
        if (result.count("help")) {
            std::cout << options.help() << std::endl;
            exit(1);
        }
        if (result.count("input")>=1) {
            if(result.count("input")>2) {
                std::cout << "Warning: multiple input directory configured. Only use the last one!" << std::endl;
            }
            input_dir = result["input"].as<std::string>();
        } else {
            std::cout << "-i or --input must be specified!" << std::endl;
            exit(1);
        }
        if (result.count("output") || result.count("dump")) {
            is_dump = true;
        }
        if (is_dump) {
            output_dir = result["output"].as<std::string>();
        }
        if (result.count("force")) {
            is_force = true;
        }
	if(result.count("thread")) {
	    is_thread = true;
	}
        std::cout << "Configured: " << std::endl;
        std::cout << "\t" << "Input: " << input_dir << std::endl;
        std::cout << "\t" << "Output: " << (is_dump ? output_dir : std::string("<stdout>")) << std::endl;
    } catch (cxxopts::exceptions::exception e) {
        std::cout << "Error: " << e.what() << std::endl;
        std::cout << options.help() << std::endl;
        exit(1);
    }
}

int main(int argc, char *argv[]) {
    // arg parse
    parse(argc, argv);
    std::string rankOrThread = is_thread ? "Thread":"Rank";
    try {
        RecordReader reader(input_dir.c_str());
        RecordTraceCollection& traces = reader.get_all_traces();
	std::cout << traces.size() << std::endl;
        if (is_dump) {
            if(fs::exists(output_dir)) {
                if(is_force) {
                    fs::remove_all(output_dir);
                } else {
                    std::cout << "Error: output directory (" << output_dir << ") exists!" << std::endl;
                    exit(1);
                }
            }
            if(!fs::create_directories(output_dir)) {
                std::cout << "Error: failed to create output directory (" << output_dir << ")!" << std::endl;
                exit(1);
            }
        }
        for(auto it : traces) {
        // for(auto it=traces.begin(); it!=traces.end(); ++it) {
            FILE* fp;
            if(is_dump) {
                std::string fn = output_dir + std::string("/result.") + std::to_string(it.first);
                fp = fopen(fn.c_str(), "w");
                if (fp==NULL) {
                    std::cout << "Error: failed to open output file " << fn << std::endl;
                    exit(1);
                }
            } else {
                fp = stdout;
            }
            std::cout << "## " << rankOrThread << " " << it.first << std::endl;
            RecordTrace* rt = it.second;
            int i = 0, event_id = 0;
            for(RecordTraceIterator rit=rt->begin(); rit!=rt->end(); rit=rit.next(),++i) {
                // fprintf(fp, "=== Event %d ===\n", i);
                fprintf(fp, "%s", RecordHelper::dump_string(rit.val(), event_id).c_str());
            }
	    if(is_dump) {
                fclose(fp);
            }
        }
    } catch (fs::filesystem_error const& e) {
        std::cout << e.what() << std::endl;
        exit(1);
    }
    return 0;
}
