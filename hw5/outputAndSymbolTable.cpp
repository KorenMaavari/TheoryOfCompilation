#include <iostream>
 // The <iostream> header file is part of the C++ Standard Library. 
// It allows input-output stream operations, such as using `std::cout` to print output 
// or `std::cin` to read input from the user.

#include <algorithm>
 // The <algorithm> header provides standard algorithms that operate on containers.
// For example, `std::find_if`, which is used to search for elements in a range that satisfy a given condition.

#include "output.hpp"
 // This is a custom header file, which declares various functions, classes, and variables related 
// to semantic analysis, error reporting, and scope printing for a compiler's intermediate representation.

#define CAST_TO_FORMAL(mishtane) std::dynamic_pointer_cast < ast::Formal > (mishtane)
// This macro simplifies the repetitive task of casting a `std::shared_ptr<ast::Node>` 
// to `std::shared_ptr<ast::Formal>` using `std::dynamic_pointer_cast`. It takes a single argument, `mishtane` 
// (Hebrew for "variable").

#define CAST_TO_VARDECL(mishtane) std::dynamic_pointer_cast < ast::VarDecl > (mishtane)
// Similarly, this macro casts a `std::shared_ptr<ast::Node>` to a `std::shared_ptr<ast::VarDecl>`.

namespace output {
    // All the functionality defined here is encapsulated in the `output` namespace. 
    // This helps organize code and avoid name conflicts with other parts of the program.

    // Declaring a global vector to track variables within the current scope.
    std::vector < std::shared_ptr < ast::Node >> mishtaneMisgeret;
    // Each element in this vector represents a variable or formal parameter 
    // (wrapped as `std::shared_ptr<ast::Node>`), enabling efficient scope management.

    // Keeps track of the number of variables in each active scope.
    std::vector < int > MisparMishtaneNokhehi;
    // For each scope, the last element indicates how many variables are defined. 
    // This is used during scope cleanup to remove all variables introduced in a scope.

    // Stores globally defined functions, represented as `ast::FuncDecl`.
    std::vector < std::shared_ptr < ast::FuncDecl >> HatsharatMishtaneGlobali;

    // Tracks nodes associated with global variable usage or function calls.
    std::vector < std::shared_ptr < ast::Node >> KriatMishtaneGlobali;

    // Tracks the return type of the current function being analyzed.
    ast::BuiltInType returnType;

    // Boolean flag to indicate whether the current operation involves a function call.
    bool zoKria = false;

    // Boolean flag to indicate whether variables are in use or not.
    bool shimush = true;

    // Tracks the total number of variables declared, used for offset calculations.
    int moneMishtanim = 0;

    // ScopePrinter::visit(ast::Funcs&)
    // This method processes the `Funcs` node in the Abstract Syntax Tree (AST),
    // which represents a collection of function declarations.
    void ScopePrinter::visit(ast::Funcs & node) {
        // Initialize the built-in `print` function.
        std::shared_ptr < ast::ID > mezaheHadpasa = std::make_shared < ast::ID > ("print");
        // Creates a new identifier node for the function name "print".

        std::shared_ptr < ast::Type > tippusHahzaratHadpasa = std::make_shared < ast::Type > (ast::BuiltInType::VOID);
        // Specifies that the `print` function has a return type of `void`.

        std::shared_ptr < ast::ID > mezaheFormaliHadpasa = std::make_shared < ast::ID > ("var");
        // Represents the name of the single parameter for the `print` function.

        std::shared_ptr < ast::Type > tuppusFormaliHadpasa = std::make_shared < ast::Type > (ast::BuiltInType::STRING);
        // Indicates that the parameter type for `print` is a `string`.

        std::shared_ptr < ast::Formal > formalHadpasa = std::make_shared < ast::Formal > (mezaheFormaliHadpasa, tuppusFormaliHadpasa);
        // Creates a formal parameter node for the function `print`.

        std::shared_ptr < ast::Formals > formalsHadpasa = std::make_shared < ast::Formals > (formalHadpasa);
        // Wraps the single parameter into a list of formals.

        std::shared_ptr < ast::Statements > statementsHadpasa = std::make_shared < ast::Statements > ();
        // Initializes an empty statement block for the `print` function body (it has no implementation).

        std::shared_ptr < ast::FuncDecl > HatsharatHadpasa = std::make_shared < ast::FuncDecl > (
            mezaheHadpasa, tippusHahzaratHadpasa, formalsHadpasa, statementsHadpasa);
        // Constructs the entire function declaration node for `print`.

        // Check if the function `print` already exists in the global function list.
        if (std::find_if(HatsharatMishtaneGlobali.begin(), HatsharatMishtaneGlobali.end(),
                [ & ](const std::shared_ptr < ast::FuncDecl > & func) {
                    return func -> id -> value == "print";
                }) == HatsharatMishtaneGlobali.end()) {
            // If not found, add it to the global list.
            HatsharatMishtaneGlobali.push_back(HatsharatHadpasa);

            // Prepare the list of parameter types for logging purposes.
            std::vector < ast::BuiltInType > tippusim;
            for (auto haFormalHaNokhehi: formalsHadpasa -> formals) {
                tippusim.push_back(haFormalHaNokhehi -> type -> type);
            }

            // Emit the function signature for debugging or compilation output.
            emitFunc(mezaheHadpasa -> value, tippusHahzaratHadpasa -> type, tippusim);
        }

        // Repeat the same logic for the `printi` function, which accepts an `int` parameter.
        std::shared_ptr < ast::ID > mezaheHadpasaI = std::make_shared < ast::ID > ("printi");
        std::shared_ptr < ast::Type > tippusHahzaratHadpasaI = std::make_shared < ast::Type > (ast::BuiltInType::VOID);
        std::shared_ptr < ast::ID > mezaheFormaliHadpasaI = std::make_shared < ast::ID > ("var");
        std::shared_ptr < ast::Type > tuppusFormaliHadpasaI = std::make_shared < ast::Type > (ast::BuiltInType::INT);
        std::shared_ptr < ast::Formal > formalHadpasaI = std::make_shared < ast::Formal > (mezaheFormaliHadpasaI, tuppusFormaliHadpasaI);
        std::shared_ptr < ast::Formals > formalsHadpasaI = std::make_shared < ast::Formals > (formalHadpasaI);
        std::shared_ptr < ast::Statements > statementsHadpasaI = std::make_shared < ast::Statements > ();
        std::shared_ptr < ast::FuncDecl > HatsharatHadpasaI = std::make_shared < ast::FuncDecl > (
            mezaheHadpasaI, tippusHahzaratHadpasaI, formalsHadpasaI, statementsHadpasaI);

        if (std::find_if(HatsharatMishtaneGlobali.begin(), HatsharatMishtaneGlobali.end(),
                [ & ](const std::shared_ptr < ast::FuncDecl > & func) {
                    return func -> id -> value == "printi";
                }) == HatsharatMishtaneGlobali.end()) {
            HatsharatMishtaneGlobali.push_back(HatsharatHadpasaI);
            std::vector < ast::BuiltInType > tippusimI;
            for (auto haFormalHaNokhehi: formalsHadpasaI -> formals) {
                tippusimI.push_back(haFormalHaNokhehi -> type -> type);
            }
            emitFunc(mezaheHadpasaI -> value, tippusHahzaratHadpasaI -> type, tippusimI);
        }

        // Ensure the main function exists, matches the signature, and check for duplicates.
        bool mainKayyam = false;
        for (auto funktsiyya: node.funcs) {
            if (funktsiyya -> id -> value == "main") {
                mainKayyam = true;
                if (!funktsiyya -> formals -> formals.empty() || funktsiyya -> return_type -> type != ast::BuiltInType::VOID) {
                    errorMainMissing();
                }
            }
            for (auto existingFunc: HatsharatMishtaneGlobali) {
                if (existingFunc -> id -> value == funktsiyya -> id -> value) {
                    errorDef(funktsiyya -> id -> line, funktsiyya -> id -> value);
                }
            }

            HatsharatMishtaneGlobali.push_back(funktsiyya);
            std::vector < ast::BuiltInType > tippusim;
            for (auto haFormalHaNokhehi: funktsiyya -> formals -> formals) {
                tippusim.push_back(haFormalHaNokhehi -> type -> type);
            }
            emitFunc(funktsiyya -> id -> value, funktsiyya -> return_type -> type, tippusim);
        }

        // If no main function exists, report an error.
        if (!mainKayyam) {
            errorMainMissing();
        }

        // Visit each function in the list, processing its body and associated declarations.
        for (auto mehazrer: node.funcs) {
            moneMishtanim = 0;
            mehazrer -> accept( * this);
        }

        // Output the complete scope information for debugging purposes.
        std::cout << * this;
    }

    void ScopePrinter::visit(ast::FuncDecl & node) {
        // Start a new scope for the function body.
        beginScope();
        // Enter a new stack frame for the function.
        enrtyFrame();

        // Temporarily disable variable usage checks while processing the function name.
        shimush = false;
        node.id -> accept( * this); // Visit the function's identifier (name).
        shimush = true;

        // Visit the function's return type to validate its correctness.
        node.return_type -> accept( * this);

        // Store the function's return type for later type checking.
        returnType = node.return_type -> type;

        // Visit the list of formal parameters to check their validity and add them to the scope.
        node.formals -> accept( * this);

        // Visit the function's body (statements) to validate all expressions and logic.
        node.body -> accept( * this);

        // End the current scope since the function body has been fully processed.
        endScope();

        // Exit the current stack frame.
        exitFrame();
    }

    void ScopePrinter::visit(ast::Formals & node) {
        // Iterate over each formal parameter in the list.
        for (auto mehazrer = node.formals.begin(); mehazrer != node.formals.end(); ++mehazrer) {
            ( * mehazrer) -> accept( * this); // Validate each formal parameter individually.
        }
        // Reset the variable counter since formal parameters do not affect it globally.
        moneMishtanim = 0;
    }

    void ScopePrinter::visit(ast::Formal & node) {
        // Temporarily disable variable usage checks while processing the formal parameter's name.
        shimush = false;
        node.id -> accept( * this); // Visit the parameter's identifier (name).
        shimush = true;

        // Visit the parameter's type to validate its correctness.
        node.type -> accept( * this);

        // Ensure the parameter's name does not conflict with existing variables or parameters in the scope.
        for (auto mishtane: mishtaneMisgeret) {
            if (CAST_TO_FORMAL(mishtane) -> id -> value == node.id -> value) {
                errorDef(node.id -> line, node.id -> value); // Report a duplicate parameter error.
            }
        }

        // Ensure the parameter's name does not conflict with globally defined functions.
        for (auto funktsiyya: HatsharatMishtaneGlobali) {
            if (funktsiyya -> id -> value == node.id -> value) {
                errorDef(node.id -> line, node.id -> value); // Report a conflict with a function name.
            }
        }

        // Add the parameter to the current scope.
        mishtaneMisgeret.push_back(std::make_shared < ast::Formal > (node));

        // Update the count of variables in the current scope.
        MisparMishtaneNokhehi.back() ++;

        // Emit information about the parameter for debugging or compilation output.
        emitVar(node.id -> value, node.type -> type, --moneMishtanim);
    }

    void ScopePrinter::visit(ast::Assign & node) {
        // Validate the variable being assigned to.
        node.id -> accept( * this);

        // Validate the expression being assigned.
        node.exp -> accept( * this);

        // Flag to check if the variable exists in the current scope.
        bool loKayyam = true;

        bool zeBituy = false;

        // Iterate over all variables in the current scope to find the target variable.
        for (auto mishtane: mishtaneMisgeret) {
            if (CAST_TO_VARDECL(mishtane)) {
                if (CAST_TO_VARDECL(mishtane) -> id -> value == node.id -> value) {
                    loKayyam = false; // The variable exists in the current scope.

                    // Check for type compatibility between the variable and the expression.
                    if (!(CAST_TO_VARDECL(mishtane) -> type -> type == node.exp -> type) &&
                        !(node.exp -> type == ast::BuiltInType::BYTE &&
                            CAST_TO_VARDECL(mishtane) -> type -> type == ast::BuiltInType::INT)) {
                        errorMismatch(node.line); // Report a type mismatch error.
                    }
                }
            }

            if (CAST_TO_FORMAL(mishtane)) {
                if (CAST_TO_FORMAL(mishtane) -> id -> value == node.id -> value) {
                    loKayyam = false;
                    if (!(CAST_TO_FORMAL(mishtane) -> type -> type == node.exp -> type) &&
                        (!(node.exp -> type ==
                            ast::BuiltInType::BYTE &&
                            CAST_TO_FORMAL(mishtane) -> type -> type ==
                            ast::BuiltInType::INT)))
                        errorMismatch(node.line);
                }
            }
        }

        // If the variable does not exist, check global functions and report an error if necessary.
        if (loKayyam) {
            for (auto funktsiyya: HatsharatMishtaneGlobali) {
                if (funktsiyya -> id -> value == node.id -> value) {
                    errorDefAsFunc(node.line, funktsiyya -> id -> value);
                }
            }

            if (zeBituy)
                errorUndef(node.line,
                    std::dynamic_pointer_cast < ast::ID > (node.exp) -> value);

            errorUndef(node.line, node.id -> value); // Report that the variable is undefined.
        }
    }

    void ScopePrinter::visit(ast::VarDecl & node) {
        // Temporarily disable variable usage checks while processing the variable's name.
        shimush = false;
        node.id -> accept( * this); // Visit the variable's identifier (name).
        shimush = true;

        // Visit the variable's type to validate its correctness.
        node.type -> accept( * this);

        // If the variable has an initial value, visit the initialization expression.
        if (node.init_exp) {
            node.init_exp -> accept( * this);
        }

        // Ensure the variable name does not conflict with existing variables or parameters in the scope.
        bool kvarKayyam = false;
        for (auto mishtane: mishtaneMisgeret) {
            if (CAST_TO_VARDECL(mishtane)) {
                if (CAST_TO_VARDECL(mishtane) -> id -> value == node.id -> value) {
                    kvarKayyam = true; // Variable already exists in the scope.
                    break;
                }
            }

            if (CAST_TO_FORMAL(mishtane)) {
                if (CAST_TO_FORMAL(mishtane) -> id -> value == node.id -> value) {
                    kvarKayyam = true;
                    break;
                }
            }

        }

        // Ensure the variable name does not conflict with globally defined functions.
        if (!kvarKayyam) {
            for (auto funktsiyya: HatsharatMishtaneGlobali) {
                if (funktsiyya -> id -> value == node.id -> value) {
                    errorDefAsFunc(node.line, funktsiyya -> id -> value);
                }
            }
        } else {
            errorDef(node.line, node.id -> value); // Report a duplicate variable error.
        }

        // Check for type compatibility between the variable and its initial value, if present.
        if (node.init_exp) {
            node.init_exp -> accept( * this);
            if (!(node.init_exp -> type == node.type -> type) &&
                !(node.init_exp -> type == ast::BuiltInType::BYTE && node.type -> type == ast::BuiltInType::INT)) {
                errorMismatch(node.line); // Report a type mismatch error.
            }
        }

        node.type -> accept( * this);

        // Add the variable to the current scope.
        mishtaneMisgeret.push_back(std::make_shared < ast::VarDecl > (node));

        // Update the count of variables in the current scope.
        if (!MisparMishtaneNokhehi.empty()) {
            MisparMishtaneNokhehi.back() ++;
        }

        // Emit information about the variable for debugging or compilation output.
        emitVar(node.id -> value, node.type -> type, moneMishtanim++);
    }

    void ScopePrinter::visit(ast::While & node) {
        // Start a new scope for the while loop.
        beginScope();
        // Enter a new stack frame for the loop.
        enrtyFrame();

        // Increment the counter for active loops, allowing break/continue statements.
        hafsakaVeHemshekhHukiyim++;

        // Visit the condition of the while loop to validate its type.
        node.condition -> accept( * this);

        // Ensure the condition is of type BOOL.
        if (node.condition -> type != ast::BuiltInType::BOOL) {
            errorMismatch(node.condition -> line); // Report a type mismatch error for the condition.
        }

        // Check whether the body of the while loop is enclosed in braces.
        if (node.body -> zeSograyim == true) {
            // If the body is in braces, create a new nested scope for it.
            beginScope();
            enrtyFrame();
            node.body -> accept( * this); // Validate the loop body.
            endScope(); // End the nested scope.
            exitFrame(); // Exit the stack frame for the nested scope.
        } else {
            node.body -> accept( * this); // Validate the loop body without a new scope.
        }

        // Decrement the counter for active loops after processing the while loop.
        hafsakaVeHemshekhHukiyim--;

        // End the scope created for the while loop.
        endScope();
        // Exit the stack frame for the while loop.
        exitFrame();
    }

    void ScopePrinter::visit(ast::If & node) {
        // Start a new scope for the if statement.
        beginScope();
        // Enter a new stack frame for the if statement.
        enrtyFrame();

        // Visit the condition of the if statement to validate its type.
        node.condition -> accept( * this);

        // Ensure the condition is of type BOOL.
        if (node.condition -> type != ast::BuiltInType::BOOL) {
            errorMismatch(node.condition -> line); // Report a type mismatch error for the condition.
        }

        // Check whether the "then" branch of the if statement is enclosed in braces.
        if (node.then -> zeSograyim == true) {
            // If the "then" branch is in braces, create a new nested scope for it.
            beginScope();
            enrtyFrame();
            node.then -> accept( * this); // Validate the "then" branch.
            endScope(); // End the nested scope.
            exitFrame(); // Exit the stack frame for the nested scope.
        } else {
            node.then -> accept( * this); // Validate the "then" branch without a new scope.
        }

        // End the scope created for the if statement's condition and "then" branch.
        endScope();
        exitFrame();

        // If an "else" branch exists, process it similarly.
        if (node.otherwise) {
            beginScope(); // Start a new scope for the "else" branch.
            enrtyFrame();
            if (node.otherwise -> zeSograyim == true) {
                beginScope(); // Create a nested scope for the "else" branch.
                enrtyFrame();
                node.otherwise -> accept( * this); // Validate the "else" branch.
                endScope(); // End the nested scope.
                exitFrame(); // Exit the stack frame for the nested scope.
            } else {
                node.otherwise -> accept( * this); // Validate the "else" branch without a new scope.
            }
            endScope(); // End the scope created for the "else" branch.
            exitFrame();
        }
    }

    void ScopePrinter::visit(ast::Return & node) {
        // Check if the return statement includes an expression.
        if (node.exp) {
            // Visit the return expression to validate its type.
            node.exp -> accept( * this);

            // Ensure the return type matches the function's declared return type.
            if (!(returnType == node.exp -> type ||
                    (returnType == ast::BuiltInType::INT && node.exp -> type == ast::BuiltInType::BYTE))) {
                errorMismatch(node.line); // Report a type mismatch error for the return type.
            }
        } else {
            // If there is no return expression, ensure the function's return type is VOID.
            if (returnType != ast::BuiltInType::VOID) {
                errorMismatch(node.line); // Report a type mismatch error for missing return value.
            }
        }
    }

    void ScopePrinter::visit(ast::Continue & node) {
        // Ensure the "continue" statement is inside a loop.
        if (hafsakaVeHemshekhHukiyim == 0) {
            errorUnexpectedContinue(node.line); // Report an error for an unexpected "continue".
        }
    }

    void ScopePrinter::visit(ast::Break & node) {
        // Ensure the "break" statement is inside a loop.
        if (hafsakaVeHemshekhHukiyim == 0) {
            errorUnexpectedBreak(node.line); // Report an error for an unexpected "break".
        }
    }

    void ScopePrinter::visit(ast::Statements & node) {
        // Iterate over each statement in the list.
        for (auto mehazrer = node.statements.begin(); mehazrer != node.statements.end(); ++mehazrer) {
            // Check whether the statement is enclosed in braces.
            if (( * mehazrer) -> zeSograyim == true) {
                // Create a new scope for the statement.
                beginScope();
                enrtyFrame();
                ( * mehazrer) -> accept( * this); // Validate the statement.
                endScope(); // End the scope for the statement.
                exitFrame(); // Exit the stack frame for the statement's scope.
            } else {
                ( * mehazrer) -> accept( * this); // Validate the statement without a new scope.
            }
        }
    }

    void ScopePrinter::visit(ast::Call & node) {
        // Temporarily enable function usage checks.
        zoKria = true;

        // Visit the function identifier to validate its existence.
        node.func_id -> accept( * this);

        // Temporarily disable function usage checks.
        zoKria = false;

        // Visit the list of arguments to validate their types.
        node.args -> accept( * this);

        // Flag to check if the function exists globally.
        bool funktsiyyaKayyemet = false;

        // Search for the function in the global list of function declarations.
        for (auto funktsiyya: HatsharatMishtaneGlobali) {
            if (funktsiyya -> id -> value == node.func_id -> value) {
                // Function exists; validate its parameters and return type.
                node.type = funktsiyya -> return_type -> type;
                funktsiyyaKayyemet = true;

                // Validate each argument against the corresponding parameter.
                int haIndeks = 0;
                for (auto haFormalHaNokhehi: funktsiyya -> formals -> formals) {
                    
                    bool mishtaneKayyam = false;
                    for (auto mishtane: mishtaneMisgeret) {
                        if (CAST_TO_VARDECL(mishtane)) {
                            if (CAST_TO_VARDECL(mishtane) -> id -> value == node.args -> exps[haIndeks] -> erekhBituy) {
                                mishtaneKayyam = true;
                            }
                        }
                        if (CAST_TO_FORMAL(mishtane)) {
                            if (CAST_TO_FORMAL(mishtane) -> id -> value == node.args -> exps[haIndeks] -> erekhBituy) {
                                mishtaneKayyam = true;
                            }
                        }
                    }
                    
                    // Ensure the argument matches the parameter's type or is convertible.
                    if ((haFormalHaNokhehi -> type -> type != node.args -> exps[haIndeks] -> type &&
                            !(haFormalHaNokhehi -> type -> type == ast::BuiltInType::INT &&
                                node.args -> exps[haIndeks] -> type == ast::BuiltInType::BYTE)) ||
                        funktsiyya -> formals -> formals.size() != node.args -> exps.size()) {
                        std::vector < std::string > tippusim;
                        for (auto haFormalHaNokhehi: funktsiyya -> formals -> formals) {
                            switch (haFormalHaNokhehi -> type -> type) {
                            case ast::BuiltInType::VOID:
                                tippusim.push_back("VOID");
                                break;
                            case ast::BuiltInType::BYTE:
                                tippusim.push_back("BYTE");
                                break;
                            case ast::BuiltInType::STRING:
                                tippusim.push_back("STRING");
                                break;
                            case ast::BuiltInType::INT:
                                tippusim.push_back("INT");
                                break;
                            case ast::BuiltInType::BOOL:
                                tippusim.push_back("BOOL");
                                break;
                            }                        }
                        errorPrototypeMismatch(node.line, node.func_id -> value, tippusim); // Report a prototype mismatch.
                    }
                    haIndeks++;
                }
            }
        }

        // If the function does not exist, report it.
        if (!funktsiyyaKayyemet) {

            for (auto mishtane: mishtaneMisgeret) {
                if (CAST_TO_VARDECL(mishtane) -> id -> value == node.func_id -> value) {
                    errorDefAsVar(node.line, node.func_id -> value);
                }
                if (CAST_TO_FORMAL(mishtane) -> id -> value == node.func_id -> value) {
                    errorDefAsVar(node.line, node.func_id -> value);
                }
            }

            errorUndefFunc(node.line, node.func_id -> value);
        }
    }

    void ScopePrinter::visit(ast::ExpList & node) {
        // Iterate through each expression in the expression list.
        for (auto mehazrer = node.exps.begin(); mehazrer != node.exps.end(); ++mehazrer) {
            ( * mehazrer) -> accept( * this); // Validate each expression using the visitor pattern.
        }
    }

    void ScopePrinter::visit(ast::Or & node) {
        // Visit the left operand to validate its type.
        node.left -> accept( * this);
        // Visit the right operand to validate its type.
        node.right -> accept( * this);

        // Ensure both operands are of type BOOL.
        if (!(node.left -> type == ast::BuiltInType::BOOL && node.right -> type == ast::BuiltInType::BOOL)) {
            errorMismatch(node.line); // Report a type mismatch error.
        }

        // Set the result type of the logical OR operation to BOOL.
        node.type = ast::BuiltInType::BOOL;
    }

    void ScopePrinter::visit(ast::And & node) {
        // Visit the left operand to validate its type.
        node.left -> accept( * this);
        // Visit the right operand to validate its type.
        node.right -> accept( * this);

        // Ensure both operands are of type BOOL.
        if (!(node.left -> type == ast::BuiltInType::BOOL && node.right -> type == ast::BuiltInType::BOOL)) {
            errorMismatch(node.line); // Report a type mismatch error.
        }

        // Set the result type of the logical AND operation to BOOL.
        node.type = ast::BuiltInType::BOOL;
    }

    void ScopePrinter::visit(ast::Not & node) {
        // Visit the operand of the NOT operation to validate its type.
        node.exp -> accept( * this);

        // Ensure the operand is of type BOOL.
        if (!(node.exp -> type == ast::BuiltInType::BOOL)) {
            errorMismatch(node.line); // Report a type mismatch error.
        }

        // Set the result type of the logical NOT operation to BOOL.
        node.type = ast::BuiltInType::BOOL;
    }

    void ScopePrinter::visit(ast::Cast & node) {
        // Visit the expression to be cast to validate its type.
        node.exp -> accept( * this);
        // Visit the target type of the cast.
        node.target_type -> accept( * this);

        // Ensure the cast is valid between INT and BYTE types.
        if (!((node.exp -> type == ast::BuiltInType::INT || node.exp -> type == ast::BuiltInType::BYTE) &&
                (node.target_type -> type == ast::BuiltInType::INT || node.target_type -> type == ast::BuiltInType::BYTE))) {
            errorMismatch(node.line); // Report a type mismatch error for invalid casts.
        }

        // Set the result type of the cast to the target type.
        node.type = node.target_type -> type;
    }

    void ScopePrinter::visit(ast::Type & node) {
        // The visit method for types does not perform any additional operations
        // because types are inherently valid and self-contained.
    }

    void ScopePrinter::visit(ast::RelOp & node) {
        // Visit the left operand to validate its type.
        node.left -> accept( * this);
        // Visit the right operand to validate its type.
        node.right -> accept( * this);

        // Ensure both operands are either INT or BYTE.
        if (!((node.left -> type == ast::BuiltInType::INT || node.left -> type == ast::BuiltInType::BYTE) &&
                (node.right -> type == ast::BuiltInType::INT || node.right -> type == ast::BuiltInType::BYTE))) {
            errorMismatch(node.line); // Report a type mismatch error.
        }

        // Set the result type of the relational operation to BOOL.
        node.type = ast::BuiltInType::BOOL;
    }

    void ScopePrinter::visit(ast::BinOp & node) {
        // Visit the left operand to validate its type.
        node.left -> accept( * this);
        // Visit the right operand to validate its type.
        node.right -> accept( * this);

        // Ensure both operands are either INT or BYTE.
        if (!((node.left -> type == ast::BuiltInType::INT || node.left -> type == ast::BuiltInType::BYTE) &&
                (node.right -> type == ast::BuiltInType::INT || node.right -> type == ast::BuiltInType::BYTE))) {
            errorMismatch(node.line); // Report a type mismatch error.
        }

        // If both operands are BYTE, set the result type to BYTE.
        if (node.left -> type == ast::BuiltInType::BYTE && node.right -> type == ast::BuiltInType::BYTE) {
            node.type = ast::BuiltInType::BYTE;
        } else {
            // Otherwise, set the result type to INT.
            node.type = ast::BuiltInType::INT;
        }
    }

    void ScopePrinter::visit(ast::ID & node) {
        // Store the identifier's name for error reporting.
        node.erekhBituy = node.value;

        // If variable/function usage checks are enabled:
        if (shimush) {
            bool loKayyam = true; // Track whether the identifier is undefined.

            // Search through the current scope for the variable declaration.
            for (auto mishtane: mishtaneMisgeret) {
                if (CAST_TO_VARDECL(mishtane)) {
                    if (CAST_TO_VARDECL(mishtane) -> id -> value == node.value) {
                        node.type = CAST_TO_VARDECL(mishtane) -> type -> type; // Assign its type.
                        loKayyam = false;

                        // If the identifier is used incorrectly as a variable, report it.
                        if (zoKria) {
                            errorDefAsVar(node.line, node.value);
                        }
                        break;
                    }
                }
                if (CAST_TO_FORMAL(mishtane)) {
                    if (CAST_TO_FORMAL(mishtane) -> id -> value == node.value) {
                        node.type = CAST_TO_FORMAL(mishtane) -> type -> type; // Assign its type.
                        loKayyam = false;

                        // If the identifier is used incorrectly as a variable, report it.
                        if (zoKria) {
                            errorDefAsVar(node.line, node.value);
                        }
                        break;
                    }
                }
            }

            // Check global function declarations for the identifier.
            for (auto funktsiyya: HatsharatMishtaneGlobali) {
                if (funktsiyya -> id -> value == node.value) {
                    // If it is a function and being used as a variable, report an error.
                    if (!zoKria) {
                        errorDefAsFunc(node.line, node.value);
                    }
                    loKayyam = false;
                }
            }

            // If the identifier is still undefined, report an error.
            if (loKayyam) {
                if (zoKria) {
                    errorUndefFunc(node.line, node.value);
                }
                errorUndef(node.line, node.value);
            }
        }
    }

    void ScopePrinter::visit(ast::Bool & node) {
        // Set the type of a boolean literal to BOOL.
        node.type = ast::BuiltInType::BOOL;
    }

    void ScopePrinter::visit(ast::String & node) {
        // Set the type of a string literal to STRING.
        node.type = ast::BuiltInType::STRING;
    }

    void ScopePrinter::visit(ast::NumB & node) {
        // Ensure that the byte value is within the valid range (0 to 255).
        if (node.value >= 256 || node.value < 0) {
            errorByteTooLarge(node.line, node.value); // Report an error if out of range.
        }

        // Set the type of the byte literal to BYTE.
        node.type = ast::BuiltInType::BYTE;
    }

    void ScopePrinter::visit(ast::Num & node) {
        // Set the type of a numeric literal to INT.
        node.type = ast::BuiltInType::INT;
    }

    std::ostream & operator << (std::ostream & os,
        const ScopePrinter & printer) {
        // Begin the output of the global scope.
        os << "---begin global scope---" << std::endl;

        // Append all globally declared functions to the output stream.
        os << printer.globalsBuffer.str();

        // Append all local variables and their scopes to the output stream.
        os << printer.buffer.str();

        // End the output of the global scope.
        os << "---end global scope---" << std::endl;

        // Return the output stream for chaining.
        return os;
    }

    static std::string toString(ast::BuiltInType type) {
        // Convert the BuiltInType enumeration to its string representation.
        switch (type) {
        case ast::BuiltInType::INT:
            return "int"; // For integer type.
        case ast::BuiltInType::BOOL:
            return "bool"; // For boolean type.
        case ast::BuiltInType::BYTE:
            return "byte"; // For byte type (small integer, 0-255).
        case ast::BuiltInType::VOID:
            return "void"; // For void type (used in functions with no return value).
        case ast::BuiltInType::STRING:
            return "string"; // For string type.
        default:
            return "unknown"; // Fallback for unrecognized types.
        }
    }

    void ScopePrinter::emitFunc(const std::string & id,
        const ast::BuiltInType & returnType,
            const std::vector < ast::BuiltInType > & tippusim) {
        // Add the function declaration to the global scope buffer.
        globalsBuffer << id << " ("; // Start the function declaration with its name.

        // Append the parameter types.
        for (int haIndeks = 0; haIndeks < tippusim.size(); ++haIndeks) {
            globalsBuffer << toString(tippusim[haIndeks]); // Convert the parameter type to a string.
            if (haIndeks != tippusim.size() - 1) {
                globalsBuffer << ","; // Add a comma between parameter types.
            }
        }

        // Add the return type of the function.
        globalsBuffer << ") -> " << toString(returnType) << std::endl;
    }

    void ScopePrinter::emitVar(const std::string & id,
        const ast::BuiltInType & type, int offset) {
        // Add a variable declaration to the local scope buffer.
        buffer << indent() << id << " " << toString(type) << " " << offset << std::endl;
    }

    void ScopePrinter::endScope() {
        // Indicate the end of the current scope in the buffer.
        buffer << indent() << "---end scope---" << std::endl;

        // Decrease the indentation level to reflect exiting a scope.
        indentLevel--;
    }

    void ScopePrinter::beginScope() {
        // Increase the indentation level to reflect entering a new scope.
        indentLevel++;

        // Indicate the start of a new scope in the buffer.
        buffer << indent() << "---begin scope---" << std::endl;
    }

    std::string ScopePrinter::indent() const {
        // Generate a string with two spaces per indentation level for readability.
        std::string totsaa; // Initialize an empty string.
        for (int haIndeks = 0; haIndeks < indentLevel; ++haIndeks) {
            totsaa += "  "; // Append two spaces for each indentation level.
        }
        return totsaa; // Return the constructed indentation string.
    }

    ScopePrinter::ScopePrinter(): indentLevel(0) {
        // Constructor initializes the indentation level to zero.
    }

    void errorByteTooLarge(int lineno,
        const int value) {
        // Report an error for a byte value that is out of the valid range (0-255).
        std::cout << "line " << lineno << ": byte value " << value << " out of range" << std::endl;
        exit(0); // Exit the program as the error is critical.
    }

    void errorMainMissing() {
        // Report an error indicating the absence of the mandatory 'main' function.
        std::cout << "Program has no 'void main()' function" << std::endl;
        exit(0); // Exit the program as the error is critical.
    }

    void errorUnexpectedContinue(int lineno) {
        // Report an error for an unexpected 'continue' statement outside of a loop.
        std::cout << "line " << lineno << ": unexpected continue statement" << std::endl;
        exit(0); // Exit the program as the error is critical.
    }

    void errorUnexpectedBreak(int lineno) {
        // Report an error for an unexpected 'break' statement outside of a loop.
        std::cout << "line " << lineno << ": unexpected break statement" << std::endl;
        exit(0); // Exit the program as the error is critical.
    }

    void errorPrototypeMismatch(int lineno,
        const std::string & id, std::vector < std::string > & tippusim) {
        // Report a mismatch between the expected and provided parameter types in a function call.
        std::cout << "line " << lineno << ": prototype mismatch, function " << id << " expects parameters (";

        // Append the expected parameter types to the error message.
        for (int haIndeks = 0; haIndeks < tippusim.size(); ++haIndeks) {
            std::cout << tippusim[haIndeks];
            if (haIndeks != tippusim.size() - 1) {
                std::cout << ","; // Add a comma between parameter types.
            }
        }

        std::cout << ")" << std::endl;
        exit(0); // Exit the program as the error is critical.
    }

    void errorMismatch(int lineno) {
        // Report a generic type mismatch error in an expression or assignment.
        std::cout << "line " << lineno << ": type mismatch" << std::endl;
        exit(0); // Exit the program as the error is critical.
    }

    void errorUndefFunc(int lineno,
        const std::string & id) {
        // Report an error for the usage of an undefined function.
        std::cout << "line " << lineno << ": function " << id << " is not defined" << std::endl;
        exit(0); // Exit the program as the error is critical.
    }

    void errorDef(int lineno,
        const std::string & id) {
        // Report an error for a redefinition of a symbol (variable or function).
        std::cout << "line " << lineno << ": symbol " << id << " is already defined" << std::endl;
        exit(0); // Exit the program as the error is critical.
    }

    void errorDefAsVar(int lineno,
        const std::string & id) {
        // Report an error for a function being used as a variable.
        std::cout << "line " << lineno << ": symbol " << id << " is a variable" << std::endl;
        exit(0); // Exit the program as the error is critical.
    }

    void errorDefAsFunc(int lineno,
        const std::string & id) {
        // Report an error for a variable being used as a function.
        std::cout << "line " << lineno << ": symbol " << id << " is a function" << std::endl;
        exit(0); // Exit the program as the error is critical.
    }

    void errorUndef(int lineno,
        const std::string & id) {
        // Report an error for the usage of an undefined variable.
        std::cout << "line " << lineno << ": variable " << id << " is not defined" << std::endl;
        exit(0); // Exit the program as the error is critical.
    }

    void errorSyn(int lineno) {
        // Report a syntax error in the input.
        std::cout << "line " << lineno << ": syntax error\n";
        exit(0); // Exit the program as the error is critical.
    }

    void errorLex(int lineno) {
        // Report a lexical error in the input.
        std::cout << "line " << lineno << ": lexical error\n";
        exit(0); // Exit the program as the error is critical.
    }

    void exitFrame() {
        // Remove all variables declared in the current frame from the symbol table.
        if (!MisparMishtaneNokhehi.empty()) {
            for (int haIndeks = 0; haIndeks < MisparMishtaneNokhehi.back(); haIndeks++) {
                mishtaneMisgeret.pop_back(); // Remove the variable from the scope.
                moneMishtanim -= 1; // Decrement the variable count.
            }
            MisparMishtaneNokhehi.pop_back(); // Remove the current frame's variable count.
        }
    }

    void enrtyFrame() {
        // Add a new frame to track the number of variables in the current scope.
        MisparMishtaneNokhehi.push_back(0);
    }
}