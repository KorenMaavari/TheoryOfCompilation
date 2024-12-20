#ifndef SYMBOLTABLEMANAGER_HPP
#define SYMBOLTABLEMANAGER_HPP

#include <string>
#include <unordered_map>
#include <vector>
#include "nodes.hpp"
#include "output.hpp" // For ScopePrinter

struct Symbol {
    std::string id;
    ast::BuiltInType type;
    int offset;
    bool isFunction;
    std::vector<ast::BuiltInType> paramTypes;
};

class SemanticContext {
    output::ScopePrinter printer;
    std::vector<std::unordered_map<std::string, Symbol>> scopes;
    bool insideLoop = false;
    bool mainDefined = false;

public:
    SemanticContext();
    void enterScope();
    void exitScope();
    void declareVariable(const std::string &id, ast::BuiltInType type, int offset, int line);
    void declareFunction(const std::string &id, ast::BuiltInType returnType, const std::vector<ast::BuiltInType> &params, int line);
    Symbol *lookup(const std::string &id, int line);
    void markMainDefined();
    bool isMainDefined() const;
    void setInsideLoop(bool status);
    bool isInsideLoop() const;
    output::ScopePrinter &getPrinter();
};

#endif // SYMBOLTABLEMANAGER_HPP
