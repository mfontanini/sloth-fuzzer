#ifndef FUZZER_SYNTAX_H
#define FUZZER_SYNTAX_H

#include <map>
#include <string>

#ifndef YYLTYPE_IS_DECLARED
#define YYLTYPE_IS_DECLARED

struct YYLTYPE  
{  
  int first_line;  
  int first_column;  
  int last_line;  
  int last_column;  
};

#endif

#ifndef YYTOKENTYPE
# define YYTOKENTYPE

enum fuzzer_identifiers {
    TEMPLATE = 258,
    IDENTIFIER,
    INT_CONST,
    TEMPLATES,
    BLOCK
};
#endif

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
