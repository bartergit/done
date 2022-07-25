#pragma once

#include "TypedValue.h"

struct SingleExpr {
    TypedValue value;
};

struct BinExpr;

struct FunctionCall;

struct VariableRef {
    std::string ref;
};

typedef std::variant<BinExpr *, SingleExpr *, VariableRef *, FunctionCall *> Expr;

struct BinExpr {
    std::string op;
    Expr first;
    Expr second;
};

struct FunctionCall {
    std::string name;
    std::vector<Expr> arguments;
};