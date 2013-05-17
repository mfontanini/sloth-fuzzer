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
 
#ifndef FUZZER_CHOICE_FIELD_H
#define FUZZER_CHOICE_FIELD_H

#include <vector>
#include "field.h"
#include "field_impl.h"

class choice_field_impl : public clonable_field_impl<choice_field_impl> {
public:
    template<typename InputIterator>
    choice_field_impl(InputIterator start, InputIterator end)
    : choices(start, end), current_choice(nullptr)
    {
        for(const auto &i : choices) {
            auto &&deps = i.dependent_fields();
            dependents.insert(
                dependents.end(),
                deps.begin(), 
                deps.end()
            );
        }
    }

    void set(size_t index, value_type value);
    value_type get(size_t index) const;
    size_t size() const;
    void prepare(generation_context &ctx);
    dependents_type dependent_fields() const;
private:
    std::vector<field> choices;
    dependents_type dependents;
    field *current_choice;
};

#endif // FUZZER_CHOICE_FIELD_H
