#ifndef FUZZER_SYNTAX_H
#define FUZZER_SYNTAX_H

#include <map>
#include <string>

enum fuzzer_identifiers {
    TEMPLATE,
    IDENTIFIER,
    INT_CONST,
    TEMPLATES,
    BLOCK
};

/*class symbol_table {
public:
    unsigned add_string(const std::string& str) {
        auto iter = symbols.find(str);
        if(iter == symbols.end())
            iter = symbols.insert({ str, symbols.size() }).first;
        return iter->second;
    }
private:
    std::map<std::string, unsigned> symbols;
};*/

#endif // FUZZER_SYNTAX_H
