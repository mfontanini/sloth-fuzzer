#ifndef FUZZER_VALUE_FILLER_H
#define FUZZER_VALUE_FILLER_H

#include <vector>
#include <string>
#include "field_filler.h"
#include "field.h"

class value_filler : public field_filler {
public:
    template<typename ForwardIterator>
    value_filler(ForwardIterator start, ForwardIterator end)
    : data(start, end) 
    {
        
    }
    
    value_filler(const std::string &str);
    
    void fill(field &f, const field_mapper&);
private:
    std::vector<field::value_type> data;
};

#endif // FUZZER_VALUE_FILLER_H
