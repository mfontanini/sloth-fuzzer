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

#include <ctime>
#include "field_serializer.h"
#include "topological_sorter.h"
#include "generation_context.h"

field_serializer::field_serializer(field root, size_t num_threads, size_t max_fields)
: max_fields(max_fields), running(true)
{
    for(auto i = size_t(); i < num_threads; ++i)
        threads.emplace_back(&field_serializer::thread_proc, this, root);
}

field_serializer::~field_serializer()
{
    if(running)
        stop();
    for(auto &t : threads)
        t.join();
}

void field_serializer::stop()
{
    running = false;
    pop_cond.notify_all();
    push_cond.notify_all();
}

auto field_serializer::next_field() -> serialization_type
{
    std::unique_lock<std::mutex> locker(queue_lock);
    if(result_queue.empty()) {
        push_cond.wait(locker);
        // woken up by stop()
        if(!running)
            return serialization_type();
    }
    auto result = std::move(result_queue.front());
    result_queue.pop();
    pop_cond.notify_one();
    return result;
}

void field_serializer::store_field(serialization_type f)
{
    std::unique_lock<std::mutex> locker(queue_lock);
    if(result_queue.size() >= max_fields) {
        pop_cond.wait(locker);
        if(!running)
            return;
    }
    result_queue.push(std::move(f));
    push_cond.notify_one();
}

void field_serializer::thread_proc(field root)
{
    // some seed plx
    generation_context ctx{std::random_device()()};
    topological_sorter sorter;
    auto ordered = sorter.topological_sort(root);
    ctx.get_mapper().identify_fields(root);
    while(running) {
        for(const auto &i : ordered) {
            const_cast<field&>(ctx.get_mapper().find_field(i)).prepare(ctx);
            const_cast<field&>(ctx.get_mapper().find_field(i)).fill(ctx);
        }
        store_field(serialization_type(root.begin(), root.end()));
    }
}
