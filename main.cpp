#include <iostream>
#include <string>

#include "create_test_cases.h"
#include "parser.h"
#include "expr_maker.h"

#include "Machine.h"

#include "evaluate_expr.h"
#include "control_flow.h"
#include "program_executor.h"


int main() {
    auto input = R"(1.2 + 15 + {1:2, "3": null , undefined: {true: ""} } - "11  sd " + null * undefined)";
    input = R"({1:3} == {1:"3"})";
    input = "a + 3";
    Parser parser{input};
    auto tokens = *parser.parse();
    auto res = generate_expression_tree(tokens);
    auto val = evaluate_expr(res);
    std::cout << val.str();
    return 0;
}
