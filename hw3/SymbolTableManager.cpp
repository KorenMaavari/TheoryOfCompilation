#include "SymbolTableManager.hpp" // Include header to implement its declared functions.

/// Constructor: Initializes the SemanticContext object.
/// Sets up the global scope and adds built-in library functions like 'print' and 'printi'.
SemanticContext::SemanticContext() {
    scopes.emplace_back(); // Create a global (root) scope.
    printer.emitFunc("print", ast::BuiltInType::VOID, {ast::BuiltInType::STRING}); // Built-in function for printing strings.
    printer.emitFunc("printi", ast::BuiltInType::VOID, {ast::BuiltInType::INT}); // Built-in function for printing integers.
}

/// Enters a new scope by pushing an empty symbol table onto the stack.
void SemanticContext::enterScope() {
    scopes.emplace_back(); // Add a new scope to the stack.
    printer.beginScope(); // Log the beginning of a new scope for debugging.
}

/// Exits the current scope by popping the top symbol table from the stack.
void SemanticContext::exitScope() {
    printer.endScope(); // Log the end of the scope for debugging.
    scopes.pop_back(); // Remove the top scope.
}

/// Declares a variable in the current scope.
/// Throws an error if the variable is already declared.
void SemanticContext::declareVariable(const std::string &id, ast::BuiltInType type, int offset, int line) {
    if (scopes.back().count(id)) output::errorDef(line, id); // Error if variable already exists in this scope.
    scopes.back()[id] = {id, type, offset, false, {}}; // Add the variable to the current scope.
    printer.emitVar(id, type, offset); // Log the variable declaration for debugging.
}

/// Declares a function in the current scope.
/// Throws an error if the function is already declared.
void SemanticContext::declareFunction(const std::string &id, ast::BuiltInType returnType, const std::vector<ast::BuiltInType> &params, int line) {
    if (scopes.back().count(id)) output::errorDef(line, id); // Error if function already exists in this scope.
    scopes.back()[id] = {id, returnType, 0, true, params}; // Add the function to the current scope.
    printer.emitFunc(id, returnType, params); // Log the function declaration for debugging.
}

/// Looks up a symbol in all scopes, starting from the innermost.
/// Throws an error if the symbol is not found.
Symbol *SemanticContext::lookup(const std::string &id, int line) {
    for (auto it = scopes.rbegin(); it != scopes.rend(); ++it) { // Traverse scopes from inner to outer.
        if (it->count(id)) return &it->at(id); // Return the symbol if found.
    }
    output::errorUndef(line, id); // Throw an error if the symbol is undefined.
    return nullptr; // This line is unreachable but required by the compiler.
}

/// Marks the 'main' function as defined.
void SemanticContext::markMainDefined() { mainDefined = true; }

/// Checks if the 'main' function is defined.
bool SemanticContext::isMainDefined() const { return mainDefined; }

/// Sets the insideLoop flag to indicate whether the code is within a loop.
void SemanticContext::setInsideLoop(bool status) { insideLoop = status; }

/// Checks if the code is inside a loop.
bool SemanticContext::isInsideLoop() const { return insideLoop; }

/// Provides access to the ScopePrinter for debugging.
output::ScopePrinter &SemanticContext::getPrinter() { return printer; }
