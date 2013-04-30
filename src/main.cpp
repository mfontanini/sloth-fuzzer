#include <iostream>
#include <random>
#include <algorithm>
#include "block_field.h"
#include "compound_field.h"
#include "field.h"
#include "value_filler.h"
#include "function_nodes.h"
#include "const_value_node.h"

int main() {
    auto filler = std::make_shared<value_filler>("ASD-carlos-jaskldjaskl");
    auto impl = make_unique<compound_field_impl>();
    impl->add_field(field::from_impl<block_field_impl>(nullptr, 5));
    impl->add_field(field::from_impl<block_field_impl>(nullptr, 10));
    impl->add_field(field::from_impl<block_field_impl>(nullptr, 15));
    impl->add_field(field::from_impl<block_field_impl>(nullptr, 3));
    std::random_device rnd;
    field f(filler, std::move(impl));
    f.prepare(rnd);
    std::cout << f.size() << std::endl;
    std::iota(f.begin(), f.end(), 0);
    f.fill();
    for(const auto &i : f)
        std::cout << i << ", ";
    std::cout << std::endl;
    
    std::cout << "Data: " << 
        multiplies_function_node(
            make_unique<plus_function_node>(make_unique<const_value_node>(15), make_unique<const_value_node>(5)),
            make_unique<const_value_node>(2)
        ).eval() << std::endl;
}

