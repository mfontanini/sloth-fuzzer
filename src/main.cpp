#include <iostream>
#include <fstream>
#ifndef WIN32
    #include <signal.h>
#endif
#include <atomic>
#include <boost/filesystem.hpp>
#include "field.h"
#include "field_mapper.h"
#include "topological_sorter.h"
#include "parser/syntax_parser.h"
#include "generation_context.h"
#include "executer.h"
#include "field_serializer.h"

std::atomic<bool> running(true);
field_serializer* serializer_ptr(nullptr);

field parse_file(const std::string &template_file)
{
    std::ifstream input_stream(template_file); 
    syntax_parser parser;
    parser.parse(input_stream);
    return std::move(parser.get_root_field());
}

void run(const std::string &template_file, const std::string &cmd) 
{
    const std::string file_name = "/dev/shm/fuzzer.mp3";
    
    executer exec(cmd);
    field root = parse_file(template_file);
    size_t execution_count{}, crashed_count{};
    field_serializer serializer(std::move(root), 4);
    serializer_ptr = &serializer;
    while(running) {
        field_serializer::serialization_type f = serializer.next_field();
        if(running) {
            execution_count++;
            if(execution_count % 300 == 0) {
                std::cout << "\r[+] Executing test number " << execution_count << ". Crashed: " << crashed_count;
                std::cout.flush();
            }
            if(exec.execute(f.begin(), f.end(), file_name) == executer::exec_status::killed_by_signal) {
                boost::filesystem::rename(file_name, "/dev/shm/carlos");
                crashed_count++;
            }
        }
    }
}

int main(int argc, char *argv[]) {
    #ifndef WIN32
        signal(SIGINT, [](int) { 
            running = false; 
            if(serializer_ptr) 
                serializer_ptr->stop();
        });
    #endif
    try {
        //run("templates/png", "/usr/bin/convert {%} /dev/shm/foo.jpg");
        run("templates/mp3", "/usr/local/bin/mpg123");
        //run("templates/test", "/usr/bin/hexdump -C");
        std::cout << std::endl;
    }
    catch(std::exception &ex) {
        std::cout << std::endl;
        std::cout << "Terminated due to exception: " << ex.what() << std::endl;
    }
}

