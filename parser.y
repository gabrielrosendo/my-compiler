%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// #include "AST.h"

#define TABLE_SIZE 100

extern int yylex();   // Declare yylex, the lexer function
extern int yyparse(); // Declare yyparse, the parser function
extern FILE* yyin;    // Declare yyin, the file pointer for the input file
extern int yylineno;  // Declare yylineno, the line number counter
// extern TAC* tacHead;  // Declare the head of the linked list of TAC entries

void yyerror(const char* s);

// ASTNode* root = NULL; 
// SymbolTable* symTab = NULL;
// Symbol* symbol = NULL;
%}

%union {
    int number;
    char character;
    char* string;
    char* operator;
    // struct ASTNode* ast;
}

%token <number> NUMBER
%token <string> ID
%token <string> TYPE
%token <string> PRINT_KEYWORD
%token <string> IF_KEYWORD
%token <string> ELSE_KEYWORD
%token <string> WHILE_KEYWORD
%token <string> RETURN_KEYWORD
%token <operator> OPERATOR
%token <string> SYMBOL
%token <string> PARENTHESIS
%token <string> SEMICOLON
%token <string> EQ
%token <string> LBRACE
%token <string> RBRACE

%type <ast> Program StmtList Stmt VarDeclList expression

%%
Program: StmtList
       | VarDeclList StmtList
       ;

StmtList: Stmt
        | StmtList Stmt
        ;

Stmt: PRINT_KEYWORD expression SEMICOLON { 
    printf("Parsed print statement\n");
    // TO-DO
}
    | IF_KEYWORD expression LBRACE Program RBRACE { 
        printf("Parsed if statement\n");
        // TO-DO
}
    | WHILE_KEYWORD expression LBRACE Program RBRACE { 
        printf("Parsed while statement\n");
        // TO-DO
}
    | RETURN_KEYWORD expression SEMICOLON { 
        printf("Parsed return statement\n");
        // TO-DO
}
    ;

VarDeclList: TYPE ID SEMICOLON { 
    printf("Parsed variable declaration: %s\n", $2);

    /*
    symbol = lookupSymbol(symTab, $2);
    if (symbol != NULL) {
        fprintf(stderr, "Error: Variable %s already declared\n", $2);
        exit(EXIT_FAILURE);
    } else {
        $$ = malloc(sizeof(ASTNode));
        if (!$$) {
            fprintf(stderr, "Memory allocation failed for VarDecl\n");
            exit(EXIT_FAILURE);
        }
        $$->type = NodeType_VarDecl;
        $$->varDecl.varType = strdup($1);
        $$->varDecl.varName = strdup($2);

        addSymbol(symTab, $2, $1);
    }
    */
}
    | VarDeclList TYPE ID SEMICOLON { 
        printf("Parsed variable declaration\n");
        // TO-DO
}
    ;

expression: NUMBER { printf("Parsed number: %d\n", $1); }
          | ID { printf("Parsed identifier: %s\n", $1); }
          | expression OPERATOR expression { 
              printf("Parsed operator expression\n");
          }
          ;

%%

void yyerror(const char* s) {
    fprintf(stderr, "Error: %s at line %d\n", s, yylineno);
}

int main(int argc, char **argv) {
    if (argc > 1) {
        yyin = fopen(argv[1], "r");
        if (!yyin) {
            perror(argv[1]);
            return 1;
        }
    }
    yyparse();
    return 0;
}