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

int main() {
    auto filler = std::make_shared<value_filler>("ASD-carlos-jaskldjaskl");
    auto impl = make_unique<compound_field_impl>();
    impl->add_field(field::from_impl<block_field_impl>(nullptr, 5));
    impl->add_field(field::from_impl<block_field_impl>(nullptr, 10));
    impl->add_field(field::from_impl<block_field_impl>(nullptr, 15));
    impl->add_field(field::from_impl<block_field_impl>(nullptr, 3));
    std::random_device rnd;
    field f(filler, std::move(impl));
    field f2(f);
    f.prepare(rnd);
    
    f.accept_visitor([](const field &f) { std::cout << "a: " << f.id() << std::endl; });
    std::cout << std::endl;
    
    
    std::cout << f.size() << std::endl;
    std::iota(f.begin(), f.end(), 0);
    f.fill(field_mapper());
    for(const auto &i : f)
        std::cout << i << ", ";
    std::cout << std::endl;
    
    
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

