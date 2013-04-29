#ifndef FUZZER_VALUE_WRAPPER_IMPL_H
#define FUZZER_VALUE_WRAPPER_IMPL_H

#include <memory>
#include <cstdint>
#include <cstddef>
#include "utils.h"

class value_wrapper_impl {
public:
    typedef uint8_t value_type;

    virtual std::unique_ptr<value_wrapper_impl> clone() const = 0;
    
    virtual void set(size_t index, value_type value) = 0;
    virtual value_type get(size_t index) const = 0;
    virtual size_t size() const = 0;
private:
    
};

// crtp so clone is always defined
template<typename Concrete>
class clonable_value_wrapper_impl : public value_wrapper_impl {
public:
    std::unique_ptr<value_wrapper_impl> clone() const {
        return make_unique<Concrete>(static_cast<const Concrete&>(*this));
    }
};

#endif // FUZZER_VALUE_WRAPPER_IMPL_H
