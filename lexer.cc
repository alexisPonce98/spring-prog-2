/*
 * Copyright (C) Rida Bazzi, 2016
 *
 * Do not share this file with anyone
 */
#include <iostream>
#include <istream>
#include <vector>
#include <string>
#include <cctype>

#include "lexer.h"
#include "inputbuf.h"
void fidure();
using namespace std;

string reserved[] = { "END_OF_FILE",
    "IF", "WHILE", "DO", "THEN", "PRINT",
    "PLUS", "MINUS", "DIV", "MULT",
    "EQUAL", "COLON", "COMMA", "SEMICOLON",
    "LBRAC", "RBRAC", "LPAREN", "RPAREN",
    "NOTEQUAL", "GREATER", "LESS", "LTEQ", "GTEQ",
    "DOT", "NUM", "ID", "ERROR", "REALNUM", "PUBLIC", "PRIVATE" // TODO: Add labels for new token types here (as string)
};

#define KEYWORDS_COUNT 5
string keyword[] = { "IF", "WHILE", "DO", "THEN", "PRINT" };

void Token::Print()
{
    cout << "{" << this->lexeme << " , "
         << reserved[(int) this->token_type] << " , "
         << this->line_no << "}\n";
}

LexicalAnalyzer::LexicalAnalyzer()
{
    this->line_no = 1;
    tmp.lexeme = "";
    tmp.line_no = 1;
    tmp.token_type = ERROR;
}

void LexicalAnalyzer::skipComment(){
  char c;
  bool comment_encountered = false;
  char tmp;
  input.GetChar(c);
  if(!input.EndOfInput()){
    if(c== '/'){
      input.GetChar(tmp);
      if(tmp=='/'){
        while(tmp != '\n'){
          input.GetChar(tmp);
        }
        return;
      }
      else{
        input.UngetChar(tmp);
        input.UngetChar(c);
      }
    }
    else{
      input.UngetChar(c);
    }
  }
}

bool LexicalAnalyzer::SkipSpace()
{
    char c;
    bool space_encountered = false;

    input.GetChar(c);
    line_no += (c == '\n');

    while (!input.EndOfInput() && isspace(c)) {
        space_encountered = true;
        input.GetChar(c);
        line_no += (c == '\n');
    }

    if (!input.EndOfInput()) {
        input.UngetChar(c);
    }
    return space_encountered;
}

bool LexicalAnalyzer::IsKeyword(string s)
{
    for (int i = 0; i < KEYWORDS_COUNT; i++) {
        if (s == keyword[i]) {
            return true;
        }
    }
    return false;
}

TokenType LexicalAnalyzer::FindKeywordIndex(string s)
{
    for (int i = 0; i < KEYWORDS_COUNT; i++) {
        if (s == keyword[i]) {
            return (TokenType) (i + 1);
        }
    }
    return ERROR;
}

Token LexicalAnalyzer::ScanNumber()
{
    char c;
//bool flag_8 = false;
    input.GetChar(c);
    if (isdigit(c)) {
        if (c == '0') {
            tmp.lexeme = "0";
        } else {
            tmp.lexeme = "";
            while (!input.EndOfInput() && isdigit(c)) {
            /*  if( c== '0' || c== '9')
              {
                flag_8 = true;
              }*/
                tmp.lexeme += c;
                input.GetChar(c);
              }
            if (!input.EndOfInput()) {
                input.UngetChar(c);
            }
      }


        /*Longest matching rule goes here
         math = {0,1,2,3}
                Num, RealNum, BASE08NUM, BASE16NUM
         potential match = Arrary of size 4 initialized to zero and changed to 1 if potentiall matching
         longest mathcin length = the length of the longest potential length
         if there is not match and no potential match then the code will break and
          the longest mathcning length will restore the lenght of the lexeme*/
          /*
        int matching;
        //int[] potential = {0,0,0,0};
        int longest_match = 0;
        int length_longest_match;
        int used_lex = 0;
        int grabbed = 0;
        char let_grabbed[100];
        bool zero = true;
        if(c != '0'){
        input.GetChar(c);
        let_grabbed[grabbed] = c;
        grabbed++;
        }
        if(c ==  '0'){
          input.GetChar(c);
          let_grabbed[grabbed] = c;
          grabbed++;
          while(zero){
          if(c == '.'){
            tmp.lexeme += c;
            used_lex++;
            input.GetChar(c);
            let_grabbed[grabbed] = c;
            grabbed++;
            bool good;

            if(c != '0' || good == true){
            if(isdigit(c)){
            //  tmp.lexeme += c;
              //used_lex++;

              while(!input.EndOfInput() && isdigit(c)){
                tmp.lexeme += c;
                used_lex++;
                input.GetChar(c);
                let_grabbed[grabbed] = c;

              }
              if(!input.EndOfInput()){
                input.UngetChar(c);
              }
              tmp.token_type = REALNUM;
              tmp.line_no = line_no;
              return tmp;
            }
            else{
              zero = false;
              for(int i = grabbed-1; i>= 0; i--){
                input.UngetChar(let_grabbed[i]);
              }
              for(int i = 0; i < used_lex; i++){
                tmp.lexeme.pop_back();
              }
            }
            }
            else{
              if(c == '0'){
              }
              zero = false;
              for(int i = grabbed-1; i>= 0; i--){
                input.UngetChar(let_grabbed[i]);
              }
              for(int i = 0; i < used_lex; i++){
                tmp.lexeme.pop_back();
              }
            }
            }
            else if(c == 'x'){
              tmp.lexeme += c;
              used_lex++;
              input.GetChar(c);
              let_grabbed[grabbed] = c;
              grabbed++;
              if(c == '0'){
                tmp.lexeme += c;
                used_lex++;
                input.GetChar(c);
                let_grabbed[grabbed] = c;
                if(c == '8'){
                  if(let_grabbed[0] != '0'){
                    if(let_grabbed[0] != '9'){
                      if(let_grabbed[0] != '8'){
                    for(int i = 0; i < grabbed; i++){
                      if(let_grabbed[i] == '0'|| let_grabbed[i] == '1' || let_grabbed[i] == '2' || let_grabbed[i] == '3'
                          || let_grabbed[i] == '4' || let_grabbed[i] == '5' || let_grabbed[i] == '6' || let_grabbed[i] == '7'
                          || let_grabbed[i] == '8' || let_grabbed[i] == '9' || let_grabbed[i] == 'x'){
                            flag_8 = false;
                          }
                          else{
                            flag_8 = true;
                          }
                      }
                    }
                    else{
                      flag_8 = true;
                    }
                    }
                    else{
                      flag_8 = true;
                    }
                    }
                    else{
                      flag_8 = true;
                    }
                  if(!flag_8){
                  tmp.lexeme += c;
                  tmp.token_type = BASE08NUM;
                  tmp.line_no = line_no;
                  flag_8 = false;
                  return tmp;
                  }
                  else{
                    for(int i = grabbed; i>= 0; i--){
                      input.UngetChar(let_grabbed[i]);
                    }
                    for(int i = 0; i < used_lex; i++){
                      tmp.lexeme.pop_back();
                    }
                  }
                }
              }
              else if(c == '1'){
                tmp.lexeme += c;
                used_lex++;
                input.GetChar(c);
                let_grabbed[grabbed] = c;
                grabbed++;
                if(c == '6')
                  tmp.lexeme += c;
                  tmp.token_type = BASE16NUM;
                  tmp.line_no = line_no;
                  return tmp;
              }
              else{
                zero = false;
                for(int i = grabbed-1; i >= 0; i--){
                input.UngetChar(let_grabbed[i]);
              }
              for(int i = 0; i <used_lex; i++){
                tmp.lexeme.pop_back();
              }
              }
            }
            else{
              zero = false;
              for(int i = grabbed-1; i >= 0; i--){
              input.UngetChar(let_grabbed[i]);
            }
            for(int i = 0; i <used_lex; i++){
              tmp.lexeme.pop_back();
            }
              tmp.token_type = NUM;
              tmp.line_no = line_no;
              return tmp;
            }
          }
          }
          else if(c == '.'){
            tmp.lexeme += c;
            used_lex++;
            input.GetChar(c);
            let_grabbed[grabbed] = c;
            grabbed++;
            if(tmp.lexeme[0] != '9'){
            if(isdigit(c)){
              while(!input.EndOfInput() && isdigit(c)){
                tmp.lexeme += c;
                used_lex++;
                input.GetChar(c);
                let_grabbed[grabbed] = c;
                grabbed++;

              }
              if(!input.EndOfInput() || !isdigit(c)) {
                  input.UngetChar(c);
              }
              tmp.token_type = REALNUM;
              tmp.line_no = line_no;
              return tmp;
            }
            else{
              zero = false;
              for(int i = grabbed-1; i >= 0; i--){
                input.UngetChar(let_grabbed[i]);
              }
              for(int i = 0; i<used_lex; i++){
                tmp.lexeme.pop_back();
              }

            }
          }
            else{
              zero = false;
              for(int i = grabbed-1; i >= 0; i--){
                input.UngetChar(let_grabbed[i]);
              }
              for(int i = 0; i<used_lex; i++){
                tmp.lexeme.pop_back();
              }
            }
          }
          else if(c == 'x'){
            tmp.lexeme += c;
            used_lex++;
            input.GetChar(c);
            let_grabbed[grabbed] = c;
            grabbed++;
            if(c == '0'){
              tmp.lexeme += c;
              used_lex++;
              input.GetChar(c);
              let_grabbed[grabbed] =c;
              grabbed++;
              if(c == '8'){
                if(tmp.lexeme[0] != '0'){
                  if(tmp.lexeme[0] != '9'){
                    if(tmp.lexeme[0] != '8'){
                  for(int i = 0; i < grabbed; i++){
                    if(tmp.lexeme[i] == '0'|| tmp.lexeme[i] == '1' || tmp.lexeme[i] == '2' || tmp.lexeme[i] == '3'
                        || tmp.lexeme[i] == '4' || tmp.lexeme[i] == '5' || tmp.lexeme[i] == '6' || tmp.lexeme[i] == 'x'){
                          flag_8 = false;
                          if(let_grabbed[i] == 'x'){
                            break;
                          }
                        }
                        else{
                          flag_8 = true;
                          break;
                        }
                    }
                  }
                  else{
                    flag_8 = true;
                  }
                  }
                  else{
                    flag_8 = true;
                  }
                  }
                  else{
                    flag_8 = true;
                  }
                if(!flag_8){
                tmp.lexeme += c;
                tmp.token_type = BASE08NUM;
                tmp.line_no = line_no;
                flag_8 = false;
                return tmp;
                }
                else{
                  for(int i = grabbed-1; i>= 0; i--){
                    input.UngetChar(let_grabbed[i]);
                  }
                  for(int i = 0; i < used_lex; i++){
                    tmp.lexeme.pop_back();
                  }
                }
              }
              else{
                for(int i = grabbed-1; i>= 0; i--){
                  input.UngetChar(let_grabbed[i]);
                }
                for(int i = 0; i < used_lex; i++){
                  tmp.lexeme.pop_back();
                }
              }
            }
            else if(c == '1'){
              tmp.lexeme += c;
              used_lex++;
              input.GetChar(c);
              let_grabbed[grabbed] = c;
              grabbed++;
              if(c == '6'){
                tmp.lexeme += c;
                tmp.token_type = BASE16NUM;
                tmp.line_no = line_no;
                return tmp;
              }
              else{
                input.UngetChar(c);
              }
            }
            else{
              for(int i = grabbed-1; i>= 0; i--){
                input.UngetChar(let_grabbed[i]);
              }
              for(int i = 0; i < used_lex; i++){
              tmp.lexeme.pop_back();
              }
            }
          }
          else if(c == 'A' || c == 'B' || c == 'C' || c == 'D' || c == 'E' || c == 'F'){
            input.UngetChar(c);
            bool no_match = true;
            int temp = 0;
            int get_temp = 0;
            char chtemp [100];
            while(no_match){
              input.GetChar(c);
              chtemp[get_temp] = c;
              get_temp++;
              if(c == 'A' || c == 'B' || c == 'C' || c == 'D' || c == 'E' || c == 'F'){
                tmp.lexeme += c;
                temp++;
              }
              else if(c =='x'){
                tmp.lexeme += c;
                temp++;
                input.GetChar(c);
                chtemp[get_temp] = c;
                get_temp++;
                if(c == '1'){
                  tmp.lexeme += c;
                  temp++;
                  input.GetChar(c);
                  chtemp[get_temp] = c;
                  get_temp++;
                  if(c == '6'){
                    tmp.lexeme += c;
                    temp++;
                    tmp.token_type = BASE16NUM;
                    tmp.line_no = line_no;
                    return tmp;
                  }
                  // else not a 6
                }//else not a 1
                else{
                  no_match = false;
                  for(int i = get_temp-1; i >= 0; i--){
                    if(chtemp[i] != '\n'){
                      input.UngetChar(chtemp[i]);
                    }
                  }
                  for(int i = 0; i< temp; i++){
                    tmp.lexeme.pop_back();
                  }
                }
              }  //else not an 'x'
              else if(isdigit(c)){
                while(!input.EndOfInput() && isdigit(c)) {
                  tmp.lexeme += c;
                  temp++;
                  input.GetChar(c);
                  chtemp[get_temp] =c;
                  get_temp++;
                  flag_8 = true;
                }
                input.UngetChar(c);
                chtemp[get_temp-1] = 0;
                get_temp--;
              }// else not an x, not a digit, not a Hexa letter
              else{
                no_match = false;
                for(int i = 0; i <temp; i++){
                  tmp.lexeme.pop_back();
                }
                for(int i = get_temp-1; i >= 0; i--){
                  input.UngetChar(chtemp[i]);
                }
              }
            }*/


        tmp.token_type = NUM;
        tmp.line_no = line_no;
        return tmp;
      }else {
        if (!input.EndOfInput()) {
            input.UngetChar(c);
        }
        tmp.lexeme = "";
        tmp.token_type = ERROR;
        tmp.line_no = line_no;
        return tmp;
    }
}

Token LexicalAnalyzer::ScanIdOrKeyword()
{
    char c;
    input.GetChar(c);

    if (isalpha(c)) {
        tmp.lexeme = "";
        while (!input.EndOfInput() && isalnum(c)) {
            tmp.lexeme += c;
            input.GetChar(c);
        }
        if (!input.EndOfInput()) {
            input.UngetChar(c);
        }
        tmp.line_no = line_no;
        if (IsKeyword(tmp.lexeme))
            tmp.token_type = FindKeywordIndex(tmp.lexeme);
        else
            tmp.token_type = ID;
    } else {
        if (!input.EndOfInput()) {
            input.UngetChar(c);
        }
        tmp.lexeme = "";
        tmp.token_type = ERROR;
    }
    return tmp;
}

// you should unget tokens in the reverse order in which they
// are obtained. If you execute
//
//    t1 = lexer.GetToken();
//    t2 = lexer.GetToken();
//    t3 = lexer.GetToken();
//
// in this order, you should execute
//
//    lexer.UngetToken(t3);
//    lexer.UngetToken(t2);
//    lexer.UngetToken(t1);
//
// if you want to unget all three tokens. Note that it does not
// make sense to unget t1 without first ungetting t2 and t3
//
TokenType LexicalAnalyzer::UngetToken(Token tok)
{
    tokens.push_back(tok);;
    return tok.token_type;
}

Token LexicalAnalyzer::GetToken()
{
    char c;

    // if there are tokens that were previously
    // stored due to UngetToken(), pop a token and
    // return it without reading from input
    if (!tokens.empty()) {
        tmp = tokens.back();
        tokens.pop_back();
        return tmp;
    }
    char c1;
    input.GetChar(c1);
    if(!input.EndOfInput()){
      while( isspace(c1) || c1 == '/'){
        input.UngetChar(c1);
        //c1 = 'n';
        SkipSpace();
        if(!input.EndOfInput()){
          skipComment();
          input.GetChar(c1);
        }
        else{
          break;
        }

      }
      if(c1 != '\n' || c1 != '/'){
        input.UngetChar(c1);
      }
    }
    tmp.lexeme = "";
    tmp.line_no = line_no;
    input.GetChar(c);
    switch (c) {
        case '.':
            tmp.lexeme = '.';
            tmp.token_type = DOT;
            return tmp;
        case 'p':
            input.GetChar(c);
            if(c == 'r'){
              input.GetChar(c);
              if(c == 'i'){//most likely private
                input.GetChar(c);
                if(c == 'v'){
                  input.GetChar(c);
                  if(c == 'a'){
                    input.GetChar(c);
                    if(c == 't'){
                      input.GetChar(c);
                      if(c == 'e'){

                        tmp.token_type = PRIVATE;
                        return tmp;
                      }
                    }
                  }
                }
              }
            }
            else if(c == 'u'){
              input.GetChar(c);
              if(c == 'b'){
                input.GetChar(c);
                if(c == 'l'){
                  input.GetChar(c);
                  if(c == 'i'){
                    input.GetChar(c);
                    if(c == 'c'){
                      tmp.token_type = PUBLIC;
                      return tmp;
                    }
                  }
                }
              }
            }
        case '+':
            tmp.lexeme = '+';
            tmp.token_type = PLUS;
            return tmp;
        case '{':
            tmp.lexeme = '{';
            tmp.token_type = LBRAC;
            return tmp;
        case '}':
            tmp.lexeme = '}';
            tmp.token_type = RBRAC;
            return tmp;
        case '-':
            tmp.lexeme = '-';
            tmp.token_type = MINUS;
            return tmp;
        case '/':
            tmp.lexeme = '/';
            tmp.token_type = DIV;
            return tmp;
        case '*':
            tmp.lexeme = '*';
            tmp.token_type = MULT;
            return tmp;
        case '=':
            tmp.lexeme = '=';
            tmp.token_type = EQUAL;
            return tmp;
        case ':':
            tmp.lexeme = ':';
            tmp.token_type = COLON;
            return tmp;
        case ',':
            tmp.lexeme = ',';
            tmp.token_type = COMMA;
            return tmp;
        case ';':
            tmp.lexeme = ';';
            tmp.token_type = SEMICOLON;
            return tmp;
        case '[':
            tmp.lexeme = '[';
            tmp.token_type = LBRAC;
            return tmp;
        case ']':
            tmp.lexeme = ']';
            tmp.token_type = RBRAC;
            return tmp;
        case '(':
            tmp.lexeme = '(';
            tmp.token_type = LPAREN;
            return tmp;
        case ')':
            tmp.lexeme = ')';
            tmp.token_type = RPAREN;
            return tmp;
        case '<':
            input.GetChar(c);
            if (c == '=') {
                tmp.token_type = LTEQ;
            } else if (c == '>') {
                tmp.token_type = NOTEQUAL;
            } else {
                if (!input.EndOfInput()) {
                    input.UngetChar(c);
                }
                tmp.token_type = LESS;
            }
            return tmp;
        case '>':
            input.GetChar(c);
            if (c == '=') {
                tmp.token_type = GTEQ;
            } else {
                if (!input.EndOfInput()) {
                    input.UngetChar(c);
                }
                tmp.token_type = GREATER;
            }
            return tmp;
        default:
            if (isdigit(c)) {
                input.UngetChar(c);
                return ScanNumber();
            } else if (isalpha(c)) {
                input.UngetChar(c);
                return ScanIdOrKeyword();
            } else if (input.EndOfInput())
                tmp.token_type = END_OF_FILE;
            else
                tmp.token_type = ERROR;

            return tmp;
    }
}

/*int main()
{
    LexicalAnalyzer lexer;
    Token token;

    token = lexer.GetToken();
    token.Print();
    while (token.token_type != END_OF_FILE)
    {
        token = lexer.GetToken();
        token.Print();
    }
}*/
