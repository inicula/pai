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
%token EQUAL
%token <std::string> NUMBER
%token <std::string> IDENTIFIER
%token <std::string> STR_LITERAL

%left DOUBLE_PIPE
%left DOUBLE_AMP
%left DOUBLE_EQUAL
%left LESS GREATER
%left PLUS MINUS
%left STAR SLASH
%right EXCL

%type <SharedExpr> expr
%type <UniqStmt> stmt
%type <std::vector<UniqStmt>*> stmts
%type <std::vector<i64>*> numbers

%%

program:
    stmts {
        for (auto& stmt : *$1)
            execute(stmt);
        delete $1;
    }

stmts:
     %empty {
        $$ = new std::vector<UniqStmt>();
     }
|
     stmts stmt {
        $1->emplace_back(std::move($2));
        $$ = $1;
     }

stmt:
    expr {
        $$ = expression_stmt($1);
    }
|
    IF expr OBRACE stmts CBRACE {
        $$ = if_stmt($2, std::move(*$4));
        delete $4;
    }
|
    IDENTIFIER EQUAL expr {
        $$ = assignment($1, $3);
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
    expr PLUS expr {
        $$ = operation($1, OT_plus, $3);
    }
|
    expr MINUS expr {
        $$ = operation($1, OT_minus, $3);
    }
|
    expr STAR expr {
        $$ = operation($1, OT_mul, $3);
    }
|
    expr SLASH expr {
        $$ = operation($1, OT_div, $3);
    }
|
    expr DOUBLE_AMP expr {
        $$ = operation($1, OT_and, $3);
    }
|
    expr DOUBLE_PIPE expr {
        $$ = operation($1, OT_or, $3);
    }
|
    expr LESS expr {
        $$ = operation($1, OT_less, $3);
    }
|
    expr GREATER expr {
        $$ = operation($1, OT_greater, $3);
    }
|
    expr DOUBLE_EQUAL expr {
        $$ = operation($1, OT_equal, $3);
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
|
    MINUS expr {
        $$ = operation(number(0), OT_minus, $2);
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

%%
