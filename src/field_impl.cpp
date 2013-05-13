#include <cstdint>
#include "field_impl.h"

void field_impl::set_value(int64_t value)
{
    int index = size() - 1;
    while(index >= 0 && value != 0) {
        set(index--, static_cast<uint8_t>(value) & 0xff);
        value >>= 8;
    }
}

int64_t field_impl::get_value() const
{
    int64_t val{};
    int end = size() - 1, index = end;
    while(index >= 0 && static_cast<size_t>(end - index) < sizeof(int64_t)) {
        val = val | (static_cast<uint64_t>(get(index)) << ((end - index) * 8));
        index--;
    }
    return val;
}
