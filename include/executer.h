#ifndef FUZZER_EXECUTER_H
#define FUZZER_EXECUTER_H

#include <string>
#include "command_parser.h"

class field;

class executer {
public:
    enum class exec_status {
        success,
        failed,
        killed_by_signal
    };
    executer(const std::string &cmd);
    
    exec_status execute(const field &f, const std::string &output_file);
private:
    exec_status do_execute(const std::string &file);
    
    command_parser cmd_template;
};

#endif // FUZZER_EXECUTER_H
