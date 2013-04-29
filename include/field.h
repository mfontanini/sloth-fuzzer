#ifndef FUZZER_FIELD_H
#define FUZZER_FIELD_H

#include <memory>
#include "utils.h"

class field_impl;
class value_wrapper;

class field {
public:
    typedef std::unique_ptr<field_impl> unique_impl;

    field(unique_impl impl);
    field(const field &rhs);
    field(field &&rhs) noexcept;
    
    field& operator=(field rhs);
    
    value_wrapper &value();
    const value_wrapper &value() const;
    
    // swap free function
    friend void swap(field &lhs, field &rhs);
    
    template<typename Impl, typename... Args>
    static field from_impl(Args&&... args) 
    {
        return field(make_unique<Impl>(std::forward<Args>(args)...));
    }
private:
    unique_impl impl;
};

#endif // FUZZER_FIELD_H
