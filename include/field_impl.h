#ifndef FUZZER_FIELD_IMPL_H
#define FUZZER_FIELD_IMPL_H

#include "utils.h"

class value_wrapper;

class field_impl {
public:
    virtual std::unique_ptr<field_impl> clone() const = 0;
    virtual void prepare() { };
    virtual value_wrapper &value() = 0;
    //virtual void prepare() = 0;
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
