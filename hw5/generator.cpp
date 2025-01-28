#include "generator.hpp"

extern output::CodeBuffer buffer;
extern std::vector<std::shared_ptr<ast::Node>> tables = outputAndSymbolTable::mishtaneMisgeret;

void LLVM_code_generator::globalFunctions() {
    buffer.emit("@.DIV_BY_ZERO_ERROR = internal constant [23 x i8] c\"Error division by zero\\00\"");
    buffer.emit("define void @check_division(i32) {");
    buffer.emit("%valid = icmp eq i32 %0, 0");
    buffer.emit("br i1 %valid, label %ILLEGAL, label %LEGAL");
    buffer.emit("ILLEGAL:");
    buffer.emit("call void @print(i8* getelementptr([23 x i8], [23 x i8]* @.DIV_BY_ZERO_ERROR, i32 0, i32 0))");
    buffer.emit("call void @exit(i32 0)");
    buffer.emit("ret void");
    buffer.emit("LEGAL:");
    buffer.emit("ret void");
    buffer.emit("}");
}

string LLVM_code_generator::generate_load_var(string rbp, int offset) {
    string reg = buffer.freshVar();
    string var_ptr = buffer.freshVar();
    buffer.emit(var_ptr + " = getelementptr i32, i32* " + rbp + ", i32 " + std::to_string(offset));
    buffer.emit(reg + " = load i32, i32* " + var_ptr);
    return reg;
}

void LLVM_code_generator::generate_store_var(string rbp, int offset, string reg) {
    string var_ptr = buffer.freshVar();
    buffer.emit(var_ptr + " = getelementptr i32, i32* " + rbp + ", i32 " + std::to_string(offset));
    buffer.emit("store i32 " + reg + ", i32* " + var_ptr);
}

void LLVM_code_generator::binop_code(Exp *res, const Exp &operand1, const Exp &operand2, const string &op) {
    res->erekhBituy = buffer.freshVar();
    string op_text;
    if (op == "+") {
        op_text = "add";
    } else if (op == "-") {
        op_text = "sub";
    } else if (op == "*") {
        op_text = "mul";
    } else {
        if (res->type == ast::BuiltInType::INT) {
            op_text = "sdiv";
        } else {
            op_text = "udiv";
        }
    }
    if (op == "/") {
        buffer.emit("call void @check_division(i32 " + operand2.erekhBituy + ")");
        buffer.emit(res->erekhBituy + " = " + op_text + " i32 " + operand1.erekhBituy + ", " + operand2.erekhBituy);
    } else {
        buffer.emit(res->erekhBituy + " = " + op_text + " i32 " + operand1.erekhBituy + ", " + operand2.erekhBituy);
        if (res->type == BYTE) {
            string old_reg = res->erekhBituy;
            res->erekhBituy = buffer.freshVar();
            buffer.emit(res->erekhBituy + " = and i32 255, " + old_reg);
        }
    }
}

void LLVM_code_generator::relop_code(Exp *res, const Exp *operand1, const Exp *operand2, const string &op) {
    res->erekhBituy = buffer.freshVar();
    string op_text;
    if (op == "==") {
        op_text = "eq";
    } else if (op == "!=") {
        op_text = "ne";
    } else if (op == ">") {
        op_text = "sgt";
    } else if (op == ">=") {
        op_text = "sge";
    } else if (op == "<") {
        op_text = "slt";
    } else {
        op_text = "sle";
    }

    buffer.emit(res->erekhBituy + " = icmp " + op_text + " i32 " + operand1->erekhBituy + ", " + operand2->erekhBituy);
    buffer.emit("br i1 " + res->erekhBituy + ", label @, label @");
}

void LLVM_code_generator::assign_code(Exp *exp, int offset, bool is_bool) {
    if (is_bool) {
        Exp *new_exp = bool_exp(exp);
        generate_store_var(tables.back()->rbp, offset, new_exp->erekhBituy);
    } else {
        generate_store_var(tables.current_scope()->rbp, offset, exp->erekhBituy);
    }
}

/*
// Generates llvm code of initializing a variable in FanC
void LLVM_code_generator::InitializeIntVariableConvertor(Num numExpression, int value)
{
    // numExpression = "int x = 5";
    string registerName = buff.freshVar(); // t0  t1 t2
    NumVariable newVariable;
    newVariable.variable_name = numExpression.erekhBituy; // newVariable.variable_name = x
    if (numExpression.erekhMispar != NULL)
    {
        newVariable.variable_value = numExpression.erekhMispar;
    }
    else // int x;
    {
        newVariable.variable_value = 0; // default value, approppiate case for int x;
    }
    variablesStack
}

void LLVM_code_generator::InitializeBoolVariableConvertor(Exp* boolExpression, bool value)
{
    // bool flag = false;
    string varName = buff.freshLabel();
    varName = boolExpression->erekhBituy;
    if (boolExpression->erekhMispar != NULL)
    {
        value = boolExpression->erekhMispar;
    }
    else
    {
        value = false; // default value, approppiate case for bool flag;
    }
}

void LLVM_code_generator::storeVariable(string& basePointer, int offset, string& registerName)
{

    string registerPtr = buff.freshVar();

} */
