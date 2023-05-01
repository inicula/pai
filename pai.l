%option c++ noyywrap

%{
#include "pai.h"
#include "pai_parser.h"
%}

%option outfile="pai_lexer.cpp" header-file="pai_lexer.h"

%%

if                          return yy::parser::token::IF;
while                       return yy::parser::token::WHILE;
True                        return yy::parser::token::TRUE;
False                       return yy::parser::token::FALSE;
"="                         return yy::parser::token::EQUAL;
"+"                         return yy::parser::token::PLUS;
"-"                         return yy::parser::token::MINUS;
"*"                         return yy::parser::token::STAR;
"%"                         return yy::parser::token::MOD;
"/"                         return yy::parser::token::SLASH;
"<"                         return yy::parser::token::LESS;
">"                         return yy::parser::token::GREATER;
"=="                        return yy::parser::token::DOUBLE_EQUAL;
"&&"                        return yy::parser::token::DOUBLE_AMP;
"||"                        return yy::parser::token::DOUBLE_PIPE;
"("                         return yy::parser::token::OPAREN;
")"                         return yy::parser::token::CPAREN;
"["                         return yy::parser::token::OBRAK;
"]"                         return yy::parser::token::CBRAK;
"{"                         return yy::parser::token::OBRACE;
"}"                         return yy::parser::token::CBRACE;
","                         return yy::parser::token::COMMA;
"!"                         return yy::parser::token::EXCL;
[a-zA-Z_][a-zA-Z0-9_]*      return yy::parser::token::IDENTIFIER;
[0-9]+                      return yy::parser::token::NUMBER;
\"(\\.|[^\\"])*\"           return yy::parser::token::STR_LITERAL;
[ \t\n]+                    // Whitespaces skipped.
#.*                         // Commens skipped.

%%
