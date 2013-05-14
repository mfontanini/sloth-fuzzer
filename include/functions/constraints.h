#ifndef FUZZER_FUNCTION_CONSTRAINT_H
#define FUZZER_FUNCTION_CONSTRAINT_H

#include <cstddef>

namespace grammar {
class field_node;
}

class function_constraint {
public:
    virtual ~function_constraint() = default;
    
    virtual void check(const grammar::field_node &f) = 0;
};

class required_size_constraint : public function_constraint {
public:
    required_size_constraint(size_t required);
    void check(const grammar::field_node &f);
private:
    size_t required;
};

#endif // FUZZER_FUNCTION_CONSTRAINT_H
