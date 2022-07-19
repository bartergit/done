#include <iostream>
#include <iomanip>

#include "TypedValue.h"
#include "create_test_cases.h"
#include "parser.h"
#include "expr_maker.h"
#include "evaluate_expr.h"

// all math operations = A.cast<NUMBER> + B.cast<NUMBER>
// except + (special case for string)
// 15 == true => 15 == 1

int main() {
    Parser parser{"1.2 + 15 + \"11  sd \" + null * undefined "};
    auto tokens = parser.parse();
//    create_test_cases();
//    std::vector<const char *> input_tokens = {"a", "+", "b", "*", "d"};
    auto res = generate_expression_tree(*tokens);
    auto val = evaluate_expr(res);
    std::cout << val.str();
//    std::cout << std::holds_alternative<BinExpr *>(res) << "\n";
//    auto result = TypedValue(0.1f) + TypedValue(0.2f);
//    std::cout << std::setprecision(20) << 0.1 + 0.2 << "\n";
//    std::cout << result.str() << "\n";
//    std::cout << TypedValue(NULLJS).str() << "\n";
    return 0;
}
