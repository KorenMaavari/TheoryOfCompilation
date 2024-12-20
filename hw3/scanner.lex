%{
#include <ostream>   // For handling output streams
#include <iostream>  // Provides input and output stream objects like std::cout
#include "output.hpp" // Includes utility functions for error reporting and token printing
//#include <cstdlib>
#include <string>
#include "parser.tab.h"
%}

%option yylineno
%option noyywrap

/* Define patterns for matching */


pattern_of_id                   [a-zA-Z][a-zA-Z0-9]*
pattern_of_num                   0|[1-9][0-9]*
pattern_of_num_b                 0b|[1-9][0-9]*b
pattern_of_string               \"([^\n\r\"\\]|\\[rnt"\\])+\" 
whitespace                       [ \t\r\n]
pattern_of_comment               \/\/[^\r\n]*[\r|\n|\r\n]?


                       
%%

"void"                          { return VOID; }
"int"                           { return INT; }
"byte"                          { return BYTE; }
"bool"                          { return BOOL; }
"and"                           { return AND; }
"or"                            { return OR; }
"not"                           { return NOT; }
"true"                          { return TRUE; }
"false"                         { return FALSE; }
"return"                        { return RETURN; }
"if"                            { return IF; }
"else"                          { return ELSE; }
"while"                         { return WHILE; }
"break"                         { return BREAK; }
"continue"                      { return CONTINUE; }
";"                             { return SC; }
","                             { return COMMA; }
"("                             { return LPAREN; }
")"                             { return RPAREN; }
"{"                             { return LBRACE; }
"}"                             { return RBRACE; }
"="                             { return ASSIGN; }
"=="                            { return R_EQ; }  
"!="                            { return R_NE; } 
"<"                             { return R_LT; } 
">"                             { return R_GT; } 
"<="                            { return R_LE; } 
">="                            { return R_GE; } 
[+]                            { return B_ADD; } 
[-]                             { return B_SUB; } 
[*]                            { return B_MUL; } 
[/]                           { return B_DIV; } 
                    
{pattern_of_id}                   { yylval = std::make_shared<ast::ID>(yytext); return ID; }
{pattern_of_num}                   { yylval = std::make_shared<ast::Num>(yytext); return NUM; }
{pattern_of_num_b}                 { yylval = std::make_shared<ast::NumB>(yytext); return NUM_B; }
{pattern_of_string}                { yylval = std::make_shared<ast::String>(yytext); return STRING; } 
{whitespace}                      ; 
{pattern_of_comment}              ;
.                               { output::errorLex(yylineno);}  
%%
