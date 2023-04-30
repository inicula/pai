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

#define IS_ARITH_OP(x) (x == OT_plus || x == OT_minus || x == OT_div || x == OT_mul)
#define IS_LOGICAL(x) (x == OT_and || x == OT_or || x == OT_neg)
#define IS_COMP(x) (x == OT_less || x == OT_greater || x == OT_equal)
#define IS_IRREDUCIBLE(expr_type)                                                             \
    (expr_type == ET_integer || expr_type == ET_bool || expr_type == ET_list)

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

static std::unordered_map<std::string, SharedExpr> vars;

static SharedExpr
add(const SharedExpr& left, const SharedExpr& right)
{
    auto res = new Expression{ET_integer, {}};

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

static SharedExpr
subtract(const SharedExpr& left, const SharedExpr& right)
{
    auto res = new Expression{ET_integer, {}};

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

static SharedExpr
multiply(const SharedExpr& left, const SharedExpr& right)
{
    auto res = new Expression{ET_integer, {}};

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

static SharedExpr
divide(const SharedExpr& left, const SharedExpr& right)
{
    auto res = new Expression{ET_integer, {}};

    if (left->type == ET_integer)
        res->members.value = left->members.value;
    else
        res->members.value = left->members.bvalue;

    if (right->type == ET_integer) {
        i64 d = right->members.value;
        assert(d);
        res->members.value /= d;
    } else {
        i64 d = right->members.bvalue;
        assert(d);
        res->members.value /= d;
    }

    return {res, Expression::Deleter{}};
}

static SharedExpr
concat(const SharedExpr& left, const SharedExpr& right)
{
    auto res = new Expression{ET_list, {.integers = left->members.integers}};
    for (auto& el : right->members.integers)
        res->members.integers.push_back(el);

    return {res, Expression::Deleter{}};
}

static bool
to_bool(const SharedExpr& e)
{
    assert(IS_IRREDUCIBLE(e->type));

    switch (e->type) {
    case ET_var:
        __builtin_unreachable();
    case ET_integer:
        return e->members.value;
    case ET_bool:
        return e->members.bvalue;
    case ET_list:
        return !e->members.integers.empty();
    case ET_operator:
        __builtin_unreachable();
    }
}

static SharedExpr
boolean(bool b)
{
    auto res = new Expression{ET_bool, {.bvalue = b}};
    return {res, Expression::Deleter{}};
}

static SharedExpr
cmp(const SharedExpr& left, const SharedExpr& right, OperatorType op)
{
    auto res = new Expression{ET_bool, {}};
    if (left->type != right->type) {
        res->members.bvalue = false;
    } else if (op == OT_less) {
        bool b;
        switch (left->type) {
        case ET_var:
            __builtin_unreachable();
        case ET_integer:
            b = left->members.value < right->members.value;
        case ET_bool:
            b = left->members.bvalue < right->members.bvalue;
        case ET_list:
            b = left->members.integers < right->members.integers;
        case ET_operator:
            __builtin_unreachable();
        }

        return boolean(b);
    } else if (op == OT_greater) {
        bool b;
        switch (left->type) {
        case ET_var:
            __builtin_unreachable();
        case ET_integer:
            b = left->members.value > right->members.value;
        case ET_bool:
            b = left->members.bvalue > right->members.bvalue;
        case ET_list:
            b = left->members.integers > right->members.integers;
        case ET_operator:
            __builtin_unreachable();
        }

        return boolean(b);
    } else if (op == OT_equal) {
        bool b;
        switch (left->type) {
        case ET_var:
            __builtin_unreachable();
        case ET_integer:
            b = left->members.value == right->members.value;
        case ET_bool:
            b = left->members.bvalue == right->members.bvalue;
        case ET_list:
            b = left->members.integers == right->members.integers;
        case ET_operator:
            __builtin_unreachable();
        }

        return boolean(b);
    } else {
        assert(false);
    }

    return {res, Expression::Deleter{}};
}

static std::shared_ptr<Expression>
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
            assert(left_eval && right_eval);
            assert(left_eval->type == right_eval->type);
            assert(IS_IRREDUCIBLE(left_eval->type));

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
                    assert(false); /* unsupported*/
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
            assert(IS_IRREDUCIBLE(left_eval->type) && IS_IRREDUCIBLE(right_eval->type));

            return cmp(left_eval, right_eval, operation.op);
        } else {
            assert(false);
        }
    }
    default:
        __builtin_unreachable();
    }
}

int
main()
{
    Expression e{ET_var, {.name = "afjskdjfsldjfklsjldjflsjdfjsljdfkjskdljfklsjdfbc"}};
}
