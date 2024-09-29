%{
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "AST.h"
#include "symbolBST.h"
#include "symbolBST_Test.h"
#include "semantic.h"

extern int yydebug;     // Debug mode for Bison
extern int yylex();   // Declare yylex, the lexer function
extern int yyparse(); // Declare yyparse, the parser function
extern FILE* yyin;    // Declare yyin, the file pointer for the input file
extern int yylineno;  // Declare yylineno, the line number counter
extern TAC* tacHead;

void yyerror(const char* s);

bool enableTesting = false;

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


%type <ast> Program VarDeclList VarDecl StmtList Stmt Expr BinOp
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

VarDeclList: {}
           | VarDecl VarDeclList {
                $$ = createNode(NodeType_VarDeclList); 
                $$->value.VarDeclList.VarDecl = $1;
                $$->value.VarDeclList.nextVarDecl = $2;
           }
;

VarDecl: TYPE ID SEMICOLON { 
            printf("PARSER: Recognized variable declaration\n");
            printf("Type: %s, ID: %s\n", $1, $2);
            $$ = createNode(NodeType_VarDecl); 
            $$->value.VarDecl.varType = $1; 
            $$->value.VarDecl.varName = $2;
            printf("Parsed variable declaration: %s\n", $2);
        }    
        | TYPE ID EQ Expr SEMICOLON {
            printf("PARSER: Recognized variable declaration\n");
            printf("Type: %s, ID: %s\n", $1, $2);
            $$ = createNode(NodeType_VarDecl); 
            $$->value.VarDecl.varType = $1; 
            $$->value.VarDecl.varName = $2;
            $$->value.VarDecl.initExpr = $4; 
            printf("Parsed variable declaration with initialization: %s\n", $2);
    }
;

StmtList: {}
        | Stmt StmtList {
            printf("Parsed statement list\n");
            $$ = createNode(NodeType_StmtList);
            $$->value.StmtList.stmt = $1;
            $$->value.StmtList.nextStmt = $2;
        }
;

Stmt: ID EQ Expr SEMICOLON { 
								printf("PARSER: Recognized assignment statement\n");
                                printf("ID: %s, EQ: %s, Expr: %p\n", $1, $2, $3);
                                $$ = createNode(NodeType_Assignment);
								$$->value.assignment.varName = $1;
								$$->value.assignment.op = $2;
								$$->value.assignment.expr = $3;
 }
	| PRINT LPAREN Expr RPAREN SEMICOLON { 
                                            printf("PARSER: Recognized print statement\n"); 
                                            $$ = createNode(NodeType_Print);
                                            $$->value.print.expr = $3;
                                         }
;

Expr: Expr BinOp Expr { printf("PARSER: Recognized expression\n");
						$$ = createNode(NodeType_Expression);
						$$->value.Expression.left = $1;
						$$->value.Expression.right = $3;
						$$->value.Expression.op = $2->value.binaryOp.op;
					  }
	| NUMBER { 
				printf("PARSER: Recognized number\n");
				$$ = createNode(NodeType_Number);
				$$->value.Number.number = $1;
			 }		
	| ID {
			$$ = createNode(NodeType_Identifier);
			$$->value.identifier.name = $1;
		}
;

BinOp: ADD {
				printf("PARSER: Recognized binary operator\n");
				$$ = createNode(NodeType_BinaryOp);
				$$->value.binaryOp.op = $1;
            }
;

%%

int main(int argc, char **argv) {
    #ifdef YYDEBUG
        yydebug = 0;  // Enable Bison debug if compiled with YYDEBUG
    #endif

    symbolBST = createSymbolBST();

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

    printf("-----printAST-----\n");
    printAST(root, 0);

    semanticAnalysis(root, symbolBST);

    TAC* tempTac = tacHead;
    while (tempTac != NULL) {
        printTAC(tempTac);
        tempTac = tempTac->next;
    }

    freeSymbolTable(symbolBST);
    printf("Freeing AST...\n");
    freeAST(root);

    if(enableTesting) {
        printf("\n\n---------------------------------------------------------");
        printf("\n\nTESTING\n\n");
        printf("---------------------------------------------------------\n\n");

        // Testing the symbolBST
        symbolBST_TestCreation();
        symbolBST_Test_AddSymbol_SingleIntegerInput();
        symbolBST_Test_AddSymbol_MultipleIntegerInputs();
        symbolBST_Test_GetSymbol_EmptySymbolBST();
        symbolBST_Test_GetSymbol_FoundAll();
        symbolBST_Test_freeSymbolTable_freesSymbolTable();
        symbolBST_Test_AddSymbol_DoublicateSymboleError1();
        symbolBST_Test_AddSymbol_DoublicateSymboleError2();
    }

    return 0;
}

void yyerror(const char* s) {
    extern char* yytext; // Declare yytext to get the current token text
    fprintf(stderr, "Error: %s at line %d, near '%s'\n", s, yylineno, yytext);
}