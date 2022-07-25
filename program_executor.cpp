#include "program_executor.h"
#include "Machine.h"

extern struct Machine machine;

TypedValue execute_body(std::vector<Statement> statements) {
    for (auto &statement: statements) {
        std::visit(overloaded{
                [](If *arg) { execute_if(arg); },
                [](While *arg) { execute_while(arg); },
                [](Assign *arg) { execute_assign(arg); },
                [](Var *arg) { execute_var(arg); },
                [](Let *arg) { execute_let(arg); },
                [](Const *arg) { execute_const(arg); },
                [](Function *arg) { execute_function_declaration(arg); },
                [](Expr *arg) {
                    evaluate_expr(*arg);
                    delete arg;
                }
        }, statement);
    }
    return TypedValue(UNDEFINED);
}

void execute_function_declaration(Function *function) {
    auto function_obj = TypedValue(OBJECT);
    auto body_ptr = new std::vector<Statement>;
    *body_ptr = function->body;
    function_obj.unwrap<TypedValue::Attributes>() = {
            {"body", TypedValue((int) (body_ptr))}
    };
    machine.memory.insert({function->name, std::move(function_obj)});
    delete function;
}

void execute_if(If *if_statement) {
    auto condition = evaluate_expr(if_statement->condition);
    if (condition.cast<BOOLEAN>().unwrap<bool>()) {
        execute_body(if_statement->body);
    }
    delete if_statement;
}

void execute_while(While *while_statement) {
    while (true) {
        auto condition = evaluate_expr(while_statement->condition);
        if (!condition.cast<BOOLEAN>().unwrap<bool>()) {
            break;
        }
        execute_body(while_statement->body);
    }
    delete while_statement;
}

void execute_assign(Assign *assign) {
    machine.memory.insert({assign->name, evaluate_expr(assign->expr)});
    delete assign;
}

void execute_let(Let *let) {
    if (machine.memory.contains(let->name)) {
        std::cout << "Uncaught exception: redeclaration of let variable " << let->name << "\n";
        delete let;
        throw 1;
    }
    machine.memory.insert({let->name, evaluate_expr(let->expr)});
    delete let;
}

void execute_var(Var *var) {
    if (machine.memory.contains(var->name)) {
        std::cout << "Uncaught exception: redeclaration of var variable " << var->name << "\n";
        delete var;
        throw 1;
    }
    machine.memory.insert({var->name, evaluate_expr(var->expr)});
    delete var;
}

void execute_const(Const *const_var) {
    if (machine.memory.contains(const_var->name)) {
        std::cout << "Uncaught exception: redeclaration of const variable " << const_var->name << "\n";
        delete const_var;
        throw 1;
    }
    machine.memory.insert({const_var->name, evaluate_expr(const_var->expr)});
    delete const_var;
}
