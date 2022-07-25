#pragma once

#include "control_flow.h"
#include "panic.h"
#include "TypedValue.h"
#include "Expr.h"
#include "evaluate_expr.h"

TypedValue execute_body(std::vector<Statement> statements);

void execute_function_declaration(Function *function);

void execute_if(If *if_statement);

void execute_while(While *while_statement);

void execute_assign(Assign *assign);

void execute_let(Let *let);

void execute_var(Var *var);

void execute_const(Const *const_var);

