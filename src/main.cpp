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

std::atomic<bool> running(true);

field parse_file(const std::string &template_file)
{
    std::ifstream input_stream(template_file); 
    syntax_parser parser;
    parser.parse(input_stream);
    return std::move(parser.get_root_field());
}

void run(const std::string &template_file, const std::string &cmd) 
{
    const std::string file_name = "/dev/shm/fuzzer";
    field root = parse_file(template_file);
    executer exec(cmd);
    generation_context ctx;
    ctx.get_mapper().identify_fields(root);
    topological_sorter sorter;
    auto ordered = sorter.topological_sort(root);
    while(running) {
        for(const auto &i : ordered) {
            const_cast<field&>(ctx.get_mapper().find_field(i)).prepare(ctx);
            const_cast<field&>(ctx.get_mapper().find_field(i)).fill(ctx);
        }
        if(exec.execute(root, file_name) == executer::exec_status::killed_by_signal) {
            boost::filesystem::rename(file_name, "/tmp/carlos");
        }
    }
}

int main(int argc, char *argv[]) {
    #ifndef WIN32
        signal(SIGINT, [](int) { running = false; });
    #endif
    run("templates/test", "/usr/bin/hexdump -C");
}

