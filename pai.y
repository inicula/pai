%language "c++"
%locations
%define api.value.type variant

%code top {
#include <variant>
#include <charconv>
#include "pai.h"
}

%code provides {
int yylex(yy::parser::semantic_type* yylval, yy::parser::location_type* yylloc);
}

%output  "pai_parser.cpp"
%defines "pai_parser.h"

%token IF
%token TRUE
%token FALSE
%token COMMA
%token BREAK
%token OPAREN
%token CPAREN
%token OBRAK
%token CBRAK
%token OBRACE
%token CBRACE
%token WHILE
%token ASSIGN
%token <std::string> NUMBER
%token <std::string> IDENTIFIER
%token <std::string> STR_LITERAL

%left DOUBLE_PIPE
%left DOUBLE_AMP
%left DOUBLE_EQUAL
%left LESS GREATER
%left PLUS MINUS
%left STAR SLASH
%precedence EXCL

%type <SharedExpr> expr
%type <std::vector<SharedExpr>*> exprs
%type <std::vector<i64>*> numbers
%type <OperatorType> bin_op

%%

program:
    exprs {
        for(auto e : *$1)
            print(e);
        delete $1;
    }

exprs:
     %empty {
        $$ = new std::vector<SharedExpr>();
        *$$ = {};
     }
|
     exprs expr {
        $1->push_back($2);
        $$ = $1;
     }

expr:
    IDENTIFIER {
        $$ = identifier($1);
    }
|
    NUMBER {
        i64 value;
        pexit(std::from_chars($1.data(), $1.data() + $1.size(), value).ec == std::errc{},
              "std::from_chars failed\n");
        $$ = number(value);
    }
|
    TRUE {
        $$ = boolean(true);
    }
|
    FALSE {
        $$ = boolean(false);
    }
|
    OBRAK numbers CBRAK {
        pexit($2, "Found nullptr\n");
        $$ = integers(*$2);
        delete $2;
    }
|
    expr bin_op expr {
        $$ = operation($1, $2, $3);
    }
|
    EXCL expr {
        $$ = operation($2, OT_neg, nullptr);
    }
|
    OPAREN expr CPAREN {
        $$ = $2;
    }
|
    STR_LITERAL {
        $$ = string($1);
    }

numbers:
    NUMBER {
        i64 value;
        pexit(std::from_chars($1.data(), $1.data() + $1.size(), value).ec == std::errc{},
              "std::from_chars failed\n");

        $$ = new std::vector<i64>();
        $$->push_back(value);
    }
|
    numbers COMMA NUMBER {
        i64 value;
        pexit(std::from_chars($3.data(), $3.data() + $3.size(), value).ec == std::errc{},
              "std::from_chars failed\n");

        $1->push_back(value);
        $$ = $1;
    }

bin_op:
    PLUS         { $$ = OT_plus;    } |
    MINUS        { $$ = OT_minus;   } |
    STAR         { $$ = OT_mul;     } |
    SLASH        { $$ = OT_div;     } |
    DOUBLE_AMP   { $$ = OT_and;     } |
    DOUBLE_PIPE  { $$ = OT_or;      } |
    LESS         { $$ = OT_less;    } |
    GREATER      { $$ = OT_greater; } |
    DOUBLE_EQUAL { $$ = OT_equal;   }

%%
