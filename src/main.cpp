#ifndef WIN32
    #include <signal.h>
#endif
#include <atomic>
#include <iostream>
#include <fstream>
#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
#include "field.h"
#include "parser/syntax_parser.h"
#include "generation_context.h"
#include "topological_sorter.h"
#include "executer.h"
#include "field_serializer.h"


std::atomic<bool> running(true);
field_serializer* serializer_ptr(nullptr);

field parse_file(const std::string &template_file)
{
    std::ifstream input_stream(template_file); 
    syntax_parser parser;
    parser.parse(input_stream);
    
    std::vector<std::string> undefined;
    parser.get_mapper().find_non_registered_fields(std::back_inserter(undefined));
    if(!undefined.empty()) {
        for(const auto &str : undefined)
            std::cout << "Field \"" << str << "\" is not defined." << std::endl;
        throw parse_error();
    }
    
    return std::move(parser.get_root_field());
}

void run(const std::string &template_file, const std::string &cmd) 
{
    const std::string file_name = "/dev/shm/fuzzer.mp3";
    
    executer exec(cmd);
    field root = parse_file(template_file);
    size_t execution_count{}, crashed_count{};
    field_serializer serializer(std::move(root), 1);
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
    namespace po = boost::program_options;
    
    std::string template_file;
    std::string command;
    
    po::options_description desc("Allowed options");
    po::positional_options_description positional;
    positional.add("command", -1);
    desc.add_options()
        ("help,h", "print help message")
        ("command,c", po::value<std::string>(&command), "command to execute")
        ("template,t", po::value<std::string>(&template_file), "template file to use")
    ;


    po::variables_map vm;
    po::store(
        po::command_line_parser(argc, argv).
        options(desc).positional(positional).run(),
        vm
    );
    po::notify(vm);    

    if (vm.count("help")) {
        std::cout << desc << "\n";
        return 1;
    }

    if (vm.count("template") == 0) {
        std::cout << "template file not set\n";
        return 2;
    }
    if (vm.count("command") == 0) {
        std::cout << "command to execute not set\n";
        return 3;
    }

    try {
        run(template_file, command);
        std::cout << std::endl;
    }
    catch(std::exception &ex) {
        std::cout << ex.what() << ". aborting..." << std::endl;
    }
}

