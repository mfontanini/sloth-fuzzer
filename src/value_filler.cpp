#ifdef FUZZER_DEBUG
#include <cassert>    
#endif
#include "value_filler.h"

value_filler::value_filler(const std::string &str)
: data(str.begin(), str.end())
{
    
}

void value_filler::fill(field &f, const field_mapper&) 
{
    std::copy(data.begin(), data.end(), f.begin());
}
