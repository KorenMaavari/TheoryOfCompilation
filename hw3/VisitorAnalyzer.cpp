#include "SymbolTableManager.hpp"
#include "nodes.hpp"
#include "visitor.hpp"
#include "output.hpp"
#include <iostream>

bool isAssignable(ast::BuiltInType target, ast::BuiltInType source) {
    if (target == source) return true;
    if (target == ast::BuiltInType::INT && source == ast::BuiltInType::BYTE) return true;
    return false;
}

class SemanticVisitor : public Visitor {
    SemanticContext context;
    int currentOffset = 0;
    std::unordered_map<void *, ast::BuiltInType> nodeTypes;

public:
    SemanticVisitor() = default;

    ~SemanticVisitor() {
        if (!context.isMainDefined()) output::errorMainMissing();
        std::cout << context.getPrinter();
    }

    void visit(ast::Num &node) override {
        nodeTypes[&node] = ast::BuiltInType::INT;
    }

    void visit(ast::NumB &node) override {
        if (node.value > 255) output::errorByteTooLarge(node.line, node.value);
        nodeTypes[&node] = ast::BuiltInType::BYTE;
    }

    void visit(ast::String &node) override {
        nodeTypes[&node] = ast::BuiltInType::STRING;
    }

    void visit(ast::ID &node) override {
        auto *symbol = context.lookup(node.value, node.line);
        if (symbol->isFunction) output::errorDefAsVar(node.line, node.value);
        nodeTypes[&node] = symbol->type;
    }

    void visit(ast::VarDecl &node) override {
        // Assume `node.type->getType()` returns the BuiltInType (adjust based on available API)
        nodeTypes[node.type.get()] = nodeTypes[node.type.get()];
        if (node.init_exp) {
            node.init_exp->accept(*this);
            if (!isAssignable(nodeTypes[node.type.get()], nodeTypes[node.init_exp.get()])) {
                output::errorMismatch(node.line);
            }
        }
        context.declareVariable(node.id->value, nodeTypes[node.type.get()], currentOffset++, node.line);
    }

    void visit(ast::FuncDecl &node) override {
        std::vector<ast::BuiltInType> paramTypes;
        for (auto &formal : node.formals->formals) {
            paramTypes.push_back(formal->type->type);
        }
        if (node.id->value == "main") {
            if (context.isMainDefined() || !paramTypes.empty() || node.return_type->type != ast::BuiltInType::VOID) {
                output::errorMainMissing();
            }
            context.markMainDefined();
        }
        context.declareFunction(node.id->value, node.return_type->type, paramTypes, node.line);

        context.enterScope();
        int paramOffset = -1;
        for (auto &formal : node.formals->formals) {
            context.declareVariable(formal->id->value, formal->type->type, paramOffset--, formal->line);
        }
        node.body->accept(*this);
        context.exitScope();
    }

    void visit(ast::If &node) override {
        node.condition->accept(*this);
        if (nodeTypes[node.condition.get()] != ast::BuiltInType::BOOL) {
            output::errorMismatch(node.line);
        }
        context.enterScope();
        node.then->accept(*this);
        context.exitScope();
        if (node.otherwise) {
            context.enterScope();
            node.otherwise->accept(*this);
            context.exitScope();
        }
    }

    void visit(ast::While &node) override {
        node.condition->accept(*this);
        if (nodeTypes[node.condition.get()] != ast::BuiltInType::BOOL) {
            output::errorMismatch(node.line);
        }
        context.setInsideLoop(true);
        context.enterScope();
        node.body->accept(*this);
        context.exitScope();
        context.setInsideLoop(false);
    }

    void visit(ast::Break &node) override {
        if (!context.isInsideLoop()) output::errorUnexpectedBreak(node.line);
    }

    void visit(ast::Continue &node) override {
        if (!context.isInsideLoop()) output::errorUnexpectedContinue(node.line);
    }

    void visit(ast::Return &node) override {
        // Add logic for validating return types
    }
};
