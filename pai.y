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
%token ELSE
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
%left STAR SLASH MOD
%right EXCL

%type <SharedExpr> expr
%type <UniqStmt> stmt
%type <std::vector<UniqStmt>*> stmts
%type <i64> num
%type <std::vector<i64>*> numbers

%%

program:
    stmts {
        for (auto& stmt : *$1)
            execute(stmt);
        delete $1;
    }
;
stmts:
     %empty {
        $$ = new std::vector<UniqStmt>();
     }
|
     stmts stmt {
        $1->emplace_back(std::move($2));
        $$ = $1;
     }
;
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
    IF expr OBRACE stmts CBRACE ELSE OBRACE stmts CBRACE {
        $$ = if_else_stmt($2, std::move(*$4), std::move(*$8));
        delete $4;
        delete $8;
    }
|
    WHILE expr OBRACE stmts CBRACE {
        $$ = while_stmt($2, std::move(*$4));
        delete $4;
    }
|
    IDENTIFIER EQUAL expr {
        $$ = assignment($1, $3);
    }
;
expr:
    IDENTIFIER {
        $$ = identifier($1);
    }
|
    num {
        $$ = number($1);
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
    OBRAK CBRAK {
        $$ = integers({});
    }
|
    expr OBRAK expr CBRAK {
        $$ = list_element($1, $3);
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
    expr MOD expr {
        $$ = operation($1, OT_mod, $3);
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
;
numbers:
    num {
        $$ = new std::vector<i64>();
        $$->push_back($1);
    }
|
    numbers COMMA num {
        $1->push_back($3);
        $$ = $1;
    }
;
num:
    NUMBER {
        i64 value;
        pexit(std::from_chars($1.data(), $1.data() + $1.size(), value).ec == std::errc{},
              "std::from_chars failed\n");
        $$ = value;
    }
|
    MINUS NUMBER {
        $2 = "-" + $2;
        i64 value;
        pexit(std::from_chars($2.data(), $2.data() + $2.size(), value).ec == std::errc{},
              "std::from_chars failed\n");
        $$ = value;
    }
;
%%
