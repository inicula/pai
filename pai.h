#pragma once
#include <vector>
#include <memory>
#include <cstdint>
#include <unordered_map>
#include <cassert>

using u8 = uint8_t;
using usize = uint64_t;
using i64 = int64_t;

enum OperatorType : u8 {
    OT_plus = 0,
    OT_minus,
    OT_mul,
    OT_div,
    OT_mod,
    OT_and,
    OT_or,
    OT_neg,
    OT_less,
    OT_greater,
    OT_equal,
};

enum ExpressionType : u8 {
    ET_var = 0,
    ET_integer,
    ET_bool,
    ET_list,
    ET_str,
    ET_operator,
    ET_list_element,
    ET_builtin_func,
};

enum StatementType : u8 {
    ST_expr = 0,
    ST_if,
    ST_assign,
    ST_while,
    ST_if_else,
    ST_break,
};

enum StatementResult : u8 {
    SR_normal = 0,
    SR_break,
};

struct Expression {
    struct Deleter {
        void
        operator()(Expression *e) const
        {
            if (!e)
                return;

            switch (e->type) {
            case ET_var:
                break;
            case ET_integer:
                break;
            case ET_bool:
                break;
            case ET_list:
                std::destroy_at(&e->members.elements);
                break;
            case ET_str:
                std::destroy_at(&e->members.str);
                break;
            case ET_operator:
                std::destroy_at(&e->members.left);
                std::destroy_at(&e->members.right);
                break;
            case ET_list_element:
                std::destroy_at(&e->members.list);
                std::destroy_at(&e->members.index);
                break;
            case ET_builtin_func:
                std::destroy_at(&e->members.func_name);
                std::destroy_at(&e->members.arg);
                break;
            }

            delete e;
        }
    };

    ExpressionType type;
    union U {
        struct {
            std::string str;
        };
        struct {
            usize uid;
        };
        struct {
            i64 value;
        };
        struct {
            bool bvalue;
        };
        struct {
            std::vector<std::shared_ptr<Expression>> elements;
        };
        struct {
            OperatorType op;
            std::shared_ptr<Expression> left;
            std::shared_ptr<Expression> right;
        };
        struct {
            std::shared_ptr<Expression> list;
            std::shared_ptr<Expression> index;
        };
        struct {
            std::string func_name;
            std::shared_ptr<Expression> arg;
        };

        ~U() {}
    } members;
};

using SharedExpr = std::shared_ptr<Expression>;

struct Statement {
    struct Deleter {
        void
        operator()(Statement *s) const
        {
            if (!s)
                return;

            switch (s->type) {
            case ST_expr:
                std::destroy_at(&s->members.expr);
                break;
            case ST_if: /* Fallthrough */
            case ST_while:
                std::destroy_at(&s->members.condition);
                std::destroy_at(&s->members.body);
                break;
            case ST_assign:
                std::destroy_at(&s->members.val);
                break;
            case ST_if_else:
                std::destroy_at(&s->members.ie_condition);
                std::destroy_at(&s->members.i_body);
                std::destroy_at(&s->members.e_body);
                break;
            case ST_break:
                break;
            }

            delete s;
        }
    };

    StatementType type;
    union U {
        struct {
            SharedExpr expr;
        };
        struct {
            SharedExpr condition;
            std::vector<std::unique_ptr<Statement, Statement::Deleter>> body;
        };
        struct {
            usize uid;
            SharedExpr val;
        };
        struct {
            SharedExpr ie_condition;
            std::vector<std::unique_ptr<Statement, Statement::Deleter>> i_body;
            std::vector<std::unique_ptr<Statement, Statement::Deleter>> e_body;
        };

        ~U() {}
    } members;
};

using UniqStmt = std::unique_ptr<Statement, Statement::Deleter>;

#define pexit(...) pexit_(__FILE__, __LINE__, __VA_ARGS__)

void pexit_(const char *, int, bool, const char *, auto &&...);
SharedExpr add(const SharedExpr &, const SharedExpr &);
SharedExpr subtract(const SharedExpr &, const SharedExpr &);
SharedExpr multiply(const SharedExpr &, const SharedExpr &);
SharedExpr divide(const SharedExpr &, const SharedExpr &);
SharedExpr mod(const SharedExpr &, const SharedExpr &);
SharedExpr concat(const SharedExpr &, const SharedExpr &);
bool to_bool(const SharedExpr &);
SharedExpr boolean(bool);
bool cmp(const SharedExpr &, const SharedExpr &, OperatorType);
SharedExpr identifier(const std::string &);
SharedExpr number(i64);
SharedExpr list(const std::vector<SharedExpr> &);
SharedExpr operation(const SharedExpr &left, OperatorType op, const SharedExpr &right);
SharedExpr string(const std::string &);
SharedExpr list_element(const SharedExpr &list, const SharedExpr &index);
SharedExpr builtin_function(const std::string &, const SharedExpr &index);
UniqStmt expression_stmt(const SharedExpr &);
UniqStmt if_stmt(const SharedExpr &, std::vector<UniqStmt> &&);
UniqStmt assignment(const std::string &, const SharedExpr &);
UniqStmt while_stmt(const SharedExpr &, std::vector<UniqStmt> &&);
UniqStmt if_else_stmt(const SharedExpr &, std::vector<UniqStmt> &&, std::vector<UniqStmt> &&);
UniqStmt break_stmt();
std::shared_ptr<Expression> evaluate(const SharedExpr &);
StatementResult execute(const UniqStmt &);
void print(const SharedExpr &);
