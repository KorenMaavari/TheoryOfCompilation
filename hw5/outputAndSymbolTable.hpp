#ifndef outputAndSymbolTable_hpp
#define outputAndSymbolTable_hpp

#include <vector>
#include <iostream>
#include <stack>
#include <string>
#include <sstream>
#include "visitor.hpp"
#include <sstream>
#include "nodes.hpp"

namespace outputAndSymbolTable {
    /* Error handling functions */

    void errorLex(int lineno);

    void errorSyn(int lineno);

    void errorUndef(int lineno, const std::string &id);

    void errorDefAsFunc(int lineno, const std::string &id);

    void errorUndefFunc(int lineno, const std::string &id);

    void errorDefAsVar(int lineno, const std::string &id);

    void errorDef(int lineno, const std::string &id);

    void errorPrototypeMismatch(int lineno, const std::string &id, std::vector<std::string> &paramTypes);

    void errorMismatch(int lineno);

    void errorUnexpectedBreak(int lineno);

    void errorUnexpectedContinue(int lineno);

    void errorMainMissing();

    void errorByteTooLarge(int lineno, int value);
    
    extern std::vector<std::shared_ptr<ast::Node>> mishtaneMisgeret;
    extern std::vector<int> MisparMishtaneNokhehi;
    extern std::vector<std::shared_ptr<ast::FuncDecl>> HatsharatMishtaneGlobali;
    extern std::vector<std::shared_ptr<ast::Node>> KriatMishtaneGlobali;

    void enrtyFrame();
    void exitFrame();
    extern ast::BuiltInType returnType;
    extern int moneMishtanim;
    extern bool zoKria;
    extern bool shimush;

    /* ScopePrinter class
     * This class is used to print scopes in a human-readable format.
     */
    class ScopePrinter : public Visitor {
    private:

        int hafsakaVeHemshekhHukiyim = 0;

        std::stringstream globalsBuffer;
        std::stringstream buffer;
        int indentLevel;

    public:
        ScopePrinter();

        void beginScope();

        void endScope();

        void emitVar(const std::string &id, const ast::BuiltInType &type, int offset);

        void emitFunc(const std::string &id, const ast::BuiltInType &returnType,
                      const std::vector<ast::BuiltInType> &paramTypes);

        friend std::ostream &operator<<(std::ostream &os, const ScopePrinter &printer);

        std::string indent() const;

        void visit(ast::Funcs &node) override;
        
        void visit(ast::FuncDecl &node) override;
        
        void visit(ast::Formals &node) override;
        
        void visit(ast::Formal &node) override;
        
        void visit(ast::Assign &node) override;
        
        void visit(ast::VarDecl &node) override;
        
        void visit(ast::While &node) override;
        
        void visit(ast::If &node) override;
        
        void visit(ast::Return &node) override;
        
        void visit(ast::Continue &node) override;
        
        void visit(ast::Break &node) override;
        
        void visit(ast::Statements &node) override;
        
        void visit(ast::Call &node) override;
        
        void visit(ast::ExpList &node) override;
        
        void visit(ast::Cast &node) override;
        
        void visit(ast::Type &node) override;
        
        void visit(ast::Or &node) override;
        
        void visit(ast::And &node) override;
        
        void visit(ast::Not &node) override;
        
        void visit(ast::RelOp &node) override;
        
        void visit(ast::BinOp &node) override;
        
        void visit(ast::ID &node) override;
        
        void visit(ast::Bool &node) override;
        
        void visit(ast::String &node) override;
        
        void visit(ast::NumB &node) override;
        
        void visit(ast::Num &node) override;

    };
}

#endif //outputAndSymbolTable_hpp
