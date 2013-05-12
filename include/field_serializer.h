#ifndef FUZZER_FIELD_SERIALIZER_H
#define FUZZER_FIELD_SERIALIZER_H

#include <queue>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <vector>
#include <thread>
#include "field.h"

class field_serializer {
public:
    typedef std::vector<uint8_t> serialization_type;

    field_serializer(field root, size_t num_threads, size_t max_fields = 10);
    ~field_serializer();
    
    void stop();
    serialization_type next_field();
private:
    field_serializer(const field_serializer&) = delete;
    field_serializer& operator=(const field_serializer&) = delete;

    void thread_proc(field root);
    void store_field(serialization_type f);

    std::queue<serialization_type> result_queue;
    std::vector<std::thread> threads;
    std::mutex queue_lock;
    std::condition_variable push_cond, pop_cond;
    size_t max_fields;
    std::atomic<bool> running;
};

#endif // FUZZER_FIELD_SERIALIZER_H
