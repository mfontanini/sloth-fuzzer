#ifndef FUZZER_EXECUTER_H
#define FUZZER_EXECUTER_H

#include <string>
#include <algorithm>
#include <fstream>
#include <cstdint>
#include "command_parser.h"
#include "exceptions.h"

class field;

class executer {
public:
    enum class exec_status {
        success,
        failed,
        killed_by_signal
    };
    executer(const std::string &cmd);
    
    template<typename ForwardIterator>
    exec_status execute(ForwardIterator start, ForwardIterator end, const std::string &output_file)
    {
        std::ofstream output(output_file, std::ios::binary);
        if(!output)
            throw file_open_exception();
        std::for_each(start, end, [&](uint8_t i) { output << i; });
        output.close();
        return do_execute(output_file);
    }
private:
    exec_status do_execute(const std::string &file);
    
    command_parser cmd_template;
};

#endif // FUZZER_EXECUTER_H
