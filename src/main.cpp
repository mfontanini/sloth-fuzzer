/*
 * Copyright (c) 2012, Matias Fontanini
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 * 
 * * Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 * * Redistributions in binary form must reproduce the above
 *   copyright notice, this list of conditions and the following disclaimer
 *   in the documentation and/or other materials provided with the
 *   distribution.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#ifndef WIN32
    #include <signal.h>
#endif
#include <atomic>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
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
    if(!input_stream)
        throw std::runtime_error("file \"" + template_file + "\" does not exist");
    syntax_parser parser;
    parser.parse(input_stream);
    
    field root_field = std::move(parser.get_root_field());
    
    std::vector<std::string> undefined;
    parser.get_mapper().find_non_registered_fields(std::back_inserter(undefined));
    if(!undefined.empty()) {
        for(const auto &str : undefined)
            std::cout << "field \"" << str << "\" is not defined." << std::endl;
        throw parse_error();
    }
    
    return root_field;
}

void move_file(const std::string &from, const std::string &to) 
{
    if(rename(from.c_str(), to.c_str()) == -1) {
        std::ifstream from_file(from);
        std::ofstream to_file(to);
        if(!from_file || !to_file)
            throw std::runtime_error("moving file failed");
        to_file << from_file.rdbuf();
    }
}

void run(const std::string &template_file, const std::string &cmd, 
  const std::string &file_name, const std::string &output_base) 
{
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
                move_file(file_name, output_base + '-' + std::to_string(crashed_count));
                crashed_count++;
                if(crashed_count == 10)
                    break;
            }
        }
    }
}

void print_usage(char *name) {
    std::cout << "Usage: " << name << " [OPTIONS] COMMAND" << std::endl;
    std::cout << "\n";
    std::cout << "OPTIONS can be:" << std::endl;
    std::cout << "\t-t <TEMPLATE>" << std::endl;
    std::cout << "\t-o <OUTPUT_FILE>" << std::endl;
    std::cout << "\t-r <RESULT_BASE>" << std::endl;
}

std::string join_args(char **start, char **end) {
    std::ostringstream oss;
    while(start < end)
        oss << *start++ << " ";
    return oss.str();
}

int main(int argc, char *argv[]) {
    #ifndef WIN32
        signal(SIGINT, [](int) { 
            running = false; 
            if(serializer_ptr) 
                serializer_ptr->stop();
        });
    #endif

    if(argc < 4) {
        print_usage(*argv);
        return EXIT_FAILURE;
    }
    std::string template_file, command, output_base("/tmp/fuzzer"), file_name("/dev/shm/fuzzer");
    char **ptr = argv + 1, **end = argv + argc;
    while(ptr < end) {
        if(*ptr[0] == '-' && std::distance(ptr, end) < 2) {
            std::cout << "expected argument after \"" << *ptr << "\"\n";
            return EXIT_FAILURE;
        }
            
        if(!std::strcmp(*ptr, "-t")) {
            template_file = ptr[1];
            ptr += 2;
        }
        else if(!std::strcmp(*ptr, "-o")) {
            file_name = ptr[1];
            ptr += 2;
        }
        else if(!std::strcmp(*ptr, "-r")) {
            output_base = ptr[1];
            ptr += 2;
        }
        else {
            if(*ptr[0] != '-')
                break;
            std::cout << "invalid option \"" << *ptr << "\" encountered.\n";
            return EXIT_FAILURE;
        }
    }
    
    if(template_file.empty()) {
        std::cout << "expected a template filename\n";
        return EXIT_FAILURE;
    }
    
    if(ptr >= argv + argc) {
        print_usage(*argv);
        return EXIT_FAILURE;
    }
    
    command = join_args(ptr, argv + argc);
    
    try {
        run(template_file, command, file_name, output_base);
        std::cout << std::endl;
        std::cout << "generated enough samples, stopping..." << std::endl;
    }
    catch(std::exception &ex) {
        std::cout << ex.what() << ". aborting..." << std::endl;
        return EXIT_FAILURE;
    }
}

