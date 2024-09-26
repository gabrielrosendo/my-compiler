%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern int yylex();   // Declare yylex, the lexer function
extern int yyparse(); // Declare yyparse, the parser function
extern FILE* yyin;    // Declare yyin, the file pointer for the input file
extern int yylineno;  // Declare yylineno, the line number counter

void yyerror(const char* s);

%}

%union {
    int number;
    char character;
    char* string;
    char* keyword;
    char* operator;
    struct ASTNode* ast;
}

%token <number> NUMBER
%token <string> ID
%token <string> TYPE
%token <keyword> PRINT_KEYWORD PRINT
%token <operator> OPERATOR ADD SUB 
%token <string> SYMBOL
%token <string> PARENTHESIS
%token <string> SEMICOLON
%token <string> EQ
%token <string> LBRACE
%token <string> RBRACE


%type <ast> Program StmtList Stmt VarDeclList VarDecl expression
%%

Program: StmtList
       | VarDeclList StmtList
       ;
StmtList: Stmt
        | StmtList Stmt
        ;

Stmt: PRINT '(' expression ')' SEMICOLON { 
    printf("Parsed print statement\n");
}

    | ID EQ expression SEMICOLON {
        printf("Parsed assignment statement\n");
}
    ;

VarDeclList: VarDecl
           | VarDeclList VarDecl
           ;

VarDecl: TYPE ID SEMICOLON { 
    printf("Parsed variable declaration: %s\n", $2);
}
    | TYPE ID EQ expression SEMICOLON {
        printf("Parsed variable declaration with initialization: %s\n", $2);
    }
    ;

expression: NUMBER { printf("Parsed number: %d\n", $1); }
          | ID { printf("Parsed identifier: %s\n", $1); }
          | expression ADD expression { 
              printf("Parsed addition expression\n");
          }
          | expression SUB expression { 
              printf("Parsed subtraction expression\n");
          }
          ;

%%

void yyerror(const char* s) {
    extern char* yytext; // Declare yytext to get the current token text
    fprintf(stderr, "Error: %s at line %d, near '%s'\n", s, yylineno, yytext);
}

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