#ifndef FUZZER_FUNCTION_CONSTRAINT_H
#define FUZZER_FUNCTION_CONSTRAINT_H

#include <cstddef>
#include <memory>

namespace grammar {
class field_node;
}

class field_impl;

class function_constraint {
public:
    virtual ~function_constraint() = default;
    
    virtual void check(const grammar::field_node &f) const = 0;
    virtual std::unique_ptr<field_impl> field_impl_from_constraint() const; 
};

class required_size_constraint : public function_constraint {
public:
    required_size_constraint(size_t required);
    void check(const grammar::field_node &f) const;
    std::unique_ptr<field_impl> field_impl_from_constraint() const;
private:
    size_t required;
};

#endif // FUZZER_FUNCTION_CONSTRAINT_H
