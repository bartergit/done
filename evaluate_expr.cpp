#include "evaluate_expr.h"

extern struct Machine machine;

TypedValue evaluate_expr(Expr expr) {
    if (holds_alternative<BinExpr *>(expr)) {
        auto expr_ptr = std::get<BinExpr *>(expr);
        if (expr_ptr->op == "+") {
            auto res = evaluate_expr(expr_ptr->first) + evaluate_expr(expr_ptr->second);
            delete expr_ptr;
            return res;
        }
        if (expr_ptr->op == "-") {
            auto res = evaluate_expr(expr_ptr->first) - evaluate_expr(expr_ptr->second);
            delete expr_ptr;
            return res;
        }
        if (expr_ptr->op == "*") {
            auto res = evaluate_expr(expr_ptr->first) * evaluate_expr(expr_ptr->second);
            delete expr_ptr;
            return res;
        }
        if (expr_ptr->op == "/") {
            auto res = evaluate_expr(expr_ptr->first) / evaluate_expr(expr_ptr->second);
            delete expr_ptr;
            return res;
        }
        if (expr_ptr->op == "==") {
            auto res = evaluate_expr(expr_ptr->first).eq(evaluate_expr(expr_ptr->second));
            delete expr_ptr;
            return res;
        }
        if (expr_ptr->op == "===") {
            auto res = evaluate_expr(expr_ptr->first).strict_eq(evaluate_expr(expr_ptr->second));
            delete expr_ptr;
            return res;
        }
        if (expr_ptr->op == "%") {
            auto res = evaluate_expr(expr_ptr->first) % evaluate_expr(expr_ptr->second);
            delete expr_ptr;
            return res;
        }
        if (expr_ptr->op == "&&") {
            auto res = evaluate_expr(expr_ptr->first) && evaluate_expr(expr_ptr->second);
            delete expr_ptr;
            return res;
        }
        if (expr_ptr->op == "||") {
            auto res = evaluate_expr(expr_ptr->first) || evaluate_expr(expr_ptr->second);
            delete expr_ptr;
            return res;
        }
        if (expr_ptr->op == "!=") {
            auto res = evaluate_expr(expr_ptr->first).neq(evaluate_expr(expr_ptr->second));
            delete expr_ptr;
            return res;
        }
        if (expr_ptr->op == "!==") {
            auto res = evaluate_expr(expr_ptr->first).strict_neq(evaluate_expr(expr_ptr->second));
            delete expr_ptr;
            return res;
        }
        if (expr_ptr->op == ".") {
            auto res = evaluate_expr(expr_ptr->first).get_attribute(evaluate_expr(expr_ptr->second));
            delete expr_ptr;
            return res;
        }
        std::unreachable();
    }
    if (std::holds_alternative<SingleExpr *>(expr)) {
        auto expr_ptr = std::get<SingleExpr *>(expr);
        auto value = expr_ptr->value;
        delete expr_ptr;
        return value;
    }
    if (std::holds_alternative<VariableRef *>(expr)) {
        auto expr_ptr = std::get<VariableRef *>(expr);
        if (!machine.memory.contains(expr_ptr->ref)) {
            std::cout << "Uncaught exception: unknown variable " << expr_ptr->ref << "\n";
        }
        auto value = machine.memory.at(expr_ptr->ref);
        delete expr_ptr;
        return value;
    }
    if (std::holds_alternative<FunctionCall *>(expr)) {
        auto expr_ptr = std::get<FunctionCall *>(expr);
        if (!machine.memory.contains(expr_ptr->name)) {
            std::cout << "Uncaught exception: unknown function " << expr_ptr->name << "\n";
        }
        auto function = machine.memory.at(expr_ptr->name);
        function();
    }
    std::unreachable();
}
