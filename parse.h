#pragma once

#include <string>
#include "sstream"
#include "set"
#include "vector"
#include "optional"
#include "TypedValue.h"

#define EXPECT(token, value) if (expect(token) != value){ \
    i = before_i; \
    return {}; \
}

struct NewParser {
    std::vector <Token> tokens;
    int i = 0;

    bool empty() const {
        return i >= tokens.size();
    }

    std::optional <std::string> expect(TokenType expected_token) {
        if (empty()) {
            return {};
        }
        auto &[token, item] = tokens.at(i++);
        if (token == expected_token) {
            return item;
        }
        return {};
    }

    std::pair <TokenType, std::string> next() {
        return tokens.at(i);
    }

    std::pair <TokenType, std::string> consume() {
        return tokens.at(i++);
    }

    std::optional <TypedValue> parse_number() {
        auto before_i = i;
        if (auto number = expect(NUMBER_TOKEN)) {
            if (i + 1 < tokens.size() and next() == Token(OPERATOR, ".")) {
                i++;
                auto &[next_token, next_item] = consume();
                if (next_token == NUMBER_TOKEN) {
                    return TypedValue(std::stof(*number + "." + next_item));
                }
                i -= 2;
            }
            return TypedValue(std::stof(*number));
        }
        i = before_i;
        return {};
    }

    std::optional <TypedValue> parse_string() {
        auto before_i = i;
        if (auto str = expect(STRING_LIT)) {
            return TypedValue(str);
        }
        i = before_i;
        return {};
    }

    std::optional <std::string> parse_operator() {
        auto before_i = i;
        if (auto op = expect(OPERATOR)) {
            return op;
        }
        i = before_i;
        return {};
    }

    std::optional <Expr> parse_identifier() {
        auto before_i = i;
        if (auto id = expect(IDENTIFIER)) {
            if (*id == "null") {
                return new SingleExpr{TypedValue(NULLJS)};
            }
            if (*id == "undefined") {
                return new SingleExpr{TypedValue(UNDEFINED)};
            }
            if (*id == "true" or *id == "false") {
                return new SingleExpr{TypedValue(*id == "true")};
            }
            return new VariableRef(*id);
        }
        i = before_i;
        return {};
    }

    std::optional <TypedValue> parse_object() {
        auto before_i = i;
        EXPECT("{");
        std::unordered_map <std::string, TypedValue> data{};
        if (!empty() and next() == Token(SYMBOL, '}')) {
            consume();
            return TypedValue(data);
        }
        while (true) {
            auto key = parse_lvalue();
            if (!key.holds_value()) {
                i = before_i;
                return {};
            }
            EXPECT(":");
            auto value = parse_expression();
            if (!value.holds_value()) {
                i = before_i;
                return {};
            }
            data.insert({key, value});
            EXPECT(",");
        }
        if (!empty() and look() == Token(SYMBOL, '}')) {
            consume()
            break;
        }
        return TypedValue(data);
    }


    std::optional <Expr> parse_lvalue() {
        if (auto num = parse_number()) {
            return new SingleExpr(*num)
        }
        if (auto str = parse_string()) {
            return new SingleExpr(*str);
        }
        if (auto expr = parse_identifier()) {
            return *expr;
        }
    }

    std::optional <std::vector<Expr>> parse_expression() {
        std::vector <Expr> result{};
        auto before_i = i;
        while (true) {
            if (auto lvalue = parse_lvalue()) {
                result.push_back(*lvalue);
                continue;
            }
            if (auto op = parse_operator()) {
                result.push_back(new BinExpr(*op));
                continue;
            }
            if (auto ob = parse_object()) {
                result.push_back(new SingleExpr{*ob});
                continue;
            }
            if (result.empty()) {
                return {};
            }
            return result;
        }
    }

    std::optional <Let> parse_let() {
        auto before_i = i;
        EXPECT(IDENTIFIER, "let");
        auto name = expect(IDENTIFIER);
        if (name.holds_value()) {
            i = before_i;
            return {};
        }
        EXPECT(SYMBOL, "=");
        if (auto expr = parse_expression()) {
            EXPECT(";");
            return Let{*name, *expr};
        }
        i = before_i;
        return {};
    }

    std::optional <Let> parse_var() {
        auto before_i = i;
        EXPECT(IDENTIFIER, "var");
        auto name = expect(IDENTIFIER);
        if (!name.holds_value()) {
            i = before_i;
            return {};
        }
        EXPECT(SYMBOL, "=");
        if (auto expr = parse_expression()) {
            EXPECT(";");
            return Let{*name, *expr};
        }
        i = before_i;
        return {};
    }

    std::optional <If> parse_if() {
        auto before_i = i;
        EXPECT(IDENTIFIER, "if");
        EXPECT("(");
        auto expr = parse_expression();
        if (!expr.holds_value()) {
            i = before_i;
            return {};
        }
        EXPECT(")");
        if (auto body = parse_body()) {
            return If{*expr, *body};
        }
        i = before_i;
        return {};
    }

    std::optional <Statement> parse_statement() {
        if (auto var = parse_var()) {
            return *var;
        }
        if (auto let = parse_let()) {
            return *let;
        }
        if (auto if_statement = parse_if()) {
            return *if_statement;
        }
        return {};
    }

    std::optional <std::vector<Statement>> parse_body() {
        auto before_i = i;
        EXPECT(SYMBOL, "{");
        std::vector <Statement> body{};
        while (true) {
            auto statement = parse_statement();
            if (!statement.holds_value()) {
                i = before_i;
                return {};
            }
            body.push_back(*statement);
            if (!empty() and look() == Token(SYMBOL, '}')) {
                consume()
                return body;
            }
        }
    }
};