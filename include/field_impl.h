#ifndef FUZZER_FIELD_IMPL_H
#define FUZZER_FIELD_IMPL_H

#include <cstdint>
#include <random>
#include <functional>
#include "utils.h"

class value_wrapper;
class field;

class field_impl {
public:
    typedef uint8_t value_type;
    typedef std::random_device random_generator;
    typedef std::function<void(const field &)> visitor_type;

    virtual ~field_impl() = default;
    
    virtual std::unique_ptr<field_impl> clone() const = 0;
    virtual void prepare(random_generator &engine) { };
    virtual void set(size_t index, value_type value) = 0;
    virtual value_type get(size_t index) const = 0;
    virtual size_t size() const = 0;
    virtual void accept_visitor(const visitor_type& visitor) const { };
private:

};

// crtp so clone is always defined
template<typename Concrete>
class clonable_field_impl : public field_impl {
public:
    std::unique_ptr<field_impl> clone() const {
        return make_unique<Concrete>(static_cast<const Concrete&>(*this));
    }
};

#endif // FUZZER_FIELD_IMPL_H
