#ifndef FUZZER_FIELD_FILLER_H
#define FUZZER_FIELD_FILLER_H

#include <array>
#include <memory>
#include "functions/constraints.h"
#include "field_impl.h"

class field;
class field_mapper;

class field_filler {
public:
    typedef field_impl::identifier_type identifier_type;
    typedef field_impl::dependents_type dependents_type;
    typedef std::unique_ptr<function_constraint> constraint_type;
    
    // derived classes can re-implement this, it will be called
    // using static polymorphism
    static const std::array<constraint_type, 0> &get_constraints() {
        static const std::array<constraint_type, 0> dummy;
        return dummy;
    };

    virtual ~field_filler() = default;
    virtual void fill(field &, generation_context &) = 0;
    virtual dependents_type dependent_fields() const {
        return { };
    }
};

#endif // FUZZER_FIELD_FILLER_H
