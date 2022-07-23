#pragma once

struct Assign;
struct If;
struct While;
struct Function;

struct Let {
    std::string name;
    Expr expr;
};


struct Var {
    std::string name;
    Expr expr;
};

struct Const {
    std::string name;
    Expr expr;
};

typedef std::variant<Assign *, If *, While *, Function *, Let *, Var *, Const *, Expr *> Statement;

struct Function {
    std::string name;
    std::vector<std::string> parameters;
    std::vector<Statement> body;
};

struct Assign {
    std::string name;
    Expr expr;
};

struct If {
    Expr condition;
    std::vector<Statement> body;
};

struct While {
    Expr condition;
    std::vector<Statement> body;
};