// #ifndef CODE_HANDLER_HPP
// #define CODE_HANDLER_HPP

// #include <string>
// #include <vector>

// enum class LabelType {
//     PRIMARY, // Used for the "true" brach in conditional instructions.
//     SECONDARY // Used for the "false" brach in conditional instructions.
// };

// // CodeHandler Class: Manages code generation and backpatching for LLVM IR.
// class CodeHandler {
//     public:
//     CodeHandler();
//     CodeHandler(const CodeHandler&);
//     void operator=(const CodeHandler&);
//     ~CodeHandler();

//     std::vector<std::string> intermediateBuffer; // Holds the intermediate instructions (for example: arithmetic operations).
//     std::vector<std::string> globalBuffer; // Holds global desclarations (for example: format string, external function declarations).

//     // Singleton Instance Method: Returns a single, shared instance of the CodeHandler class.
//     static CodeHandler& getInstance();

//     // **Label management**
//     s
// };

class Saar {
    public:
        int x;
};

// #endif // CODE_HANDLER_HPP