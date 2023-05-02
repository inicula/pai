#include "pai.h"
#include "pai_parser.h"
#include "pai_lexer.h"
#include <fstream>
#include <fmt/core.h>
#include <fmt/ranges.h>
#include <ranges>
#include <algorithm>

namespace ranges = std::ranges;

#ifdef SHOW_TYPES
#define INT_TYPE "int:"
#define BOOL_TYPE "bool"
#define LIST_TYPE "list:"
#define STR_TYPE "string:"
#else
#define INT_TYPE ""
#define BOOL_TYPE ""
#define LIST_TYPE ""
#define STR_TYPE ""
#endif
#define UNREACHABLE __builtin_unreachable()
#define IS_ARITH_OP(x)                                                                        \
    (x == OT_plus || x == OT_minus || x == OT_div || x == OT_mul || x == OT_mod)
#define IS_LOGICAL(x) (x == OT_and || x == OT_or || x == OT_neg)
#define IS_COMP(x) (x == OT_less || x == OT_greater || x == OT_equal)
#define IS_IRREDUCIBLE(expr_type)                                                             \
    (expr_type == ET_integer || expr_type == ET_bool || expr_type == ET_list ||               \
     expr_type == ET_str)

static yyFlexLexer *lexer;
static usize next_id = 0;
static std::unordered_map<std::string, usize> ids;
static std::vector<SharedExpr> vars;

static usize
get_id(const std::string &str)
{
    usize id;

    auto it = ids.find(str);
    if (it == ids.end()) {
        id = next_id++;
        ids[str] = id;
    } else {
        id = it->second;
    }

    return id;
}

void
pexit_(const char *file_name, int line, bool cond, const char *fmt_, auto &&...args)
{
    if (!cond) {
        auto fmt = "[{}:{}] Error: " + std::string(fmt_);
        fmt::print(stderr,
                   fmt::runtime(fmt),
                   file_name,
                   line,
                   std::forward<decltype(args)>(args)...);
        exit(EXIT_FAILURE);
    }
}

SharedExpr
add(const SharedExpr &left, const SharedExpr &right)
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
subtract(const SharedExpr &left, const SharedExpr &right)
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
multiply(const SharedExpr &left, const SharedExpr &right)
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
divide(const SharedExpr &left, const SharedExpr &right)
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
mod(const SharedExpr &left, const SharedExpr &right)
{
    auto res = new Expression{ET_integer, {.value = {}}};

    if (left->type == ET_integer)
        res->members.value = left->members.value;
    else
        res->members.value = left->members.bvalue;

    if (right->type == ET_integer) {
        i64 d = right->members.value;
        pexit(d, "Division by 0\n");
        res->members.value %= d;
    } else {
        i64 d = right->members.bvalue;
        pexit(d, "Division by 0\n");
        res->members.value %= d;
    }

    return {res, Expression::Deleter{}};
}

SharedExpr
concat(const SharedExpr &left, const SharedExpr &right)
{
    Expression *e = nullptr;
    if (left->type == ET_list) {
        e = new Expression{ET_list, {.elements = left->members.elements}};
        for (auto &el : right->members.elements)
            e->members.elements.push_back(el);
    } else {
        e = new Expression{ET_str, {.str = left->members.str}};
        e->members.str += right->members.str;
    }

    return {e, Expression::Deleter{}};
}

bool
to_bool(const SharedExpr &e)
{
    pexit(IS_IRREDUCIBLE(e->type), "Expected irreducible type\n");

    switch (e->type) {
    case ET_integer:
        return e->members.value;
    case ET_bool:
        return e->members.bvalue;
    case ET_list:
        return !e->members.elements.empty();
    case ET_str:
        return !e->members.str.empty();
    default:
        UNREACHABLE;
    }
}

SharedExpr
boolean(bool b)
{
    auto res = new Expression{ET_bool, {.bvalue = b}};
    return {res, Expression::Deleter{}};
}

bool
cmp(const SharedExpr &left, const SharedExpr &right, OperatorType op)
{
    if (left->type != right->type) {
        return false;
    } else if (op == OT_less) {
        switch (left->type) {
        case ET_integer:
            return left->members.value < right->members.value;
        case ET_bool:
            return left->members.bvalue < right->members.bvalue;
        case ET_list: {
            auto &v1 = left->members.elements;
            auto &v2 = right->members.elements;
            return ranges::lexicographical_compare(v1, v2, [](auto &e1, auto &e2) {
                auto eval1 = evaluate(e1);
                auto eval2 = evaluate(e2);

                return cmp(eval1, eval2, OT_less);
            });
        }
        case ET_str:
            return left->members.str < right->members.str;
        default:
            UNREACHABLE;
        }
    } else if (op == OT_greater) {
        switch (left->type) {
        case ET_integer:
            return left->members.value > right->members.value;
        case ET_bool:
            return left->members.bvalue > right->members.bvalue;
        case ET_list: {
            auto &v1 = left->members.elements;
            auto &v2 = right->members.elements;
            return ranges::lexicographical_compare(v2, v1, [](auto &e1, auto &e2) {
                auto eval1 = evaluate(e1);
                auto eval2 = evaluate(e2);

                return cmp(eval1, eval2, OT_less);
            });
        }
        case ET_str:
            return left->members.str > right->members.str;
        default:
            UNREACHABLE;
        }
    } else if (op == OT_equal) {
        switch (left->type) {
        case ET_integer:
            return left->members.value == right->members.value;
        case ET_bool:
            return left->members.bvalue == right->members.bvalue;
        case ET_list: {
            auto &v1 = left->members.elements;
            auto &v2 = right->members.elements;
            return ranges::equal(v1, v2, [](auto &e1, auto &e2) {
                auto eval1 = evaluate(e1);
                auto eval2 = evaluate(e2);

                return cmp(eval1, eval2, OT_equal);
            });
        }
        case ET_str:
            return left->members.str == right->members.str;
        default:
            UNREACHABLE;
        }
    } else {
        pexit(false, "Bug\n");
        UNREACHABLE;
    }
}

SharedExpr
identifier(const std::string &str)
{
    auto res = new Expression{ET_var, {.uid = get_id(str)}};
    return {res, Expression::Deleter{}};
}

SharedExpr
number(i64 value)
{
    auto res = new Expression{ET_integer, {.value = value}};
    return {res, Expression::Deleter{}};
}

SharedExpr
list(const std::vector<SharedExpr> &elements)
{
    auto res = new Expression{ET_list, {.elements = elements}};
    return {res, Expression::Deleter{}};
}

SharedExpr
operation(const SharedExpr &left, OperatorType op, const SharedExpr &right)
{
    auto res = new Expression{ET_operator, {.op = op, .left = left, .right = right}};
    return {res, Expression::Deleter{}};
}

SharedExpr
string(const std::string &str)
{
    pexit(str.size() >= 2, "Received string without quotes\n");
    auto res = new Expression{ET_str, {.str = str.substr(1, str.size() - 2)}};
    return {res, Expression::Deleter{}};
}

SharedExpr
list_element(const SharedExpr &list, const SharedExpr &index)
{
    auto res = new Expression{ET_list_element, {.list = list, .index = index}};
    return {res, Expression::Deleter{}};
}

SharedExpr
builtin_function(const std::string &func_name, const SharedExpr &arg)
{
    auto res = new Expression{ET_builtin_func, {.func_name = func_name, .arg = arg}};
    return {res, Expression::Deleter{}};
}

UniqStmt
expression_stmt(const SharedExpr &expr)
{
    auto res = new Statement{ST_expr, {.expr = expr}};
    return UniqStmt{res};
}

UniqStmt
if_stmt(const SharedExpr &cond, std::vector<UniqStmt> &&body)
{
    auto res = new Statement{ST_if, {.condition = cond, .body = std::move(body)}};
    return UniqStmt{res};
}

UniqStmt
assignment(const std::string &name, const SharedExpr &value)
{
    auto res = new Statement{ST_assign, {.uid = get_id(name), .val = value}};
    return UniqStmt{res};
}

UniqStmt
while_stmt(const SharedExpr &cond, std::vector<UniqStmt> &&body)
{
    auto res = new Statement{ST_while, {.condition = cond, .body = std::move(body)}};
    return UniqStmt{res};
}

UniqStmt
if_else_stmt(const SharedExpr &cond, std::vector<UniqStmt> &&i_body,
             std::vector<UniqStmt> &&e_body)
{
    auto res = new Statement{
        ST_if_else,
        {.ie_condition = cond, .i_body = std::move(i_body), .e_body = std::move(e_body)}};
    return UniqStmt{res};
}

UniqStmt
break_stmt()
{
    return UniqStmt{new Statement{ST_break, {}}};
}

std::shared_ptr<Expression>
evaluate(const std::shared_ptr<Expression> &e)
{
    if (!e)
        return nullptr;

    switch (e->type) {
    case ET_var: {
        pexit(e->members.uid < vars.size(),
              "Could not find variable id: {}\n",
              e->members.uid);
        return vars[e->members.uid];
    }
    case ET_integer: /* Fallthrough */
    case ET_bool:    /* Fallthrough */
    case ET_str:
        return e;
    case ET_list: {
        std::vector<SharedExpr> reduced_elements;
        for (auto &el : e->members.elements)
            reduced_elements.emplace_back(evaluate(el));
        return list(reduced_elements);
    }
    case ET_operator: {
        auto &operation = e->members;

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
                case OT_mod:
                    return mod(left_eval, right_eval);
                default:
                    UNREACHABLE;
                }
            } else if (left_eval->type == ET_list || left_eval->type == ET_str) {
                switch (operation.op) {
                case OT_plus:
                    return concat(left_eval, right_eval);
                case OT_minus: /* Fallthrough */
                case OT_mul:   /* Fallthrough */
                case OT_div:   /* Fallthrough */
                case OT_mod:
                    pexit(false, "Lists and strings only support + (concatenation)\n");
                    UNREACHABLE;
                default:
                    UNREACHABLE;
                }
            }
        } else if (IS_LOGICAL(operation.op)) {
            auto b_left_eval = to_bool(evaluate(operation.left));
            if (operation.op == OT_neg)
                return boolean(!b_left_eval);

            if (operation.op == OT_and)
                return boolean(b_left_eval && to_bool(evaluate(operation.right)));
            else
                return boolean(b_left_eval || to_bool(evaluate(operation.right)));
        } else if (IS_COMP(operation.op)) {
            auto left_eval = evaluate(operation.left);
            auto right_eval = evaluate(operation.right);
            pexit(IS_IRREDUCIBLE(left_eval->type) && IS_IRREDUCIBLE(right_eval->type),
                  "Expected irreducible types\n");

            return boolean(cmp(left_eval, right_eval, operation.op));
        } else {
            pexit(false, "Bug\n");
            UNREACHABLE;
        }
    }
    case ET_list_element: {
        auto list = evaluate(e->members.list);
        pexit(list->type == ET_list, "Expected list\n");

        auto index = evaluate(e->members.index);
        pexit(index->type == ET_integer, "Expected integer\n");

        auto idx = index->members.value;
        pexit(idx >= 0 && usize(idx) <= list->members.elements.size(),
              "List index out of bounds\n");

        return evaluate(list->members.elements[usize(idx)]);
    }
    case ET_builtin_func: {
        if (e->members.func_name == "len") {
            auto argument = evaluate(e->members.arg);

            switch (argument->type) {
            case ET_str:
                return number(i64(argument->members.str.size()));
            case ET_list:
                return number(i64(argument->members.elements.size()));
            default:
                pexit(false, "len() only accepts a list or a string\n");
                UNREACHABLE;
            }
        } else if (e->members.func_name == "read") {
            i64 value;
            std::cin >> value;
            return number(value);
        } else {
            pexit(false, "Unknown function: {}\n", e->members.func_name);
            UNREACHABLE;
        }
        break;
    }
    default:
        UNREACHABLE;
    }
}

StatementResult
execute(const UniqStmt &stmt)
{
    if (!stmt)
        return SR_normal;

    switch (stmt->type) {
    case ST_expr:
        print(stmt->members.expr);
        break;
    case ST_if:
        if (to_bool(evaluate(stmt->members.condition))) {
            for (auto &stmt : stmt->members.body) {
                if (execute(stmt) == SR_break)
                    return SR_break;
            }
        }
        break;
    case ST_assign:
        pexit(stmt->members.uid <= vars.size(), "Bug\n");
        if (stmt->members.uid == vars.size())
            vars.emplace_back();
        vars[stmt->members.uid] = evaluate(stmt->members.val);
        break;
    case ST_while:
        while (to_bool(evaluate(stmt->members.condition))) {
            bool found_break = false;
            for (auto &stmt : stmt->members.body) {
                if (execute(stmt) == SR_break) {
                    found_break = true;
                    break;
                }
            }

            if (found_break)
                break;
        }
        break;
    case ST_if_else: {
        std::vector<UniqStmt> *body = &stmt->members.e_body;
        if (to_bool(evaluate(stmt->members.ie_condition)))
            body = &stmt->members.i_body;

        for (auto &stmt : *body) {
            if (execute(stmt) == SR_break)
                return SR_break;
        }
        break;
    }
    case ST_break:
        return SR_break;
    }

    return SR_normal;
}

static void
print_helper(const SharedExpr &e_)
{
    auto reduced = evaluate(e_);

    bool first = true;
    switch (reduced->type) {
    case ET_integer:
        fmt::print(INT_TYPE "{}", reduced->members.value);
        break;
    case ET_bool:
        fmt::print(BOOL_TYPE "{}", reduced->members.bvalue ? "True" : "False");
        break;
    case ET_list:
        fmt::print("[");
        for (auto &el : reduced->members.elements) {
            fmt::print(fmt::runtime(first ? "" : ", "));
            print_helper(el);
            first = false;
        }
        fmt::print("]");
        break;
    case ET_str:
        fmt::print(STR_TYPE "'{}'", reduced->members.str);
        break;
    default:
        UNREACHABLE;
    }
}

void
print(const SharedExpr &e)
{
    print_helper(e);
    fmt::print("\n");
}

int
yylex(yy::parser::semantic_type *yylval, yy::parser::location_type *yylloc)
{
    yylloc->begin.line = lexer->lineno();
    int token = lexer->yylex();
    if (token == yy::parser::token::IDENTIFIER || token == yy::parser::token::NUMBER ||
        token == yy::parser::token::STR_LITERAL) {
        yylval->build(std::string(lexer->YYText()));
    }
    return token;
}

void
yy::parser::error(const location_type &loc, const std::string &msg)
{
    fmt::print(stderr, "Line {}: {}\n", loc.begin.line, msg);
    exit(EXIT_FAILURE);
}

int
main(int argc, char **argv)
{
    pexit(argc == 2, "Usage: pai <input-file>\n");

    std::ifstream in(argv[1]);
    lexer = new yyFlexLexer(&in);
    yy::parser parser;
    parser.parse();
    delete lexer;
}
