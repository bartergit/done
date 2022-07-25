#pragma once

#include <set>
#include "TypedValue.h"
#include "Expr.h"


Expr generate_expression_tree(std::vector<Expr> &current_tokens) {
    static std::set<const char *> operators{"+", "-", "*", "/", "==", "%", "&&", "||",
                                            "===", "!=", "!==",
    };
    std::vector<Expr> temp_tokens;
    std::vector<std::set<std::string>> precedence = {{"."},
                                                     {"*",   "/",   "%"},
                                                     {"+",   "-"},
                                                     {"===", "!==", "!=", "=="},
                                                     {"||",  "&&"}};
    for (auto &cur_precedence: precedence) {
        auto i = -1;
        bool to_skip = false;
        for (auto &expr: current_tokens) {
            ++i;
            if (to_skip) {
                to_skip = false;
                continue;
            }
            std::visit(overloaded{
                    [&](FunctionCall *expr) {

                    },
                    [&](VariableRef *expr) {
                        temp_tokens.emplace_back(expr);
                    },
                    [&](SingleExpr *expr) {
                        temp_tokens.emplace_back(expr);
                    },
                    [&](BinExpr *expr) {
                        if (cur_precedence.contains(expr->op)) {
                            expr->first = temp_tokens.back();
                            temp_tokens.pop_back();
                            expr->second = current_tokens[i + 1];
                            to_skip = true;
                            temp_tokens.emplace_back(expr);
                            return;
                        }
                        temp_tokens.emplace_back(expr);
                    },
            }, expr);
        }
        current_tokens = temp_tokens;
        temp_tokens.clear();

    }
    return current_tokens[0];
}