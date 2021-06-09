#ifndef _PARSER_H_
#define _PARSER_H_
#include <string>

class parser{

  public:
    void syntax_error();
    void parse_program();
    void parse_global_vars();
    void parse_scope();
    void parse_var_list();
    void parse_public_vars();
    void parse_private_vars();
    void parse_stmt_list();
    void parse_stmt();
    std::string figure_current_scope();
    std::string figure_ans(std::string one, std::string two);
    std::string figure_scope(std::string index);
};
#endif
