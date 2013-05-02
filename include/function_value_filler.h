#ifndef FUZZER_FUNCTION_VALUE_FILLER_H
#define FUZZER_FUNCTION_VALUE_FILLER_H

#include <memory>
#include "value_node.h"
#include "field_filler.h"

class function_value_filler : public field_filler {
public:
    typedef std::unique_ptr<value_node> unique_value;

    function_value_filler(unique_value value);

    void fill(field &f, const field_mapper &mapper);
    dependents_type dependent_fields() const;
private:
     unique_value value;
};

#endif // FUZZER_FUNCTION_VALUE_FILLER_H
