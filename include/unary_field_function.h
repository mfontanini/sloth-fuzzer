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

#ifndef FUZZER_UNARY_FIELD_FUNCTION_H
#define FUZZER_UNARY_FIELD_FUNCTION_H

#include "field.h"
#include "value_node.h"
#include "generation_context.h"

template<typename Concrete>
class unary_field_function : public value_node {
public:
    unary_field_function(identifier_type id)
    : id(id)
    {
        
    }
    
    double eval(generation_context &ctx)
    {
        return static_cast<Concrete&>(*this).apply(ctx.get_mapper().find_field(id));
    }

    dependents_type dependent_fields() const
    {
        return { id };
    }
private:
    // The dispatched method should follow this signature:
    //double apply(const field &input_field);

    identifier_type id;
};

template<typename Concrete>
class unary_field_filler_function : public field_filler {
public:
    unary_field_filler_function(identifier_type id)
    : id(id)
    {
        
    }
    void fill(field &f, generation_context &ctx)
    {
        static_cast<Concrete&>(*this).apply(ctx.get_mapper().find_field(id), f);
    }

    dependents_type dependent_fields() const
    {
        return { id };
    }
private:
    // The dispatched method should follow this signature:
    // void apply(const field &input_field, field &output_field);

    identifier_type id;
};

#endif // FUZZER_UNARY_FIELD_FUNCTION_H
