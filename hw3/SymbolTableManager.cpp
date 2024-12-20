#include "SymbolTableManager.hpp"

SemanticContext::SemanticContext() {
    scopes.emplace_back();
    printer.emitFunc("print", ast::BuiltInType::VOID, {ast::BuiltInType::STRING});
    printer.emitFunc("printi", ast::BuiltInType::VOID, {ast::BuiltInType::INT});
}

void SemanticContext::enterScope() {
    scopes.emplace_back();
    printer.beginScope();
}

void SemanticContext::exitScope() {
    printer.endScope();
    scopes.pop_back();
}

void SemanticContext::declareVariable(const std::string &id, ast::BuiltInType type, int offset, int line) {
    if (scopes.back().count(id)) output::errorDef(line, id);
    scopes.back()[id] = {id, type, offset, false, {}};
    printer.emitVar(id, type, offset);
}

void SemanticContext::declareFunction(const std::string &id, ast::BuiltInType returnType, const std::vector<ast::BuiltInType> &params, int line) {
    if (scopes.back().count(id)) output::errorDef(line, id);
    scopes.back()[id] = {id, returnType, 0, true, params};
    printer.emitFunc(id, returnType, params);
}

Symbol *SemanticContext::lookup(const std::string &id, int line) {
    for (auto it = scopes.rbegin(); it != scopes.rend(); ++it) {
        if (it->count(id)) return &it->at(id);
    }
    output::errorUndef(line, id);
    return nullptr; // Unreachable
}

void SemanticContext::markMainDefined() { mainDefined = true; }
bool SemanticContext::isMainDefined() const { return mainDefined; }
void SemanticContext::setInsideLoop(bool status) { insideLoop = status; }
bool SemanticContext::isInsideLoop() const { return insideLoop; }
output::ScopePrinter &SemanticContext::getPrinter() { return printer; }
