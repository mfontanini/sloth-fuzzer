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
