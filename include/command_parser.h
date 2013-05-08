#ifndef FUZZER_COMMAND_PARSER_H
#define FUZZER_COMMAND_PARSER_H

#include <vector>
#include <string>
#include <tuple>

class command_parser {
public:
    typedef std::vector<std::string> arguments_type;
    
    command_parser(const std::string &cmd);
    
    std::tuple<std::string, arguments_type> generate_template(const std::string &file_name) const;
private:
    std::tuple<std::string, arguments_type, size_t> split_cmd(const std::string &cmd);

    std::string application;
    arguments_type arguments;
    size_t replacement_index;
};

#endif // FUZZER_COMMAND_PARSER_H
