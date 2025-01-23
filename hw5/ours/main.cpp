#include <iostream>
#include "nodes.hpp"
#include "output.hpp"

// Extern from the bison-generated parser
extern int yyparse();

extern std::shared_ptr<ast::Node> program;

int main() {
    try {
        yyparse(); // Call the parser function
        output::ScopePrinter scopePrinter;
        program->accept(scopePrinter);
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
