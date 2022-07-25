#include <vector>
#include <string>
#include <iostream>
#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>
#include <fstream>
#include <sstream>


std::string exec(const char *cmd) {
    std::array<char, 128> buffer{};
    std::string result;
    std::unique_ptr<FILE, decltype(&_pclose)> pipe(_popen(cmd, "r"), _pclose);
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return result;
}

std::vector<std::pair<std::string, std::string>> cross_product(std::vector<std::string> &entries) {
    std::vector<std::pair<std::string, std::string>> result{};
    result.reserve(entries.size() * entries.size());
    for (auto &item: entries) {
        for (auto &item2: entries) {
            result.emplace_back(item, item2);
        }
    }
    return result;
}

void write(const std::string &filename, const std::string &text) {
    std::fstream fs;
    fs.open(filename, std::ios::out);
    fs.seekg(0);
    fs << text;
    fs.close();
}

void create_test_cases() {
    std::vector<std::string> entries = {
            "1", "0", "-11", "5.5", "0.0",
            "true", "false",
            "\"string\"", "\"true\"", "\"false\"", "\"null\"", "\"undefined\"", "\"\"", "\"[Object object]\"", "\"{}\"",
            "\"{a:3}\"",
            "{}", "{1: 3}", "{0: 0}", "{null:undefined}", "{a: \"b\"}",
            "{\"\": {}}",
            "null", "undefined", "NaN"
    };
    std::vector<std::string> binary_operators = {"+", "-", "*", "/", "==", "%", "&&", "||",
//                                                 "===", "!=", "!==",
    };
    auto results = cross_product(entries);
    std::stringstream ss;
    for (auto &result: results) {
        for (auto &op: binary_operators) {
            auto expr = result.first + " " + op + " " + result.second;
            ss << "console.log('" << expr << ",'," << expr << ");\n";
        }
    }
    write("../tests.js", ss.str());
    write("../tests.txt", exec("node ../tests.js"));
}