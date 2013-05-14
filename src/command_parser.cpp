#include <sstream>
#include <limits>
#include <stdexcept>
#include "command_parser.h"



command_parser::command_parser(const std::string &input)
{
    std::tie(application, arguments, replacement_index) = split_cmd(input);
}

auto command_parser::generate_template(const std::string &file_name) const 
  -> std::tuple<std::string, arguments_type>
{
    arguments_type args = arguments;
    args[replacement_index] = file_name;
    return std::make_tuple(application, args);
}

auto command_parser::split_cmd(const std::string &cmd) -> std::tuple<std::string, arguments_type, size_t>
{
    std::istringstream input(cmd);
    arguments_type args;
    std::string function, part;
    size_t index = std::numeric_limits<size_t>::max();
    if(!std::getline(input, function, ' '))
        throw std::runtime_error("no command given");
    while(std::getline(input, part, ' ')) {
        if(part == "{%}")
            index = args.size();
        args.push_back(part);
    }
    if(index == std::numeric_limits<size_t>::max()) {
        args.push_back({});
        index = args.size() - 1;
    }
    return std::make_tuple(std::move(function), std::move(args) , index);
}
