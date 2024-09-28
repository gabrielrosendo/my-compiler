%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "AST.h"
#include "symbolBST.h"
#include "symbolBST_Test.h"

extern int yydebug;     // Debug mode for Bison
extern int yylex();   // Declare yylex, the lexer function
extern int yyparse(); // Declare yyparse, the parser function
extern FILE* yyin;    // Declare yyin, the file pointer for the input file
extern int yylineno;  // Declare yylineno, the line number counter

void yyerror(const char* s);

ASTNode* root = NULL; 

SymbolBST* symbolBST = NULL;

%}

%union {
    int number;
    char character;
    char* string;
    char* keyword;
    char* op;
    struct ASTNode* ast;
}

%token <number> NUMBER
%token <string> ID
%token <string> TYPE
%token <string> PRINT
%token <op> EQ
%token <op> ADD
%token <op> SUB
%token <string> LPAREN
%token <string> RPAREN
%token <string> SEMICOLON
%token <string> LBRACE
%token <string> RBRACE


%type <ast> Program StmtList Stmt VarDeclList VarDecl expression
%%

Program: StmtList { 
            printf("The parser has started\n"); 
            root = createNode(NodeType_Program); // Create the program node
            root->value.program.StmtList = $1; // Set the statement list
        }
       | VarDeclList StmtList { 
            printf("The parser has started\n"); 
            root = createNode(NodeType_Program); // Create the program node
            root->value.program.VarDeclList = $1; // Set the variable declaration list
            root->value.program.StmtList = $2; // Set the statement list
        }
       ;
VarDeclList: VarDecl {
                printf("Parsed variable declaration\n");
                $$ = createNode(NodeType_VarDeclList); 
                $$->value.VarDeclList.VarDecl = $1;
            }
           | VarDeclList VarDecl {
                printf("Parsed variable declaration\n");
                $$ = createNode(NodeType_VarDeclList); 
                $$->value.VarDeclList.VarDecl = $1;
                $$->value.VarDeclList.nextVarDecl = $2;
           }
           ;
VarDecl: TYPE ID SEMICOLON { 
            $$ = createNode(NodeType_VarDecl); 
            $$->value.VarDecl.varType = $1; 
            $$->value.VarDecl.varName = $2;
            if(symbolBST == NULL) {
                symbolBST = createSymbolBST();
            }
            addSymbol(symbolBST, $2, $1);
            printf("Parsed variable declaration: %s\n", $2);
        }    
        | TYPE ID EQ expression SEMICOLON {
            $$->value.VarDecl.varType = $1; 
            $$->value.VarDecl.varName = $2;
            if(symbolBST == NULL) {
                symbolBST = createSymbolBST();
            }
            addSymbol(symbolBST, $2, $1);
            $$->value.VarDecl.initExpr = $3; 
            printf("Parsed variable declaration with initialization: %s\n", $2);
    }
    ;

StmtList: Stmt {
            printf("Parsed statement\n");
            $$ = createNode(NodeType_StmtList);
            $$->value.StmtList.stmt = $1;
        }  // A program can have a single print statement and nothing else
        | StmtList Stmt {
            printf("Parsed statement list\n");
            $$ = createNode(NodeType_StmtList);
            $$->value.StmtList.stmt = $1;
            $$->value.StmtList.nextStmt = $2;
        }
        ;

Stmt: PRINT LPAREN expression RPAREN SEMICOLON { 
    printf("Parsed print statement\n");
}

    | ID EQ expression SEMICOLON {
        printf("Parsed assignment statement\n");
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

int main(int argc, char **argv) {
    #ifdef YYDEBUG
        yydebug = 1;  // Enable Bison debug if compiled with YYDEBUG
    #endif

    printf("Compiler started. \n\n");

    // Check if an input file is provided
    if (argc > 1) {
        yyin = fopen(argv[1], "r");
        if (!yyin) {
            perror(argv[1]);  // Report error if file cannot be opened
            return 1;
        }
    }

    // Choose whether to run the parser (yyparse) or lexer (yylex)
    // For this example, we’ll use yyparse() as the primary function.
    yyparse();  // Call parser (which may call yylex() internally)

    // Optionally, if you want to use yylex directly:
    // yylex();  // Directly call the lexer if needed

    printf("\n\n---------------------------------------------------------");
    printf("\n\nTESTING\n\n");
    printf("---------------------------------------------------------\n\n");

    // Testing the symbolBST
    symbolBST_TestCreation();
    symbolBST_Test_AddSymbol_SingleIntegerInput();
    symbolBST_Test_AddSymbol_MultipleIntegerInputs();
    symbolBST_Test_GetSymbol_EmptySymbolBST();
    symbolBST_Test_GetSymbol_FoundAll();
    symbolBST_Test_AddSymbol_DoublicateSymboleError1();
    symbolBST_Test_AddSymbol_DoublicateSymboleError2();
    symbolBST_Test_freeSymbolTable_freesSymbolTable();

    return 0;
}

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
        $$->VarDecl.varType = strdup($1);
        $$->VarDecl.varName = strdup($2);

        addSymbol(symTab, $2, $1);
    }
    */