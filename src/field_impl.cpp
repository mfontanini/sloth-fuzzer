#include <cstdint>
#include "field_impl.h"

void field_impl::set_value(double value)
{
    uint64_t int_val = static_cast<uint64_t>(value);
    int index = size() - 1;
    while(index >= 0 && int_val != 0) {
        set(index--, static_cast<uint8_t>(int_val) & 0xff);
        int_val >>= 8;
    }
}

double field_impl::get_value() const
{
    uint64_t val{};
    int end = size() - 1, index = end;
    while(index >= 0 && static_cast<size_t>(end - index) < sizeof(uint64_t)) {
        val = val | (get(index) << (end - index));
        index--;
    }
    return val;
}
