#pragma once
#include <vector>
#include <memory>
#include <cstdint>
#include <unordered_map>
#include <cassert>

using u8 = uint8_t;
using i64 = int64_t;

enum OperatorType : u8 {
    OT_plus,
    OT_minus,
    OT_mul,
    OT_div,
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
    ET_operator,
};

struct Expression {
    struct Deleter {
        void
        operator()(Expression* e) const
        {
            switch (e->type) {
            case ET_var:
                std::destroy_at(&e->members.name);
            case ET_integer:
                break;
            case ET_bool:
                break;
            case ET_list:
                std::destroy_at(&e->members.integers);
            case ET_operator:
                std::destroy_at(&e->members.left);
                std::destroy_at(&e->members.right);
                break;
            }
        }
    };

    ExpressionType type;
    union U {
        struct {
            std::string name;
        };
        struct {
            i64 value;
        };
        struct {
            bool bvalue;
        };
        struct {
            std::vector<i64> integers;
        };
        struct {
            OperatorType op;
            std::shared_ptr<Expression> left;
            std::shared_ptr<Expression> right;
        };

        ~U() {}
    } members;
};

using SharedExpr = std::shared_ptr<Expression>;

SharedExpr add(const SharedExpr&, const SharedExpr&);
SharedExpr subtract(const SharedExpr&, const SharedExpr&);
SharedExpr multiply(const SharedExpr&, const SharedExpr&);
SharedExpr divide(const SharedExpr&, const SharedExpr&);
SharedExpr concat(const SharedExpr&, const SharedExpr&);
bool to_bool(const SharedExpr&);
SharedExpr boolean(bool);
SharedExpr cmp(const SharedExpr&, const SharedExpr&, OperatorType);
SharedExpr identifier(const std::string&);
SharedExpr number(i64);
SharedExpr integers(const std::vector<i64>&);
SharedExpr operation(const SharedExpr& left, OperatorType op, const SharedExpr& right);
std::shared_ptr<Expression> evaluate(const SharedExpr&);
void print(const SharedExpr&);
