%{
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "AST.h"
#include "symbolBST.h"
#include "functionSymbolBST.h"
#include "symbolBST_Test.h"
#include "semantic.h"
#include "codeGenerator.h"
#include "optimizer.h"
#include "arraySymbolTable.h"



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
FunctionSymbolBST* functionBST = NULL;
ArraySymbolTable* arraySymTab = NULL;
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
%token <keyword> VOID
%token <string> LBRACKET RBRACKET
%token <string> PRINT
%token <op> EQ
%token <op> ADD
%token <op> SUB
%token <op> MUL
%token <op> DIV
%token <string> LPAREN
%token <string> RPAREN
%token <string> SEMICOLON
%token <string> LBRACE
%token <string> RBRACE
%token <string> COMMA
%token <string> RETURN
%token <keyword> MAIN

%left ADD SUB
%left MUL DIV


%type <ast> Program FuncDeclList FuncDecl MainFunc ParamList ParamDecl Body VarDeclList VarDecl StmtList Stmt Expr BinOp CallParamList FuncTail FunctionCall

%%

Program: 
        FuncDeclList MainFunc { 
            printf("The parser has started\n"); 
            root = createNode(NodeType_Program); // Create the program node
            root->value.program.FuncDeclList = $1; // Set the function declaration list
            root->value.program.MainFunc = $2; // Set the statement list
        }
;

FuncDeclList: {}
            | FuncDecl FuncDeclList {
                $$ = createNode(NodeType_FuncDeclList);
                $$->value.FuncDeclList.FuncDecl = $1;
                $$->value.FuncDeclList.nextFuncDecl = $2;
            };

FuncDecl: TYPE ID LPAREN ParamList RPAREN LBRACE Body RBRACE {
            printf("PARSER: Recognized function declaration\n");
            $$ = createNode(NodeType_FuncDecl);
            $$->value.FuncDecl.FuncType = $1;
            $$->value.FuncDecl.FuncName = $2;
            $$->value.FuncDecl.ParamList = $4;
            $$->value.FuncDecl.Body = $7;
        };

MainFunc: VOID MAIN LPAREN RPAREN LBRACE Body RBRACE {
            printf("PARSER: Recognized main function\n");
            $$ = createNode(NodeType_MainFunc);
            $$->value.MainFunc.Body = $6;
        };

ParamList: /* empty */ {
    $$ = NULL;  /* Empty parameter list */
}

| ParamDecl {
    $$ = createNode(NodeType_ParamList);
    $$->value.ParamList.ParamDecl = $1;
    $$->value.ParamList.nextParamDecl = NULL;
    printf("One parameter\n");
}
| ParamDecl COMMA ParamList {
    $$ = createNode(NodeType_ParamList);
    $$->value.ParamList.ParamDecl = $1;
    $$->value.ParamList.nextParamDecl = $3;
    printf("PARSER: Recognized parameter list\n");
}
;

ParamDecl: TYPE ID {
    $$ = createNode(NodeType_ParamDecl);
    $$->value.ParamDecl.paramType = $1;
    $$->value.ParamDecl.paramName = $2;
    printf("PARSER: Recognized parameter declaration\n");
}
;

Body: VarDeclList StmtList FuncTail { 
    printf("PARSER: Recognized function body\n");
    $$ = createNode(NodeType_Body);
    $$->value.Body.VarDeclList = $1;
    $$->value.Body.StmtList = $2;
    $$->value.Body.FuncTail = $3;
    }
;

FuncTail: /* no return statement, if a void function was defined */
    { 
        $$ = createNode(NodeType_FuncTail);
        $$->value.FuncTail.expr = NULL;
        $$->value.FuncTail.type = strdup("void");
    }
    | RETURN Expr SEMICOLON { 
        printf("PARSER: Recognized function tail\n");
        $$ = createNode(NodeType_FuncTail);
        $$->value.FuncTail.expr = $2;
        $$->value.FuncTail.type = strdup("unknown");
    }
;
VarDeclList: {$$ = NULL;}
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
        | TYPE ID LBRACKET NUMBER RBRACKET SEMICOLON {
            printf("PARSER: Recognized array declaration\n");
                $$ = createNode(NodeType_VarDecl);
                $$->value.VarDecl.varType = $1;
                $$->value.VarDecl.varName = $2;
                $$->value.VarDecl.isArray = 1;
                $$->value.VarDecl.arraySize = $4;
                $$->value.VarDecl.initExpr = NULL;

        }    		
        | TYPE ID {
                  printf ("Missing semicolon after declaring variable: %s\n", $2);
                  // stop compilation
                    exit(1);

             }        
;

StmtList: {$$ = NULL;}
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
	| PRINT LPAREN ID RPAREN SEMICOLON { 
                                            printf("PARSER: Recognized print statement\n"); 
                                            $$ = createNode(NodeType_Print);
                                            $$->value.print.name = $3;
                                         }
    // Handle missng semicolon  
    | PRINT LPAREN ID RPAREN { 
                                printf ("Missing semicolon after print statement: %s\n", $3);
                                // stop compilation
                                exit(1);
                             }
    | ID EQ Expr { 
                    printf ("Missing semicolon after assignment statement: %s\n", $1);
                    // stop compilation
                    exit(1);
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
    | ID LPAREN CallParamList RPAREN {
        printf("PARSER: Recognized function call\n");
        $$ = createNode(NodeType_FunctionCall);
        $$->value.FunctionCall.funcName = $1;
        $$->value.FunctionCall.CallParamList = $3;
    }
    | ID LBRACKET Expr RBRACKET {
        printf("PARSER: Recognized array access\n");
        $$ = createNode(NodeType_ArrayAccess);
        $$->value.ArrayAccess.varName = $1;
        $$->value.ArrayAccess.indexExpr = $3;
    }
;
FunctionCall: ID LPAREN CallParamList RPAREN {
        printf("PARSER: Recognized function call\n");
        $$ = createNode(NodeType_FunctionCall);
        $$->value.FunctionCall.funcName = $1;
        $$->value.FunctionCall.CallParamList = $3;
    }
;

CallParamList:
           {
            printf("PARSER: Recognized no call parameters\n");
            $$ = NULL;
        } /*empty, i.e. it is possible not to have any parameter*/
    | Expr { 
            printf("PARSER: Recognized SINGULAR call parameter\n");
            $$ = createNode(NodeType_CallParamList);
            $$->value.CallParamList.expr = $1;
        }
    | Expr COMMA CallParamList { 
            printf("PARSER: Recognized call parameter list\n"); 
            $$ = createNode(NodeType_CallParamList);
            $$->value.CallParamList.expr = $1;
            $$->value.CallParamList.nextParam = $3;
        }
;


BinOp: ADD {
				printf("PARSER: Recognized addition operator\n");
				$$ = createNode(NodeType_BinaryOp);
				$$->value.binaryOp.op = $1;
            }
        | SUB {
            printf("Recognized subtraction operator\n");
            $$ = createNode(NodeType_BinaryOp);
            $$->value.binaryOp.op = $1;
        }
        | MUL {
            printf("Recognized multiplication operator\n");
            $$ = createNode(NodeType_BinaryOp);
            $$->value.binaryOp.op = $1;
        }
        | DIV {
            printf("Recognized division operator\n");
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
    functionBST = createFunctionSymbolBST();
    arraySymTab = createArraySymbolTable();


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
    printf("Test 2 \n");

    semanticAnalysis(root, symbolBST, functionBST, arraySymTab);

    printf("\n-----TAC CODE-----\n\n");
    TAC* tempTac = tacHead;
    while (tempTac != NULL) {
        printTAC(tempTac);
        tempTac = tempTac->next;
    }
    printf("---END TAC CODE----\n\n");

    // Output TAC to file
    FILE* tacFile = fopen("TAC.ir", "w");
    if (tacFile) {
        printf("Writing TAC to file...\n");
        printTACToFile("TAC.ir", tacHead);
        fclose(tacFile);
    } else {
        fprintf(stderr, "Error: Could not open TAC.ir for writing\n");
    }

    
    //optimizeforMIPS(&tacHead);
    initCodeGenerator("output.s", tacHead);
    generateMIPS(tacHead);
    finalizeCodeGenerator("output.s");
    // Code optimization
    printf("\n=== CODE OPTIMIZATION ===\n");
    // Traverse the linked list of TAC entries and optimize
    // But - you MIGHT need to traverse the AST again to optimize
    //optimizeTAC(&tacHead);
    // Output optimized TAC to file
    FILE* optimizedTacFile = fopen("optimizedTAC.ir", "w");
    freeSymbolTable(symbolBST);
    freeFunctionSymbolTable(functionBST);
    freeArraySymbolTable(arraySymTab);

    printf("Freeing AST...\n");
    freeAST(root);

    printf("-----TAC CODE AFTER OPTIMIZATION-----\n");
    tempTac = tacHead;
    while (tempTac != NULL) {
        printf("testing\n");
        printTAC(tempTac);
        tempTac = tempTac->next;
    }
    printf("testing\n");

    if (optimizedTacFile) {
        printf("Writing optimized TAC to file...\n");
        printTACToFile("optimizedTAC.ir", tacHead);
        fclose(optimizedTacFile);
    }

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