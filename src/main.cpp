#include <iostream>
#include <random>
#include <algorithm>
#include <sstream>
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
#include "parser/syntax_parser.h"

void test_parse() {
    std::string input = "block<2> carlos = 4; block<4> roberto = 50 + 5 * carlos;";
    std::istringstream input_stream(input); 
    syntax_parser parser;
    parser.parse(input_stream);
    field root = parser.get_root_field();
    
    std::random_device rnd;
    std::cout << root.size() << std::endl;
    
    parser.get_mapper().identify_fields(root);
    field_mapper mapper = std::move(parser.get_mapper());
    
    std::cout << "root id: " << root.id() << std::endl;
    
    topological_sorter sorter;
    
    for(const auto &i : sorter.topological_sort(root)) {
        const_cast<field&>(mapper.find_field(i)).prepare(rnd);
        const_cast<field&>(mapper.find_field(i)).fill(mapper);
    }
    
    root.accept_visitor([&](const field &f) { 
        if(f.id() != root.id())
            std::cout << f.id() << " - " << f.get_value() << std::endl; 
    });
}

int main() {
    test_parse();
    /*auto filler = std::make_shared<value_filler>("ASD-carlos-jaskldjaskl");
    auto impl = make_unique<compound_field_impl>();
    impl->add_field(field::from_impl<block_field_impl>(
        std::make_shared<function_value_filler>(make_unique<crc32_function>(1)), 3));
    impl->add_field(field::from_impl<block_field_impl>(
        std::make_shared<function_value_filler>(make_unique<crc32_function>(2)), 5));
    impl->add_field(field::from_impl<block_field_impl>(nullptr, 10));
    impl->add_field(field::from_impl<block_field_impl>(nullptr, 15));
    std::random_device rnd;
    field f(filler, std::move(impl));
    
    topological_sorter sorter;
    for(const auto &i : sorter.topological_sort(f)) {
        std::cout << "-" << i << std::endl;
    }
    
    */
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

