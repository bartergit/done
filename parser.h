#pragma once

#include <set>
#include <optional>
#include <cctype>
#include "TypedValue.h"
#include "expr_maker.h"

struct Parser {
    std::string input;
    int i = 0;
    std::set<std::string> operators{"+", "-", "*", "/", "==", "%", "&&", "||",
//                                                 "===", "!=", "!==",
    };

    char peek() {
        if (empty())
            panic();
        return input.at(i++);
    }

    char look() {
        if (empty())
            panic();
        return input.at(i);
    }

    bool empty() {
        return i >= input.size();
    }

    void expect(std::string expected) {
        if (empty() or expected != input.substr(i, expected.size())) {
            panic();
        }
        i += int(expected.size());
    }

    void check(bool condition) {
        if (!condition) {
            panic();
        }
    }

    void panic() {
        throw std::bad_optional_access();
    }

    void skip_spaces() {
        while (!empty() and look() == ' ') {
            peek();
        }
    }

    std::optional<std::string> parse_operator() {
        auto before_i = i;
        try {
            for (auto &op: operators) {
                for (char c: op) {
                    if (c != peek()) {
                        i = before_i;
                        break;
                    }
                }
                if (i != before_i) {
                    return op;
                }
            }
            return {};
        } catch (std::bad_optional_access &error) {
            i = before_i;
            return {};
        }
    }

    std::optional<std::string> parse_identifier() {
        auto before_i = i;
        try {
            std::stringstream ss;
            auto first_char = look();
            if (!isalpha(first_char) && first_char != '_') {
                return {};
            }
            while (!empty() and isalnum(look())) {
                ss << peek();
            }
            return ss.str();
        } catch (std::bad_optional_access &error) {
            i = before_i;
            return {};
        }
    }

    std::optional<std::string> parse_object() {
        auto before_i = i;
        try {
            expect("{");
            while (true) {
                auto id = *parse_any();
                expect(":");
                auto key = *parse_any();
                if (look() == '}') {
                    peek();
                    break;
                }
                expect(",");
            }
            expect("}");
        } catch (std::bad_optional_access &error) {
            i = before_i;
            return {};
        }
        }

    std::optional<std::string> parse_int() {
        auto before_i = i;
        try {
            std::stringstream ss;
            check(isdigit(look()));
            while (!empty() and isdigit(look())) {
                ss << peek();
            }
            return ss.str();
        } catch (std::bad_optional_access &error) {
            i = before_i;
            return {};
        }
    }

    std::optional<std::string> parse_float() {
        auto before_i = i;
        try {
            auto int_ptr = parse_int();        //wtf is going on here???
            expect(".");
            auto fraction = *parse_int();
            return *int_ptr + "." + fraction;
        } catch (std::bad_optional_access &error) {
            i = before_i;
            return {};
        }
    }

    std::optional<std::string> parse_string() {
        auto before_i = i;
        try {
            auto quote = "\"";
            expect(quote);
            std::stringstream ss;
            ss << quote;
            while (look() != '"') { //todo: escape string
                ss << peek();
            }
            expect(quote);
            ss << quote;
            return ss.str();
        } catch (std::bad_optional_access &error) {
            i = before_i;
            return {};
        }
    }

    std::optional<TypedValue> parse_any() noexcept {
        if (auto result = parse_string()) {
//            std::cout << "STRING IS" << result->c_str() << "\n";
            return TypedValue(*result);
        }
        if (auto result = parse_identifier()) {
            if (*result == "null"){
                return TypedValue(NULLJS);
            }
            if (*result == "undefined"){
                return TypedValue(UNDEFINED);
            }
//            if (*result == "null"){
//                return TypedValue(NULLJS);
//            }
//            return result;
        }
        if (auto result = parse_float()) {
            return TypedValue(std::stof(*result));
        }
        if (auto result = parse_int()) {
            return TypedValue(std::stof(*result));
        }
        if (auto result = parse_object()) {
            return TypedValue(OBJECT);
        }
        return {};
    }

    std::optional<std::vector<Expr>> parse() noexcept {
        std::vector<Expr> result{};
        while (!empty()) {
            skip_spaces();
            if (empty()) {
                break;
            }
            if (auto op = parse_operator()) {
                result.push_back(new BinExpr{*op});
                continue;
            }
            if (auto any = parse_any()) {
                result.push_back(new SingleExpr{*any});
                continue;
            }
            return {};
        }
        return result;
    }
};