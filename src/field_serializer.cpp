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
    generation_context ctx(std::time(0) ^ reinterpret_cast<time_t>(&root));
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
