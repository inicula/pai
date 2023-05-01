#include "pai.h"
#include "pai_parser.h"
#include "pai_lexer.h"
#include <fstream>
#include <fmt/core.h>
#include <fmt/ranges.h>

#define IS_ARITH_OP(x) (x == OT_plus || x == OT_minus || x == OT_div || x == OT_mul)
#define IS_LOGICAL(x) (x == OT_and || x == OT_or || x == OT_neg)
#define IS_COMP(x) (x == OT_less || x == OT_greater || x == OT_equal)
#define IS_IRREDUCIBLE(expr_type)                                                             \
    (expr_type == ET_integer || expr_type == ET_bool || expr_type == ET_list)

static yyFlexLexer* lexer;
static std::unordered_map<std::string, SharedExpr> vars;

void
pexit(bool cond, const char* fmt_, auto&&... args)
{
    if (!cond) {
        auto fmt = "Error: " + std::string(fmt_);
        fmt::print(stderr, fmt::runtime(fmt), std::forward<decltype(args)>(args)...);
        exit(EXIT_FAILURE);
    }
}

SharedExpr
add(const SharedExpr& left, const SharedExpr& right)
{
    auto res = new Expression{ET_integer, {.value = {}}};

    if (left->type == ET_integer)
        res->members.value = left->members.value;
    else
        res->members.value = left->members.bvalue;

    if (right->type == ET_integer)
        res->members.value += right->members.value;
    else
        res->members.value += right->members.bvalue;

    return {res, Expression::Deleter{}};
}

SharedExpr
subtract(const SharedExpr& left, const SharedExpr& right)
{
    auto res = new Expression{ET_integer, {.value = {}}};

    if (left->type == ET_integer)
        res->members.value = left->members.value;
    else
        res->members.value = left->members.bvalue;

    if (right->type == ET_integer)
        res->members.value -= right->members.value;
    else
        res->members.value -= right->members.bvalue;

    return {res, Expression::Deleter{}};
}

SharedExpr
multiply(const SharedExpr& left, const SharedExpr& right)
{
    auto res = new Expression{ET_integer, {.value = {}}};

    if (left->type == ET_integer)
        res->members.value = left->members.value;
    else
        res->members.value = left->members.bvalue;

    if (right->type == ET_integer)
        res->members.value *= right->members.value;
    else
        res->members.value *= right->members.bvalue;

    return {res, Expression::Deleter{}};
}

SharedExpr
divide(const SharedExpr& left, const SharedExpr& right)
{
    auto res = new Expression{ET_integer, {.value = {}}};

    if (left->type == ET_integer)
        res->members.value = left->members.value;
    else
        res->members.value = left->members.bvalue;

    if (right->type == ET_integer) {
        i64 d = right->members.value;
        pexit(d, "Division by 0\n");
        res->members.value /= d;
    } else {
        i64 d = right->members.bvalue;
        pexit(d, "Division by 0\n");
        res->members.value /= d;
    }

    return {res, Expression::Deleter{}};
}

SharedExpr
concat(const SharedExpr& left, const SharedExpr& right)
{
    auto res = new Expression{ET_list, {.integers = left->members.integers}};
    for (auto& el : right->members.integers)
        res->members.integers.push_back(el);

    return {res, Expression::Deleter{}};
}

bool
to_bool(const SharedExpr& e)
{
    pexit(IS_IRREDUCIBLE(e->type), "Expected irreducible type\n");

    switch (e->type) {
    case ET_integer:
        return e->members.value;
    case ET_bool:
        return e->members.bvalue;
    case ET_list:
        return !e->members.integers.empty();
    default:
        __builtin_unreachable();
    }
}

SharedExpr
boolean(bool b)
{
    auto res = new Expression{ET_bool, {.bvalue = b}};
    return {res, Expression::Deleter{}};
}

SharedExpr
cmp(const SharedExpr& left, const SharedExpr& right, OperatorType op)
{
    if (left->type != right->type) {
        return boolean(false);
    } else if (op == OT_less) {
        bool b;
        switch (left->type) {
        case ET_integer:
            b = left->members.value < right->members.value;
            break;
        case ET_bool:
            b = left->members.bvalue < right->members.bvalue;
            break;
        case ET_list:
            b = left->members.integers < right->members.integers;
            break;
        default:
            __builtin_unreachable();
        }

        return boolean(b);
    } else if (op == OT_greater) {
        bool b;
        switch (left->type) {
        case ET_integer:
            b = left->members.value > right->members.value;
            break;
        case ET_bool:
            b = left->members.bvalue > right->members.bvalue;
            break;
        case ET_list:
            b = left->members.integers > right->members.integers;
            break;
        default:
            __builtin_unreachable();
        }

        return boolean(b);
    } else if (op == OT_equal) {
        bool b;
        switch (left->type) {
        case ET_integer:
            b = left->members.value == right->members.value;
            break;
        case ET_bool:
            b = left->members.bvalue == right->members.bvalue;
            break;
        case ET_list:
            b = left->members.integers == right->members.integers;
            break;
        default:
            __builtin_unreachable();
        }

        return boolean(b);
    } else {
        pexit(false, "Bug\n");
        __builtin_unreachable();
    }
}

SharedExpr
identifier(const std::string& str)
{
    auto res = new Expression{ET_var, {.name = str}};
    return {res, Expression::Deleter{}};
}

SharedExpr
number(i64 value)
{
    auto res = new Expression{ET_integer, {.value = value}};
    return {res, Expression::Deleter{}};
}

SharedExpr
integers(const std::vector<i64>& integers)
{
    auto res = new Expression{ET_list, {.integers = integers}};
    return {res, Expression::Deleter{}};
}

SharedExpr
operation(const SharedExpr& left, OperatorType op, const SharedExpr& right)
{
    auto res = new Expression{ET_operator, {.op = op, .left = left, .right = right}};
    return {res, Expression::Deleter{}};
}

std::shared_ptr<Expression>
evaluate(const std::shared_ptr<Expression>& e)
{
    if (!e)
        return nullptr;

    switch (e->type) {
    case ET_var:
        return vars.at(e->members.name);
    case ET_integer: /* Fallthrough */
    case ET_bool:    /* Fallthrough */
    case ET_list:
        return e;
    case ET_operator: {
        auto& operation = e->members;

        if (IS_ARITH_OP(operation.op)) {
            auto left_eval = evaluate(operation.left);
            auto right_eval = evaluate(operation.right);
            pexit(left_eval && right_eval, "Found nullptr\n");
            pexit(left_eval->type == right_eval->type, "Mismatching types\n");
            pexit(IS_IRREDUCIBLE(left_eval->type), "Expected irreducible type\n");

            if (left_eval->type == ET_integer || left_eval->type == ET_bool) {
                switch (operation.op) {
                case OT_plus:
                    return add(left_eval, right_eval);
                case OT_minus:
                    return subtract(left_eval, right_eval);
                case OT_mul:
                    return multiply(left_eval, right_eval);
                case OT_div:
                    return divide(left_eval, right_eval);
                default:
                    __builtin_unreachable();
                }
            } else if (left_eval->type == ET_list) {
                switch (operation.op) {
                case OT_plus:
                    return concat(left_eval, right_eval);
                case OT_minus: /* Fallthrough */
                case OT_mul:   /* Fallthrough */
                case OT_div:
                    pexit(false, "Syntax error\n");
                default:
                    __builtin_unreachable();
                }
            }
        } else if (IS_LOGICAL(operation.op)) {
            auto b_left_eval = to_bool(evaluate(operation.left));
            if (operation.op == OT_neg)
                return boolean(!b_left_eval);

            auto b_right_eval = to_bool(evaluate(operation.right));
            if (operation.op == OT_and)
                return boolean(b_left_eval && b_right_eval);
            else
                return boolean(b_left_eval || b_right_eval);
        } else if (IS_COMP(operation.op)) {
            auto left_eval = evaluate(operation.left);
            auto right_eval = evaluate(operation.right);
            pexit(IS_IRREDUCIBLE(left_eval->type) && IS_IRREDUCIBLE(right_eval->type),
                  "Expected irreducible types\n");

            return cmp(left_eval, right_eval, operation.op);
        } else {
            pexit(false, "Bug\n");
        }
    }
    default:
        __builtin_unreachable();
    }
}

void
print(const SharedExpr& e_)
{
    auto reduced = evaluate(e_);
    pexit(IS_IRREDUCIBLE(reduced->type), "Expected irreducible type\n");

    switch (reduced->type) {
    case ET_integer:
        fmt::print("int: {}\n", reduced->members.value);
        break;
    case ET_bool:
        fmt::print("bool: {}\n", reduced->members.bvalue);
        break;
    case ET_list:
        fmt::print("list: {}\n", reduced->members.integers);
        break;
    default:
        __builtin_unreachable();
    }
}

int
yylex(yy::parser::semantic_type* yylval, yy::parser::location_type* yylloc)
{
    yylloc->begin.line = lexer->lineno();
    int token = lexer->yylex();
    if (token == yy::parser::token::IDENTIFIER || token == yy::parser::token::NUMBER) {
        yylval->build(std::string(lexer->YYText()));
    }
    return token;
}

void
yy::parser::error(const location_type& loc, const std::string& msg)
{
    fmt::print(stderr, "Line {}: {}\n", loc.begin.line, msg);
    exit(EXIT_FAILURE);
}

int
main(int argc, char** argv)
{
    pexit(argc == 2, "Usage: pai <input-file>\n");

    std::ifstream in(argv[1]);
    lexer = new yyFlexLexer(&in);
    yy::parser parser;
    parser.parse();
    delete lexer;
}
