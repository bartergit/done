#pragma once

#include <string>
#include "sstream"
#include "set"
#include "vector"
#include "optional"
#include "TypedValue.h"

enum TokenType {
    IDENTIFIER,
    SPACE,
    OPERATOR,
    NUMBER,
    SYMBOL
};

bool is_space(char c) {
    return c == ' ' or c == '\n' or c == '\t';
}

std::set<std::string> bin_operators{"+", "-", "*", "/", "==", "%", "&&", "||",
                                    "===", "!=", "!==", "."
};

void tokenize(const std::string &input) {
    auto last_token = SPACE;
    std::stringstream ss;
    std::vector<std::pair<TokenType, std::string>> result{};
    for (char c: input) {
        if (last_token == SPACE) {
            if (is_space(c)) {
                continue;
            } else if (isalpha(c) or c == '_') {
                last_token = IDENTIFIER;
                ss << c;
            } else if (isdigit(c)) {
                last_token = NUMBER;
                ss << c;
            }
        } else if (last_token == IDENTIFIER) {
            if (is_space(c)) {
                result.emplace_back(last_token, ss.str());
                ss.clear();
                last_token = SPACE;
            } else if (isalpha(c)) {
                ss << c;
            } else {
                result.emplace_back(last_token, ss.str());
                ss.clear();
                ss << c;
                last_token = SYMBOL;
            }
        } else if (last_token == NUMBER) {
            if (is_space(c)) {
                result.emplace_back(last_token, ss.str());
                ss.clear();
                last_token = SPACE;
            } else if (isdigit(c)) {
                ss << c;
            } else if (isalpha(c) or c == '_') {
                last_token = IDENTIFIER;
                ss << c;
            } else {
                result.emplace_back(last_token, ss.str());
                ss.clear();
                last_token = SYMBOL;
                ss << c;
            }
        } else
//            if (last_token == SYMBOL)
        {
            if (is_space(c)) {
                result.emplace_back(last_token, ss.str());
                last_token = SPACE;
                ss.clear();
            } else if (isalpha(c) or c == '_') {
                result.emplace_back(last_token, ss.str());
                last_token = IDENTIFIER;
                ss.clear();
                ss << c;
            } else if (isdigit(c)) {
                result.emplace_back(last_token, ss.str());
                last_token = NUMBER;
                ss.clear();
                ss << c;
            } else if (bin_operators.contains(ss.str() + c)) {
                ss << c;
            } else if (bin_operators.contains(ss.str())) {
                result.emplace_back(OPERATOR, ss.str());
                ss.clear();
                ss << c;
            } else {
                result.emplace_back(SYMBOL, ss.str());
                ss.clear();
                ss << c;
            }
        }
    }
}

typedef std::pair<TokenType, std::string> Token;

struct NewParser {
    std::vector<Token> tokens;
    int i = 0;

    void expect(TokenType expected_token, const std::string &expected_value) {
    }

    std::string expect(TokenType token) {

    }

    std::pair<TokenType, std::string> next() {
    }

    std::optional<TypedValue> parse_number() {
        auto number = expect(NUMBER);
        return TypedValue(std::stof(number));
    }

    void parse_expression() {
        while (next() != Token(SYMBOL, ";")) {
            auto s = parse_number();
        }
    }

#define EXPECT(token, value) if (expect(token) != value){ \
    return {}; \
}

    std::optional<> parse_let() {
        EXPECT(IDENTIFIER, "left");
        auto name = expect(IDENTIFIER);
        expect(SYMBOL, "=");
        parse_expression();
    }
};

struct Parser {
    std::string input;
    int i = 0;


    char peek_char() {
        return input.at(i++);
    }

    char look_char() {
        return input.at(i);
    }

    std::string peek_identifier() {
        std::stringstream ss;
        auto c = peek_char();
        if (!isalpha(c) and c != '_') {
            return {};
        }
        while (true) {
            c = peek_char();
            if (empty() or !(isalnum(c) or c == '_'))
                break;
            ss << c;
        }
        return ss.str();
    }

    std::string peek_non_alphanum_sequence() {
        std::stringstream ss;
        while (true) {
            auto c = peek_char();
            if (c == ' ' or c == '\n' or isalnum(c))
                break;
            ss << c;
        }
        return ss.str();
    }

    [[nodiscard]] bool empty() const {
        return i >= input.size();
    }

    void skip_spaces() {
        while (!empty() and look_char() == ' ') {
            peek_char();
        }
    }

    std::optional<std::string> parse_operator() {
        auto token = peek_non_alphanum_sequence();
        for (auto &op: operators) {
            if (token == op) {
                return op;
            }
        }
        return {};
    }

    std::optional<std::string> parse_int() {
        std::stringstream ss;
        if (!isdigit(look_char()))
            return {};
        while (!empty() and isdigit(look_char())) {
            ss << peek_char();
        }
        return ss.str();
    }
};