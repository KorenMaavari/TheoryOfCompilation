#include "SymbolTableManager.hpp" // Semantic context and symbol table management.
#include "nodes.hpp" // AST structures for representing the program.
#include "visitor.hpp" // Base visitor interface for traversing the AST.
#include "output.hpp" // Error reporting and scope debugging utilities.
#include <iostream> // Provides std::cout for outputting results.

/// Checks if one type is assignable to another.
/// For example, BYTE is assignable to INT, but not vice versa.
bool isAssignable(ast::BuiltInType target, ast::BuiltInType source) {
    if (target == source) return true; // Types are identical.
    if (target == ast::BuiltInType::INT && source == ast::BuiltInType::BYTE) return true; // BYTE can be assigned to INT.
    return false; // All other cases are invalid.
}

/// A concrete visitor for performing semantic analysis on the AST.
class SemanticVisitor : public Visitor {
    SemanticContext context; /// The semantic context managing scopes and symbols.
    int currentOffset = 0; /// Tracks the offset for variable storage in the current scope.
    std::unordered_map<void *, ast::BuiltInType> nodeTypes; /// Maps AST nodes to their computed types.

public:
    /// Default constructor.
    SemanticVisitor() = default;

    /// Destructor: Ensures 'main' is defined and outputs the final scope structure.
    ~SemanticVisitor() {
        if (!context.isMainDefined()) output::errorMainMissing(); // Error if 'main' is not defined.
        std::cout << context.getPrinter(); // Print the scope structure.
    }

    /// Processes numeric literals.
    void visit(ast::Num &node) override {
        nodeTypes[&node] = ast::BuiltInType::INT; // Assign INT type to the node.
    }

    /// Processes byte literals.
    void visit(ast::NumB &node) override {
        if (node.value > 255) output::errorByteTooLarge(node.line, node.value); // Error if value exceeds BYTE range.
        nodeTypes[&node] = ast::BuiltInType::BYTE; // Assign BYTE type to the node.
    }

    /// Processes string literals.
    void visit(ast::String &node) override {
        nodeTypes[&node] = ast::BuiltInType::STRING; // Assign STRING type to the node.
    }

    /// Processes boolean literals (e.g., true, false).
    void visit(ast::Bool &node) override {
        // Assign BOOL type to the node.
        nodeTypes[&node] = ast::BuiltInType::BOOL;
    }

    /// Processes identifier nodes by resolving their types from the symbol table.
    void visit(ast::ID &node) override {
        auto *symbol = context.lookup(node.value, node.line); // Look up the symbol.
        if (symbol->isFunction) output::errorDefAsVar(node.line, node.value); // Error if the ID refers to a function.
        nodeTypes[&node] = symbol->type; // Assign the symbol's type to the node.
    }

    /// Processes binary operations (e.g., addition, subtraction).
    void visit(ast::BinOp &node) override {
        // Visit both operands to determine their types.
        node.left->accept(*this);
        node.right->accept(*this);

        // Check if both operands are INT (valid types for binary operations).
        if (nodeTypes[node.left.get()] != ast::BuiltInType::INT || nodeTypes[node.right.get()] != ast::BuiltInType::INT) {
            output::errorMismatch(node.line); // Emit a mismatch error if types are incompatible.
        }

        // Assign INT as the result type of the binary operation.
        nodeTypes[&node] = ast::BuiltInType::INT;
    }

    /// Processes relational operations (e.g., <, >, ==).
    void visit(ast::RelOp &node) override {
        // Visit both operands to determine their types.
        node.left->accept(*this);
        node.right->accept(*this);

        // Ensure both operands are of the same type (usually INT or BOOL for relational operators).
        if (nodeTypes[node.left.get()] != nodeTypes[node.right.get()]) {
            output::errorMismatch(node.line); // Emit a mismatch error if types are incompatible.
        }

        // Assign BOOL as the result type of the relational operation.
        nodeTypes[&node] = ast::BuiltInType::BOOL;
    }

    /// Processes logical NOT operations (e.g., !).
    void visit(ast::Not &node) override {
        // Visit the operand to determine its type.
        node.exp->accept(*this);

        // Ensure the operand is BOOL (logical NOT operates on boolean values).
        if (nodeTypes[node.exp.get()] != ast::BuiltInType::BOOL) {
            output::errorMismatch(node.line); // Emit a mismatch error if the operand is not BOOL.
        }

        // Assign BOOL as the result type of the NOT operation.
        nodeTypes[&node] = ast::BuiltInType::BOOL;
    }

    /// Processes logical AND operations (e.g., &&).
    void visit(ast::And &node) override {
        // Visit both operands to determine their types.
        node.left->accept(*this);
        node.right->accept(*this);

        // Ensure both operands are BOOL (logical AND operates on boolean values).
        if (nodeTypes[node.left.get()] != ast::BuiltInType::BOOL || nodeTypes[node.right.get()] != ast::BuiltInType::BOOL) {
            output::errorMismatch(node.line); // Emit a mismatch error if operands are not BOOL.
        }

        // Assign BOOL as the result type of the AND operation.
        nodeTypes[&node] = ast::BuiltInType::BOOL;
    }

    /// Processes logical OR operations (e.g., ||).
    void visit(ast::Or &node) override {
        // Visit both operands to determine their types.
        node.left->accept(*this);
        node.right->accept(*this);

        // Ensure both operands are BOOL (logical OR operates on boolean values).
        if (nodeTypes[node.left.get()] != ast::BuiltInType::BOOL || nodeTypes[node.right.get()] != ast::BuiltInType::BOOL) {
            output::errorMismatch(node.line); // Emit a mismatch error if operands are not BOOL.
        }

        // Assign BOOL as the result type of the OR operation.
        nodeTypes[&node] = ast::BuiltInType::BOOL;
    }

    /// Processes type nodes (e.g., INT, BOOL).
    void visit(ast::Type &node) override {
        // Directly assign the type specified in the node.
        nodeTypes[&node] = node.type;
    }

    /// Processes type casting operations (e.g., (int) expression).
    void visit(ast::Cast &node) override {
        // Visit the expression being cast.
        node.exp->accept(*this);

        // Get the target type as BuiltInType.
        ast::BuiltInType targetType = node.target_type->type;

        // Ensure the cast is valid.
        if (!isAssignable(targetType, nodeTypes[node.exp.get()])) {
            output::errorMismatch(node.line); // Emit mismatch error for invalid casts.
        }

        // Assign the target type as the result type of the cast.
        nodeTypes[&node] = targetType;
    }

    /// Processes a list of expressions (e.g., arguments to a function call).
    void visit(ast::ExpList &node) override {
        for (auto &exp : node.exps) {
            exp->accept(*this); // Visit each expression in the list.
        }
        // No specific type is assigned to the list itself.
    }

    /// Processes function call expressions (e.g., func(a, b)).
    void visit(ast::Call &node) override {
        auto *symbol = context.lookup(node.func_id->value, node.line); // Lookup the function in the symbol table.

        if (!symbol->isFunction) {
            output::errorUndefFunc(node.line, node.func_id->value); // Emit an error if the ID does not refer to a function.
        }

        // Dereference the ExpList to access the vector of expressions.
        auto &args = node.args->exps;

        // Ensure the number of arguments matches the function signature.
        if (args.size() != symbol->paramTypes.size()) {
            output::errorMismatch(node.line); // Emit mismatch error if argument count is incorrect.
        }

        // Visit each argument and ensure its type matches the function parameter type.
        for (size_t i = 0; i < args.size(); ++i) {
            args[i]->accept(*this);
            if (nodeTypes[args[i].get()] != symbol->paramTypes[i]) {
                output::errorMismatch(node.line); // Emit mismatch error if argument type is incorrect.
            }
        }

        // Assign the return type of the function as the result type of the call.
        nodeTypes[&node] = symbol->type;
    }

    /// Processes a list of statements (e.g., the body of a function or control structure).
    void visit(ast::Statements &node) override {
        for (auto &statement : node.statements) {
            statement->accept(*this); // Visit each statement in the list.
        }
        // No specific type is assigned to a block of statements.
    }

    /// Processes variable declarations.
    void visit(ast::VarDecl &node) override {
        // Visit the type of the variable to ensure it's valid.
        node.type->accept(*this);

        // If the variable has an initialization expression, visit and check it.
        if (node.init_exp) {
            node.init_exp->accept(*this);

            // Ensure the type of the initialization expression matches the variable type.
            if (!isAssignable(nodeTypes[node.type.get()], nodeTypes[node.init_exp.get()])) {
                output::errorMismatch(node.line); // Emit a mismatch error if types are incompatible.
            }
        }

        // Declare the variable in the current scope.
        context.declareVariable(node.id->value, nodeTypes[node.type.get()], currentOffset++, node.line);
    }

    /// Processes assignment statements.
    void visit(ast::Assign &node) override {
        // Visit the variable and the expression being assigned.
        node.id->accept(*this);
        node.exp->accept(*this);

        // Ensure the types are compatible for assignment.
        if (!isAssignable(nodeTypes[node.id.get()], nodeTypes[node.exp.get()])) {
            output::errorMismatch(node.line); // Emit a mismatch error if types are incompatible.
        }
    }

    /// Processes formal parameters in function declarations.
    void visit(ast::Formal &node) override {
        // Visit the type of the formal parameter.
        node.type->accept(*this);

        // Declare the formal parameter as a variable in the current scope.
        context.declareVariable(node.id->value, nodeTypes[node.type.get()], -1, node.line);
    }

    /// Processes a list of formal parameters.
    void visit(ast::Formals &node) override {
        for (auto &formal : node.formals) {
            formal->accept(*this); // Visit each formal parameter.
        }
        // No specific type is assigned to the list of formals.
    }

    /// Processes function declarations.
    void visit(ast::FuncDecl &node) override {
        std::vector<ast::BuiltInType> paramTypes;

        // Gather parameter types and visit each formal.
        for (auto &formal : node.formals->formals) {
            formal->accept(*this);
            paramTypes.push_back(nodeTypes[formal->type.get()]);
        }

        // Check for 'main' function validity.
        if (node.id->value == "main") {
            if (context.isMainDefined() || !paramTypes.empty() || nodeTypes[node.return_type.get()] != ast::BuiltInType::VOID) {
                output::errorMainMissing();
            }
            context.markMainDefined();
        }

        // Declare the function in the symbol table.
        context.declareFunction(node.id->value, nodeTypes[node.return_type.get()], paramTypes, node.line);

        // Set the current function's return type.
        context.setCurrentFunctionReturnType(nodeTypes[node.return_type.get()]);

        // Enter the function's scope.
        context.enterScope();

        // Add formal parameters to the scope.
        int paramOffset = -1;
        for (auto &formal : node.formals->formals) {
            context.declareVariable(formal->id->value, nodeTypes[formal->type.get()], paramOffset--, formal->line);
        }

        // Visit the function body.
        node.body->accept(*this);

        // Exit the function's scope.
        context.exitScope();
    }

    /// Processes a list of functions.
    void visit(ast::Funcs &node) override {
        for (auto &func : node.funcs) {
            func->accept(*this); // Visit each function.
        }
        // No specific type is assigned to the list of functions.
    }

    /// Processes a break statement.
    void visit(ast::Break &node) override {
        // Ensure the break statement is inside a loop.
        if (!context.isInsideLoop()) {
            output::errorUnexpectedBreak(node.line); // Emit an error if break is outside a loop.
        }
    }

    /// Processes a continue statement.
    void visit(ast::Continue &node) override {
        // Ensure the continue statement is inside a loop.
        if (!context.isInsideLoop()) {
            output::errorUnexpectedContinue(node.line); // Emit an error if continue is outside a loop.
        }
    }

    /// Processes a return statement.
    void visit(ast::Return &node) override {
        if (node.exp) {
            // Visit the return expression if present.
            node.exp->accept(*this);

            // Check if the return type matches the enclosing function's return type.
            // (Assume `context.getCurrentFunctionReturnType()` gives the expected return type.)
            if (!isAssignable(context.getCurrentFunctionReturnType(), nodeTypes[node.exp.get()])) {
                output::errorMismatch(node.line); // Emit a mismatch error for invalid return types.
            }
        } else {
            // Ensure the function is void if no expression is returned.
            if (context.getCurrentFunctionReturnType() != ast::BuiltInType::VOID) {
                output::errorMismatch(node.line); // Emit a mismatch error for missing return value.
            }
        }
    }

    /// Processes an if statement.
    void visit(ast::If &node) override {
        // Visit the condition expression.
        node.condition->accept(*this);

        // Ensure the condition is of type BOOL.
        if (nodeTypes[node.condition.get()] != ast::BuiltInType::BOOL) {
            output::errorMismatch(node.line); // Emit a mismatch error if the condition is not BOOL.
        }

        // Visit the 'then' branch inside a new scope.
        context.enterScope();
        node.then->accept(*this);
        context.exitScope();

        // Visit the 'else' branch if present, inside a new scope.
        if (node.otherwise) {
            context.enterScope();
            node.otherwise->accept(*this);
            context.exitScope();
        }
    }

    /// Processes a while loop.
    void visit(ast::While &node) override {
        // Visit the condition expression.
        node.condition->accept(*this);

        // Ensure the condition is of type BOOL.
        if (nodeTypes[node.condition.get()] != ast::BuiltInType::BOOL) {
            output::errorMismatch(node.line); // Emit a mismatch error if the condition is not BOOL.
        }

        // Set the loop context to true and visit the body inside a new scope.
        context.setInsideLoop(true);
        context.enterScope();
        node.body->accept(*this);
        context.exitScope();
        context.setInsideLoop(false); // Reset the loop context.
    }

};
