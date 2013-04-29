#include <iostream>
#include <algorithm>
#include "block_field.h"
#include "field.h"

int main() {
    field f = field::from_impl<block_field_impl>(15);
    std::cout << f.value().size() << std::endl;
    std::iota(f.value().begin(), f.value().end(), 0);
    for(const auto &i : f.value())
        std::cout << static_cast<int>(i) << ", ";
    std::cout << std::endl;
}

