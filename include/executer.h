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
