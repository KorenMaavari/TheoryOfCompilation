#ifndef _GENERATOR_HPP_
#define _GENERATOR_HPP_
#include "hw5-supplied/output.hpp"
#include <vector>
#include "outputAndSymbolTable.hpp"
using namespace std;
using namespace ast;

using std::string;




//template <typename T>
class LLVM_code_generator {
    public:
        void globalFunctions();
        string generate_load_var(string rbp, int offset);
        void generate_store_var(string rbp, int offset, string reg);
            void binop_code(Exp* res, const Exp& operand1, const Exp& operand2, const string& op);
         void relop_code(Exp* res, const Exp* operand1, const Exp* operand2, const string& op);
            void bool_eval_code(Exp* res, const Exp* operand1, const Exp* operand2, const string& op, const string& label);
        void assign_code(Exp* exp, int offset, bool is_bool);





        /*
         output::CodeBuffer buff;
        vector<T> variablesStack;
        void InitializeIntVariableConvertor(Num numExpression);
        void InitializeBoolVariableConvertor(Bool boolExpression);
        void LoadVariableFromStack(Binop expression)
        void StoreVariableInStack()
        //void storeVariable(string& basePointer, int offset, string& registerName);
        void AccessToVariableConvertor();
        void ArithmeticExpressionConvertor();
        void NumericOverflowConvertor();
        void BooleanExpressionConvertor();
        void FunctionCallConvertor();
        void IfConvertor();
        void WhileConvertor();
        void BreakConvertor();
        void ContinueConvertor();
        void ReturnConvertor();
        void LibraryFunctionsConvertor();
        void PrintiConvertor(); // Mind the i
        void PrintConvertor(); // Mind the lack of i
        void ErrorConvertor(); // not sure */
};

/* class NumVariable{
    public:
        string variable_name;
        int variable_value;
        string registerName;
};

class BoolVariable{
    public:
        string variable_name;
        bool variable_value;
        string registerName;
};


template <typename T>
// Generates llvm code of initializing a variable in FanC
void LLVM_code_generator<T>::InitializeIntVariableConvertor(Num numExpression)
{
    // numExpression = "int x = 5"; int x = 4 + y;
    NumVariable newNumVariable;
    newNumVariable.registerName = buff.freshVar(); // t0  t1 t2 
    newNumVariable.variable_name = numExpression.erekhBituy; // newVariable.variable_name = x
    if (numExpression.erekhMispar != NULL)
    {
        newNumVariable.variable_value = numExpression.erekhMispar;
    }
    else // int x;
    {
        newNumVariable.variable_value = 0; // default value, approppiate case for int x;
    }
    variablesStack.push_back(newNumVariable);
    buff.emit(newNumVariable.registerName + " = add i32 " +  to_string(variable_value) + ", 0");
}

template <typename T>
void LLVM_code_generator<T>::InitializeBoolVariableConvertor(Bool boolExpression)
{
    // bool flag = false;
    BoolVariable newBoolVariable;
    newBoolVariable.registerName = buff.freshLabel(); // registerName = %t2
    newBoolVariable.variable_name = boolExpression->erekhBituy; // newBoolVariable.variable_name = flag
    if (boolExpression->erekhMispar != NULL)
    {
        newBoolVariable.variable_value = boolExpression->erekhMispar; // newBoolVariable.variable_value = 0
    }
    else
    {
        newBoolVariable.variable_value = false; // default value, approppiate case for bool flag;
    }
    variablesStack.push_back(newBoolVariable); // push([variable_name, variable_value])
    buff.emit(newBoolVariable.registerName + " = add i32 " +  to_string(variable_value) + ", 0"); // buff.emit(%t2 = add i32 0, 0)
}

template <typename T>
void LLVM_code_generator<T>::LoadVariableFromStack(Binop expression)
{
    string newRegisterName;
    // int y = x + 3;
    // %y = load i32, i32* %x --> y = x
    // %y = add i32 %y, 3
    for (int i = 0;variablesStack.size();i++)
    {
        if (variablesStack<NumVariable>[i]->variable_name == expression.leftHandSide.erekhBituy) // we found the FanC variable x in the stack
        {
            newRegisterName = buff.freshVar();
            buff.emit(newRegisterName + " = load i32, i32* " + variablesStack[i].registerName); 
            buff.emit(newRegisterName + " = add i32 " +  newRegisterName + ", " + expression.rightHandSide); // buff.emit(%t2 = add i32 0, 0)
        }
    }
}


void LLVM_code_generator::storeVariable(string& basePointer, int offset, string& registerName)
{
    
    string registerPtr = buff.freshVar();

}
*/

#endif