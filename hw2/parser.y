%{

#include <iostream>
#include "nodes.hpp"
#include "output.hpp"
#include "token.hpp"

// bison declarations
extern int yylineno;
extern int yylex();

void yyerror(const char*);

// root of the AST, set by the parser and used by other parts of the compiler
std::shared_ptr<ast::Node> program; // "program" is the rootNode

using namespace std;

// TODO: Place any additional declarations here
using namespace output;

%}

// TODO: Define tokens here
%union {
    shared_ptr<ast::Node> astNode;
    shared_ptr<ast::ID> identifier;
    shared_ptr<ast::Exp> expression;
}

%token <astNode> VOID INT BYTE BOOL AND OR NOT TRUE FALSE RETURN IF ELSE WHILE BREAK CONTINUE
%token SC COMMA LPAREN RPAREN LBRACE RBRACE
%token ASSIGN RELOP BINOP
%token <identifier> ID
%token <expression> NUM NUM_B STRING

// TODO: Define precedence and associativity here
%left LPAREN RPAREN
%right '+' '-'
%right NOT
%left '*' '/'
%left RELOP
%left AND OR
%right ASSIGN

%type <astNode> Program Funcs FuncDecl RetType Formals FormalsList FormalDecl Statements Statement Call ExpList Type Exp

%%

// While reducing the start variable, set the root of the AST
Program:  Funcs { program = $1; }
;

// TODO: Define grammar here
Funcs:      { $$ = make_shared<ast::Funcs>(); } 
        | FuncDecl Funcs { $2->push_front($1); // Maybe push_back
             $$ = $2 ; }
;

FuncDecl: RetType ID LPAREN Formals RPAREN LBRACE Statements RBRACE { $$ = make_shared<ast::FuncDecl>($2, $1, $4, $7); } 
;
RetType: Type { $$ = $1; }
        | VOID { $$ = make_shared<ast::Type>(ast::BuiltInType::VOID); }       
;
Formals: { $$ = make_shared<ast::Formals>(); }
         | FormalsList { $$ = $1; }
;
FormalsList: FormalDecl { $$ = make_shared<ast::Formals>(); $$->push_back($1); } // maybe push_front
         | FormalDecl COMMA FormalsList { $$ = $3 ; $$->push_front($1);}
;
FormalDecl: Type ID {$$ = make_shared<ast::Formal>($2, $1);}
;
Statements: Statement {$$ = make_shared<ast::Statements>($1); }
         | Statements Statement {$1->push_back($2); $$ = $1;}
;
Statement: LBRACE Statements RBRACE {$$ = $2;}   
         | Type ID SC {$$ = make_shared<ast::VarDecl>($2, $1);}
         | Type ID ASSIGN Exp SC {$$ = make_shared<ast::VarDecl>($2, $1, $4);}
         | ID ASSIGN Exp SC {$$ = make_shared<ast::Assign>($1, $3);}
         | Call SC {$$ = $1;}
         | RETURN SC {$$ = make_shared<ast::Return>();}
         | RETURN Exp SC {$$ = make_shared<ast::Return>($2)};
         | IF LPAREN Exp RPAREN Statement {$$ = make_shared<ast::If>($3, $5); }
         | IF LPAREN Exp RPAREN Statement ELSE Statement {$$ = make_shared<ast::If>($3, $5, $7); }
         | WHILE LPAREN Exp RPAREN Statement {$$ = make_shared<ast::While>($3, $5);}
         | BREAK SC {$$ = make_shared<ast::Break>();}
         | CONTINUE {$$ = make_shared<ast::Continue>();}
;
Call: ID LPAREN ExpList RPAREN {$$ = make_shared<ast::Call>($1, $3);}
         | ID LPAREN RPAREN {$$ = make_shared<ast::Call>($1);}
;
ExpList: Exp {$$ = make_shared<ast::ExpList>(); $$->push_back($1);}
         | Exp COMMA ExpList {$$ = $3; $$->push_front($1);}
;
Type: INT {$$ = make_shared<ast::Type>(ast::BuiltInType::INT);}
        | BYTE {$$ = make_shared<ast::Type>(ast::BuiltInType::BYTE);}
        | BOOL {$$ = make_shared<ast::Type>(ast::BuiltInType::BOOL);}
;
Exp: LPAREN Exp RPAREN {$$ = $2;}
        | Exp BINOP Exp {$$ = make_shared<ast::BinOp>($1, $3, dynamic_pointer_cast<ast::BinOpType>(yytext[0])); }
        | ID {$$ = $1;}
        | Call {$$ = $1;}
        | NUM {$$ = $1;}
        | NUM_B {$$ = $1;}
        | STRING {$$ = $1;}
        | TRUE {$$ = make_shared<ast::Bool>(true);}
        | FALSE {$$ = make_shared<ast::Bool>(false);}
        | NOT Exp {$$ = make_shared<ast::Not>($2);}
        | Exp AND Exp {$$ = make_shared<ast::And>($1, $3);}
        | Exp OR Exp {$$ = make_shared<ast::Or>($1, $3);}
        | Exp RELOP Exp {$$ = make_shared<ast::RELOP>($1, $3, dynamic_pointer_cast<ast::RelOpType>(yytext[0]));}
        | LPAREN Type RPAREN Exp {$$ = make_shared<ast::Cast>($4, $2);}
;

%%

// TODO: Place any additional code here
void yyerror(const char* message)
{
    errorSyn(yylineno);
}
