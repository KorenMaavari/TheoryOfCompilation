#include <iostream>

// Extern from the bison-generated parser
extern int yyparse();

int main() {
    try {
        yyparse(); // Call the parser function
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
