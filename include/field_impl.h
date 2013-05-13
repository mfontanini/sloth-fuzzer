#ifndef FUZZER_FIELD_IMPL_H
#define FUZZER_FIELD_IMPL_H

#include <cstdint>
#include <functional>
#include <vector>
#include "utils.h"

class value_wrapper;
class field;
class field_mapper;
class generation_context;

class field_impl {
public:
    typedef uint8_t value_type;
    typedef std::function<void(const field &)> visitor_type;
    typedef unsigned int identifier_type;
    typedef std::vector<identifier_type> dependents_type;

    virtual ~field_impl() = default;
    
    virtual std::unique_ptr<field_impl> clone() const = 0;
    virtual void prepare(generation_context &) { };
    virtual void set(size_t index, value_type value) = 0;
    virtual value_type get(size_t index) const = 0;
    virtual size_t size() const = 0;
    virtual size_t field_count() const { return 1; };
    virtual void accept_visitor(const visitor_type& visitor) const { };
    virtual dependents_type dependent_fields() const { 
        return { };
    };
    virtual void set_value(int64_t value);
    virtual int64_t get_value() const;
private:

};

// crtp so clone is always defined
template<typename Concrete>
class clonable_field_impl : public field_impl {
public:
    std::unique_ptr<field_impl> clone() const 
    {
        return make_unique<Concrete>(static_cast<const Concrete&>(*this));
    }
};

#endif // FUZZER_FIELD_IMPL_H
