#include <iostream>
#include <istream>
#include <vector>
#include <string>
#include <cctype>
#include <map>
#include <iterator>
#include <algorithm>
#include <stack>
#include "lexer.h"
#include "parser.h"

using namespace std;
LexicalAnalyzer lexer;
Token t;
vector<string> scope;
map<string, string> value;
map<string, string> varScope;
map<string, string> privVar;
map<string, string> pubVar;
map<string, int> scope_pub_var;
map<string, int> scope_priv_var;
map<string, bool> scope_pubs;
map<string, bool> scope_privs;
vector<string> pub_order;
vector<string> pub_scope;
vector<string> priv_order;
vector<string> priv_scope;
vector<string> ans;
int pub_counter = 0;
int priv_counter = 0;
bool global = false;
bool pub;
bool priv;

  void parser::syntax_error(){// function to print out the syntax error message
  cout <<"Syntax Error\n";
  exit(0);
}

  void parser::parse_program(){// initates the parser
  t = lexer.GetToken();
  Token t1 = lexer.GetToken();
  bool ambig = false;// used this to know if it alreay went to parse_scope or not
  if(t1.token_type != LBRAC){// uses peeking to go around the ambiguity problem
    lexer.UngetToken(t1);// need to unget all tokens in reverse order of what was gatheres
    lexer.UngetToken(t);
    scope.push_back("global");
    parse_global_vars();
    }
  else{// if there is no LBRACE goes straigt to here
    ambig = true;
    lexer.UngetToken(t1);
    lexer.UngetToken(t);
    parse_scope();
  }
  if(!ambig){// only enters if gloval vars was epsilong
    parse_scope();
  }
  Token t2 = lexer.GetToken();
  Token t3 = lexer.GetToken();
  if(t2.token_type != END_OF_FILE){
    syntax_error();
  }
  for(int i = 0; i <= ans.size()-1; i++){
    cout<<ans[i];
  }
  }

  void parser::parse_global_vars(){// parses through the global variabls
    t = lexer.GetToken();
    if(t.token_type == ID){
      lexer.UngetToken(t);
      global = true;
      parse_var_list();
      t = lexer.GetToken();
      if(t.token_type != SEMICOLON){// checks if there is a semicolon after paprsing all the global variables
        syntax_error();
      }
    }
    else{// if ther is no initial ID it will give synatax error, but will most likel not ever enter here.
      lexer.UngetToken(t);
      syntax_error();
    }

  }

  string parser::figure_current_scope(){
    return scope.back();
  }

  void parser::parse_var_list(){// parses through all the variables,
    t = lexer.GetToken();
    if(t.token_type == ID){
      Token t1 = lexer.GetToken();
      if(global){
        varScope.insert(std::pair<string, string> (t.lexeme, "::"));
      }
      else{
        if(pub){
          pub_order.push_back(t.lexeme);
          pub_scope.push_back(figure_current_scope());
          pub_counter++;
          scope_pubs[figure_current_scope()] = true;
          scope_pub_var[figure_current_scope()] = scope_pub_var[figure_current_scope()]+1;
          pubVar.insert(std::pair<string, string> (t.lexeme, (figure_current_scope())  ));
        }
        if(priv){
          priv_order.push_back(t.lexeme);
          priv_scope.push_back(figure_current_scope());
          priv_counter++;
          scope_privs[figure_current_scope()] = true;
          scope_priv_var[figure_current_scope()] = scope_priv_var[figure_current_scope()]+1;
          privVar.insert(std::pair<string, string> (t.lexeme, (figure_current_scope())));
        }
      }
      if(t1.token_type == COMMA){// checks if there will be more variables to parse
        parse_var_list();
      }
      else{
        lexer.UngetToken(t1);
      }
    }
    else{// gives a syntax error if there is not intial ID, but it will most likely never enter here either
      lexer.UngetToken(t);
      syntax_error();
    }
    global = false;
  }

  void parser::parse_scope(){// function that parses throught the scope
    t = lexer.GetToken();
    if(t.token_type == ID){// needs to have an ID at the begining of parsing the scope
      Token t1 = lexer.GetToken();
      if(t1.token_type == LBRAC){// needs to havea LBRACE after an initial ID
        scope_pub_var.insert(std::pair<string, int> (t.lexeme, 0));
        scope_priv_var.insert(std::pair<string, int> (t.lexeme, 0));
        scope.push_back(t.lexeme);
        Token t2 = lexer.GetToken();
                                  // ambiguity starts here
        if(t2.token_type == PUBLIC){// if there is a public statemt to parse it enters here
          lexer.UngetToken(t2);
          pub = true;
          parse_public_vars();
          pub = false;
          Token t3 = lexer.GetToken();
          if(t3.token_type == PRIVATE){
            lexer.UngetToken(t3);
            priv = true;
            parse_private_vars();
            priv = false;
            parse_stmt_list();
          }
          else if(t3.token_type == ID){
            lexer.UngetToken(t3);
            parse_stmt_list();
          }
          else{
            lexer.UngetToken(t3);
            syntax_error();
          }
        }
        else if(t2.token_type == PRIVATE){// if there is a private statemtn to parser it enters here
          lexer.UngetToken(t2);
          priv = true;
          parse_private_vars();
          priv = false;
          parse_stmt_list();
        }
        else if(t2.token_type == ID){// if there is no public or private statments it continues to parse the statment list
          lexer.UngetToken(t2);
          parse_stmt_list();
        }
        else{// there has to be atleast a stmt list so if no it will call syntax error
          lexer.UngetToken(t2);
          lexer.UngetToken(t1);
          lexer.UngetToken(t);
          syntax_error();
        }
        Token t3 = lexer.GetToken();
        if(t3.token_type != RBRAC){
          syntax_error();
        }

        if(scope_pubs[figure_current_scope()]){
          int pub_size = ((pub_order.size())-scope_pub_var[figure_current_scope()]);
          pub_order.erase(pub_order.begin() + pub_size, pub_order.end());
          pub_scope.erase(pub_scope.begin() + pub_size, pub_scope.end());
        }
        if(scope_privs[figure_current_scope()]){
          int priv_size = ((priv_order.size())-scope_priv_var[figure_current_scope()]);
          priv_order.erase(priv_order.begin() + priv_size, priv_order.end());
          priv_scope.erase(priv_scope.begin() + priv_size, priv_scope.end());
        }
        scope.pop_back();

      }
      else{// calls syntax error if ther is no LBRACE after the inital ID
        lexer.UngetToken(t1);
        lexer.UngetToken(t);
        syntax_error();
      }
    }
    else{// calls syntax error if ther is no inital ID
      lexer.UngetToken(t);
      syntax_error();
    }
  }

  void parser::parse_public_vars(){// parses through the public section of the secion list
    t = lexer.GetToken();
    if(t.token_type == PUBLIC){
      Token t1 = lexer.GetToken();
      if(t1.token_type == COLON){
        parse_var_list();
        Token t2 = lexer.GetToken();
        if(t2.token_type != SEMICOLON){
          lexer.UngetToken(t2);
          lexer.UngetToken(t1);
          lexer.UngetToken(t);
          syntax_error();
        }
      }
      else{// if the public decleration is not followed by a colon clls syntax error
        lexer.UngetToken(t1);
        lexer.UngetToken(t);
        syntax_error();
      }
    }
    else{// if there is nn public decleration then it will call syntax error, might not reach here
      lexer.UngetToken(t);
      syntax_error();
    }

  }

  void parser::parse_private_vars(){// parse the private section
    t = lexer.GetToken();
    if(t.token_type == PRIVATE){
      Token t1 = lexer.GetToken();
      if(t1.token_type == COLON){
        parse_var_list();
        Token t2 = lexer.GetToken();
        if(t2.token_type != SEMICOLON){// if there is no SEMICOLON at the end it will call syntax error
          lexer.UngetToken(t2);
          lexer.UngetToken(t1);
          lexer.UngetToken(t);
          syntax_error();
        }
      }
      else{// needs to be followed by a colon so it will call syntax error if there is none
        lexer.UngetToken(t1);
        lexer.UngetToken(t);
        syntax_error();
      }
    }
    else{// needs to start with a provate decleration so it will call syntax error if there is none
      lexer.UngetToken(t);
      syntax_error();
    }
  }

  void parser::parse_stmt_list(){// a list of stmt's
    parse_stmt();// immeditately calls parse_stmt because it could only start off with a stmt
    t = lexer.GetToken();
    if(t.token_type == ID){// checks if it needs to call itself again
      lexer.UngetToken(t);
      parse_stmt_list();
    }
    else{
      lexer.UngetToken(t);
    }
  }

  string parser::figure_scope(string index){
    map<string,string>::iterator it = varScope.begin();
    map<string, string>::iterator it1 = privVar.begin();
    map<string, string>::iterator it2 = pubVar.end();
    string final;
    string priv_current_scope;
    bool found = false;
    bool stop = true;
    bool priv_stop= true;
    bool priv_found = false;
    while(it != varScope.end()){
      if(it->first == index){
        found = true;
        if(it->second == "::"){
          final = (it->second + it->first);
        }
        else{
          final = (it->second + "." + it->first);
        }
      }
      it++;
    }
    for(int i = priv_order.size()-1; i>= 0; i--){
      if(priv_order[i] == index && priv_stop){
        if(priv_scope[i] == figure_current_scope()){
          found = true;
          priv_stop = false;
          priv_found = true;
          final = (priv_scope[i] + "." + priv_order[i]);
        }
      }
    }
    /*while(it1 != privVar.end()){
      if(it1->first == index){
        priv_current_scope = figure_current_scope();
        if(it1->second == priv_current_scope){
          found = true;
          final = (it1->second + "." + it1->first);
        }
        else{
          it2 = pubVar.begin();
          while(it2 != pubVar.end()){
            if(it2->first == index){
              found = true;
              final = (it2->second + "." + it2->first);
            }
            it2++;
          }
          if(!found){
            final = ("?." + it1->first);
          }
        }
      }
      it1++;
    }*/
    for(int i = pub_order.size()-1; i>= 0; i--){
      if(pub_order[i] == index && stop){
        if(!priv_found){
        found = true;
        stop = false;
        final = (pub_scope[i] + "." + pub_order[i]);
      }
      }
    }
  /*  while(it2 != pubVar.begin()){
      if(it2->first == index){
        found = true;
        final = (it2->second + "." + it2->first);
      }
      it2--;
    }*/

    if(!found){
      final = ("?." + index);
    }
    return final;
  }

  string parser::figure_ans(string one, string two){
    string lhs;
    string rhs;
    string eq = (" = ");
    string final;
    lhs = figure_scope(one);
    rhs = figure_scope(two);
    final = (lhs + eq + rhs + "\n");
    ans.push_back(final);
  }

  void parser::parse_stmt(){// parse the stmt section
    t = lexer.GetToken();
    Token t1 = lexer.GetToken();                          // ambiguity starts here
    if(t1.token_type == EQUAL){// peeks ahead to solve the ambiguity problem
      Token t2 = lexer.GetToken();
      if(t2.token_type == ID){
        Token t3 = lexer.GetToken();
        if(t3.token_type != SEMICOLON){// needs to have a SEMICOLON at the end of the stmt otherwise throws syntax error
          lexer.UngetToken(t3);
          lexer.UngetToken(t2);
          lexer.UngetToken(t1);
          lexer.UngetToken(t);
          syntax_error();
        }
        else{
          figure_ans(t.lexeme, t2.lexeme);
          return;
        }
      }
      else{// at this point after an EQUAL there has to be another ID otherwise throws syntax error
        lexer.UngetToken(t2);
        lexer.UngetToken(t1);
        lexer.UngetToken(t);
        syntax_error();
      }
    }
    else if(t1.token_type == LBRAC){
      lexer.UngetToken(t1);
      lexer.UngetToken(t);
      parse_scope();
    }
    else{
      syntax_error();
    }
    }

  int main(){
    parser parse;
    parse.parse_program();

  }
