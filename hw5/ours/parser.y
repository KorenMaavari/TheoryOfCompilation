%{
#include <iostream>
#include "nodes.hpp"
#include "output.hpp"
#include <string>
//#include "token.hpp"

// bison declarations
extern int yylineno;
extern int yylex();
void yyerror(const char*);



using namespace std;
using namespace ast;
using namespace output;

// root of the AST, set by the parser and used by other parts of the compiler
shared_ptr<Node> program; 
%}

// Define tokens here
%token VOID
%token INT
%token BYTE
%token BOOL
%token AND
%token OR
%token NOT
%token TRUE
%token FALSE
%token RETURN
%token IF
%token ELSE
%token WHILE
%token BREAK
%token CONTINUE
%token SC
%token COMMA
%token LPAREN 
%token RPAREN
%token LBRACE
%token RBRACE
%token ASSIGN
%token ID
%token NUM
%token NUM_B
%token STRING
%token R_EQ
%token R_NE
%token R_LT
%token R_GT
%token R_LE
%token R_GE
%token B_ADD
%token B_SUB
%token B_MUL
%token B_DIV
// Define precedence and associativity here
%right ASSIGN
%left OR
%left AND
%left R_EQ R_NE
%left R_LT R_GT R_LE R_GE
%left B_ADD B_SUB
%left B_MUL B_DIV
%right NOT
%left LPAREN RPAREN LBRACE RBRACE


%nonassoc ELSE
%nonassoc IF
%%

// While reducing the start variable, set the root of the AST
Program:  Funcs { program = $1; }
;

// Grammar for functions
Funcs:      { $$ = make_shared<Funcs>(); } 
        | FuncDecl Funcs {  $$ = $2; dynamic_pointer_cast<ast::Funcs>($$)->push_front(dynamic_pointer_cast<ast::FuncDecl>($1)); }
;

// Function declarations
FuncDecl: RetType ID LPAREN Formals RPAREN LBRACE Statements RBRACE {$$ = make_shared<ast::FuncDecl>(dynamic_pointer_cast<ast::ID>($2), dynamic_pointer_cast<ast::Type>($1), dynamic_pointer_cast<ast::Formals>($4), dynamic_pointer_cast<ast::Statements>($7)); } 
;

// Return type for functions
RetType: Type { $$ = $1; }
        | VOID { $$ = make_shared<ast::Type>(ast::BuiltInType::VOID); }       
;

// Formals for function parameters
Formals: { $$ = make_shared<ast::Formals>(); }
         | FormalsList { $$ = $1; }
;

FormalsList: FormalDecl { $$ = make_shared<ast::Formals>(dynamic_pointer_cast<ast::Formal>($1)); }
           | FormalDecl COMMA FormalsList { $$ = $3; dynamic_pointer_cast<ast::Formals>($$)->push_front(dynamic_pointer_cast<ast::Formal>($1)); }
;

// Formal declaration for parameters
FormalDecl: Type ID { $$ = make_shared<ast::Formal>(dynamic_pointer_cast<ast::ID>($2), dynamic_pointer_cast<ast::Type>($1)); }
;

// Statements block
Statements: Statement { $$ = make_shared<ast::Statements>(dynamic_pointer_cast<Statement>($1)); }
         | Statements Statement { $$ = $1; dynamic_pointer_cast<Statements>($$)->push_back(dynamic_pointer_cast<Statement>($2)); }
;

// Single statement rule
Statement: LBRACE Statements RBRACE { (dynamic_pointer_cast<ast::Statements>($2))->zeSograyim = true; $$ = $2; }   
         | Type ID SC { $$ = make_shared<ast::VarDecl>(dynamic_pointer_cast<ast::ID>($2), dynamic_pointer_cast<ast::Type>($1)); }
         | Type ID ASSIGN Exp SC { $$ = make_shared<ast::VarDecl>(dynamic_pointer_cast<ast::ID>($2), dynamic_pointer_cast<ast::Type>($1), dynamic_pointer_cast<ast::Exp>($4)); }
         | ID ASSIGN Exp SC { $$ = make_shared<ast::Assign>(dynamic_pointer_cast<ast::ID>($1), dynamic_pointer_cast<ast::Exp>($3)); }
         | Call SC { $$ = $1; }
         | RETURN SC { $$ = make_shared<ast::Return>(); }
         | RETURN Exp SC { $$ = make_shared<ast::Return>(dynamic_pointer_cast<ast::Exp>($2)); }
         | IF LPAREN Exp RPAREN Statement { $$ = make_shared<ast::If>(dynamic_pointer_cast<ast::Exp>($3), dynamic_pointer_cast<ast::Statement>($5)); }
         | IF LPAREN Exp RPAREN Statement ELSE Statement { $$ = make_shared<ast::If>(dynamic_pointer_cast<ast::Exp>($3), dynamic_pointer_cast<ast::Statement>($5), dynamic_pointer_cast<ast::Statement>($7)); }
         | WHILE LPAREN Exp RPAREN Statement { $$ = make_shared<ast::While>(dynamic_pointer_cast<ast::Exp>($3), dynamic_pointer_cast<ast::Statement>($5)); }
         | BREAK SC { $$ = make_shared<ast::Break>(); }
         | CONTINUE SC { $$ = make_shared<ast::Continue>(); }
;

// Function call
Call: ID LPAREN ExpList RPAREN { $$ = make_shared<ast::Call>(dynamic_pointer_cast<ast::ID>($1), dynamic_pointer_cast<ast::ExpList>($3)); }
         | ID LPAREN RPAREN { $$ = make_shared<ast::Call>(dynamic_pointer_cast<ast::ID>($1)); }
;

// Expression list
ExpList: Exp { $$ = make_shared<ast::ExpList>(dynamic_pointer_cast<ast::Exp>($1)); }
         | Exp COMMA ExpList { $$ = $3; dynamic_pointer_cast<ast::ExpList>($$)->push_front(dynamic_pointer_cast<ast::Exp>($1)); }
;

// Type definitions
Type: INT { $$ = make_shared<ast::Type>(ast::BuiltInType::INT); }
        | BYTE { $$ = make_shared<ast::Type>(ast::BuiltInType::BYTE); }
        | BOOL { $$ = make_shared<ast::Type>(ast::BuiltInType::BOOL); }
;

// Expression rules
Exp: LPAREN Exp RPAREN { $$ = $2; }
        | Exp B_ADD Exp { 
            $$ = make_shared<ast::BinOp>(dynamic_pointer_cast<ast::Exp>($1), dynamic_pointer_cast<ast::Exp>($3), ast::BinOpType::ADD); }
        | Exp B_SUB Exp { 
            $$ = make_shared<ast::BinOp>(dynamic_pointer_cast<ast::Exp>($1), dynamic_pointer_cast<ast::Exp>($3), ast::BinOpType::SUB); }
        | Exp B_MUL Exp { 
            $$ = make_shared<ast::BinOp>(dynamic_pointer_cast<ast::Exp>($1), dynamic_pointer_cast<ast::Exp>($3), ast::BinOpType::MUL); }
        | Exp B_DIV Exp { 
            $$ = make_shared<ast::BinOp>(dynamic_pointer_cast<ast::Exp>($1), dynamic_pointer_cast<ast::Exp>($3), ast::BinOpType::DIV); }
        | ID { $$ = $1; }
        | Call { $$ = $1; }
        | NUM { $$ = $1; }
        | NUM_B { $$ = $1; }
        | STRING { $$ = $1; }
        | TRUE { $$ = make_shared<ast::Bool>(true); }
        | FALSE { $$ = make_shared<ast::Bool>(false); }
        | NOT Exp { $$ = make_shared<ast::Not>(dynamic_pointer_cast<ast::Exp>($2)); }
        | Exp AND Exp { $$ = make_shared<ast::And>(dynamic_pointer_cast<ast::Exp>($1), dynamic_pointer_cast<ast::Exp>($3)); }
        | Exp OR Exp { $$ = make_shared<ast::Or>(dynamic_pointer_cast<ast::Exp>($1), dynamic_pointer_cast<ast::Exp>($3)); }
        | Exp R_EQ Exp { $$ = make_shared<ast::RelOp>(dynamic_pointer_cast<ast::Exp>($1), dynamic_pointer_cast<ast::Exp>($3), ast::RelOpType::EQ); }
        | Exp R_NE Exp { $$ = make_shared<ast::RelOp>(dynamic_pointer_cast<ast::Exp>($1), dynamic_pointer_cast<ast::Exp>($3), ast::RelOpType::NE); }
        | Exp R_LT Exp { $$ = make_shared<ast::RelOp>(dynamic_pointer_cast<ast::Exp>($1), dynamic_pointer_cast<ast::Exp>($3), ast::RelOpType::LT); }
        | Exp R_GT Exp { $$ = make_shared<ast::RelOp>(dynamic_pointer_cast<ast::Exp>($1), dynamic_pointer_cast<ast::Exp>($3), ast::RelOpType::GT); }
        | Exp R_LE Exp { $$ = make_shared<ast::RelOp>(dynamic_pointer_cast<ast::Exp>($1), dynamic_pointer_cast<ast::Exp>($3), ast::RelOpType::LE); }
        | Exp R_GE Exp { $$ = make_shared<ast::RelOp>(dynamic_pointer_cast<ast::Exp>($1), dynamic_pointer_cast<ast::Exp>($3), ast::RelOpType::GE); }
        | LPAREN Type RPAREN Exp { $$ = make_shared<ast::Cast>(dynamic_pointer_cast<ast::Exp>($4), dynamic_pointer_cast<ast::Type>($2)); }
;

%%

// Error reporting
void yyerror(const char* message) {
    errorSyn(yylineno); 
}