#ifndef FUZZER_EXECUTER_H
#define FUZZER_EXECUTER_H

#include <string>
#include "command_parser.h"

class field;

class executer {
public:
    enum class execution_status {
        success,
        failed,
        killed_by_signal
    };
    executer(const std::string &cmd);
    
    execution_status execute(const field &f, const std::string &output_file);
private:
    execution_status do_execute(const std::string &file);
    
    command_parser cmd_template;
};

#endif // FUZZER_EXECUTER_H
