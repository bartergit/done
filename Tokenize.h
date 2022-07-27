#pragma once

enum TokenType {
    IDENTIFIER,
    SPACE,
    OPERATOR,
    NUMBER_TOKEN,
    SYMBOL,
    STRING_LIT
};

std::set <std::string> bin_operators{"+", "-", "*", "/", "==", "%", "&&", "||",
                                     "===", "!=", "!==", "."
};


bool is_space(char c) {
    return c == ' ' or c == '\n' or c == '\t';
}

typedef std::pair <TokenType, std::string> Token;


void tokenize(const std::string &input) {
    auto last_token = SPACE;
    std::stringstream ss;
    std::vector <std::pair<TokenType, std::string>> result{};
    for (char c: input) {
        if (last_token == SPACE) {
            if (is_space(c)) {
                continue;
            } else if (isalpha(c) or c == '_') {
                last_token = IDENTIFIER;
                ss << c;
            } else if (isdigit(c)) {
                last_token = NUMBER_TOKEN;
                ss << c;
            } else if (c == '"') {
                last_token = STRING_LIT;
            } else {
                last_token = SYMBOL;
                ss << c;
            }
        } else if (last_token == IDENTIFIER) {
            if (is_space(c)) {
                result.emplace_back(last_token, ss.str());
                ss.clear();
                last_token = SPACE;
            } else if (isalpha(c)) {
                ss << c;
            } else if (c == '"') {
                result.emplace_back(last_token, ss.str());
                ss.clear();
                last_token = STRING_LIT;
            } else {
                result.emplace_back(last_token, ss.str());
                ss.clear();
                ss << c;
                last_token = SYMBOL;
            }
        } else if (last_token == NUMBER_TOKEN) {
            if (is_space(c)) {
                result.emplace_back(last_token, ss.str());
                ss.clear();
                last_token = SPACE;
            } else if (isdigit(c)) {
                ss << c;
            } else if (isalpha(c) or c == '_') {
                last_token = IDENTIFIER;
                ss << c;
            } else if (c == '"') {
                result.emplace_back(last_token, ss.str());
                ss.clear();
                last_token = STRING_LIT;
            } else {
                result.emplace_back(last_token, ss.str());
                ss.clear();
                last_token = SYMBOL;
                ss << c;
            }
        } else if (last_token == STRING_LIT) {
            if (c == '"') {
                result.emplace_back(last_token, ss.str());
                ss.clear();
                last_token = SPACE;
                continue;
            }
            ss << c;
        } else if (last_token == SYMBOL) {
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
                last_token = NUMBER_TOKEN;
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
        } else {
            panic("unreachable TokenType: OPERATOR");
        }
    }
}