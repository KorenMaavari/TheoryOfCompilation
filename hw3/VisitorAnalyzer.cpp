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

    /// Processes identifier nodes by resolving their types from the symbol table.
    void visit(ast::ID &node) override {
        auto *symbol = context.lookup(node.value, node.line); // Look up the symbol.
        if (symbol->isFunction) output::errorDefAsVar(node.line, node.value); // Error if the ID refers to a function.
        nodeTypes[&node] = symbol->type; // Assign the symbol's type to the node.
    }

    // Similar heavy comments can be applied to remaining visitor methods if needed.
};
