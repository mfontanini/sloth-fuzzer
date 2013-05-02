#ifndef FUZZER_FIELD_FILLER_H
#define FUZZER_FIELD_FILLER_H

#include "field_impl.h"

class field;
class field_mapper;

class field_filler {
public:
    typedef field_impl::identifier_type identifier_type;
    typedef field_impl::dependents_type dependents_type;

    virtual ~field_filler() = default;
    virtual void fill(field &, const field_mapper&) = 0;
    virtual dependents_type dependent_fields() const {
        return { };
    }
};

#endif // FUZZER_FIELD_FILLER_H
