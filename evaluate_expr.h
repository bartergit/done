#pragma once

#include "expr_maker.h"

TypedValue evaluate_expr(Expr expr) {
    // todo: free!
//    std::cout << (std::holds_alternative<BinExpr *>(expr) || std::holds_alternative<SingleExpr *>(expr));
    if (holds_alternative<BinExpr *>(expr)) {
//        std::cout << "before1" << "\n";
        auto expr_ptr = std::get<BinExpr *>(expr);
//        std::cout << "after1" << "\n";
        if (expr_ptr->op == "+") {
            auto res = evaluate_expr(expr_ptr->first) + evaluate_expr(expr_ptr->second);
            std::cout << expr_ptr->op << " " << res.str() << "\n";
            delete expr_ptr;
            return res;
        }
        if (expr_ptr->op == "-") {
            auto res = evaluate_expr(expr_ptr->first) - evaluate_expr(expr_ptr->second);
            std::cout << expr_ptr->op << " " << res.str() << "\n";
            delete expr_ptr;
            return res;
        }
        if (expr_ptr->op == "*") {
            auto res = evaluate_expr(expr_ptr->first) * evaluate_expr(expr_ptr->second);
            std::cout << expr_ptr->op << " " << res.str() << "\n";
            delete expr_ptr;
            return res;
        }
        if (expr_ptr->op == "/") {
            auto res = evaluate_expr(expr_ptr->first) / evaluate_expr(expr_ptr->second);
            std::cout << expr_ptr->op << " " << res.str() << "\n";
            delete expr_ptr;
            return res;
        }
    }
    if (std::holds_alternative<SingleExpr *>(expr)) {
        auto expr_ptr = std::get<SingleExpr *>(expr);
        auto value = expr_ptr->value;
        std::cout << expr_ptr->value.str() << "\n";
        delete expr_ptr;
        return value;
    }
}