#ifndef SYMBOLTABLEMANAGER_HPP
#define SYMBOLTABLEMANAGER_HPP

#include <string> // Standard library for using std::string type, representing textual data.
#include <unordered_map> // Provides std::unordered_map for efficient key-value storage.
#include <vector> // Provides std::vector, a dynamic array for storing elements in a sequence.
#include "nodes.hpp" // Custom header file, defines AST (Abstract Syntax Tree) structures.
#include "output.hpp" // Custom header file, defines output utilities like ScopePrinter.

/// Represents a single symbol in the symbol table.
/// Used to store information about variables and functions.
struct Symbol {
    std::string id; /// The identifier (name) of the symbol.
    ast::BuiltInType type; /// The type of the symbol, e.g., INT, VOID, etc.
    int offset; /// The offset in memory or stack for the variable.
    bool isFunction; /// Indicates if the symbol represents a function.
    std::vector<ast::BuiltInType> paramTypes; /// Types of function parameters (empty for variables).
};

/// Handles semantic analysis by managing symbol tables and scope information.
class SemanticContext {
    output::ScopePrinter printer; /// A utility for pretty-printing scope information.
    std::vector<std::unordered_map<std::string, Symbol>> scopes; /// Stack of symbol tables, one per scope.
    bool insideLoop = false; /// Tracks whether the current code is inside a loop.
    bool mainDefined = false; /// Tracks whether the 'main' function is defined.

public:
    /// Constructor: Initializes the global scope and registers library functions.
    SemanticContext();

    /// Pushes a new scope onto the stack.
    void enterScope();

    /// Pops the current scope from the stack.
    void exitScope();

    /// Adds a variable to the current scope. Throws error if variable already exists.
    void declareVariable(const std::string &id, ast::BuiltInType type, int offset, int line);

    /// Adds a function to the current scope. Throws error if function already exists.
    void declareFunction(const std::string &id, ast::BuiltInType returnType, const std::vector<ast::BuiltInType> &params, int line);

    /// Searches for a symbol in all scopes, starting from the innermost.
    /// Throws an error if the symbol is not found.
    Symbol *lookup(const std::string &id, int line);

    /// Marks the 'main' function as defined.
    void markMainDefined();

    /// Checks whether the 'main' function is defined.
    bool isMainDefined() const;

    /// Sets the loop tracking flag.
    void setInsideLoop(bool status);

    /// Checks whether the code is inside a loop.
    bool isInsideLoop() const;

    /// Provides access to the scope printer for diagnostics.
    output::ScopePrinter &getPrinter();
};

#endif // SYMBOLTABLEMANAGER_HPP
