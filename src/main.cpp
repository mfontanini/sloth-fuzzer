#include <iostream>
#include <random>
#include <algorithm>
#include "block_field.h"
#include "compound_field.h"
#include "field.h"
#include "value_filler.h"
#include "function_nodes.h"
#include "const_value_node.h"
#include "function_value_filler.h"
#include "field_mapper.h"
#include "topological_sorter.h"
#include "functions/crc.h"
#include "function_value_filler.h"

int main() {
    auto filler = std::make_shared<value_filler>("ASD-carlos-jaskldjaskl");
    auto impl = make_unique<compound_field_impl>();
    impl->add_field(field::from_impl<block_field_impl>(nullptr, 5));
    impl->add_field(field::from_impl<block_field_impl>(nullptr, 10));
    impl->add_field(field::from_impl<block_field_impl>(nullptr, 15));
    impl->add_field(field::from_impl<block_field_impl>(
        std::make_shared<function_value_filler>(make_unique<crc32_function>(1)), 3));
    std::random_device rnd;
    field f(filler, std::move(impl));
    
    std::cout << "----\n";
    
    topological_sorter sorter;
    for(const auto &i : sorter.topological_sort(f)) {
        std::cout << "-" << i << std::endl;
    }
    
    
    /*auto filler = std::make_shared<function_value_filler>(
            make_unique<multiplies_function_node>(
            make_unique<plus_function_node>(make_unique<const_value_node>(15), make_unique<const_value_node>(5)),
            make_unique<const_value_node>(2)
        ));
    auto f = field::from_impl<block_field_impl>(filler, 4);
    std::cout << (f < f) << std::endl;
    f.fill();
    std::cout << f.size() << std::endl;
    std::cout << f.get_value() << std::endl;*/
    
}

