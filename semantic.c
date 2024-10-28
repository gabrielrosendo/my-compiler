#include <stdio.h>
#include <stdbool.h>
#include "semantic.h"
#include "symbolBST.h"
#include "functionSymbolBST.h"
#include "arraySymbolTable.h"

// Initialize the global TAC list
TAC* tacHead = NULL;

int count = 2;
bool isRight = true;
char* currentFunctionName = NULL;
char* currentFunctionCallName = NULL;

void semanticAnalysis(ASTNode* node, SymbolBST* symTab, FunctionSymbolBST* functionBST, ArraySymbolTable* arraySymTab) {

    TAC* tac = NULL;

    if (node == NULL) return;
    switch (node->type) {
        case NodeType_Program:
            printf("Starting semantic analysis\n");
            printf("Semantic Analysis running on node of type: NodeType_Program\n");
            semanticAnalysis(node->value.program.FuncDeclList, symTab, functionBST, arraySymTab);
            semanticAnalysis(node->value.program.MainFunc, symTab, functionBST, arraySymTab);
            break;

        case NodeType_FuncDeclList:
            printf("Semantic Analysis running on node of type: NodeType_FuncDeclList\n");
            semanticAnalysis(node->value.FuncDeclList.FuncDecl, symTab, functionBST, arraySymTab);
            semanticAnalysis(node->value.FuncDeclList.nextFuncDecl, symTab, functionBST, arraySymTab);
            break;

        case NodeType_FuncDecl:
            printf("Semantic Analysis running on node of type: NodeType_FuncDecl\n");
            printf("FuncDecl Name: %s\n", node->value.FuncDecl.FuncName);
            printf("FuncDecl Type: %s\n", node->value.FuncDecl.FuncType);

            //Generate TAC early to get the function decl before the body
            tac = generateTACForExpr(node);

            //Check if function already exists with sybolBST specifically for functions
            addFunctionSymbol(functionBST, node->value.FuncDecl.FuncName, node->value.FuncDecl.FuncType);
            printFunctionSymbolTable(functionBST);

            if (currentFunctionName != NULL && strcmp(currentFunctionName, "main") == 0) {
                free(currentFunctionName);
            }

            currentFunctionName = node->value.FuncDecl.FuncName;

            //Create a new symbol table to contain the scope of the next function
            symTab->next = createSymbolBST();
            symTab = symTab->next;

            semanticAnalysis(node->value.FuncDecl.ParamList, symTab, functionBST, arraySymTab);
            semanticAnalysis(node->value.FuncDecl.Body, symTab, functionBST, arraySymTab);

            //Print Function SymbolTable
            printFunctionSymbolTable(functionBST);
            break;

        case NodeType_MainFunc:
            printf("Semantic Analysis running on node of type: NodeType_MainFunc\n");

            //Update current function name for naming variables
            currentFunctionName = strdup("main");

            addFunctionSymbol(functionBST, strdup("main"), strdup("void"));
            printFunctionSymbolTable(functionBST);

            //Generate TAC early to get the function decl before the body
            tac = generateTACForExpr(node);
            
            semanticAnalysis(node->value.MainFunc.Body, symTab, functionBST, arraySymTab);
            break;

        case NodeType_ParamList:
            printf("Semantic Analysis running on node of type: NodeType_ParamList\n");
            semanticAnalysis(node->value.ParamList.ParamDecl, symTab, functionBST, arraySymTab);
            semanticAnalysis(node->value.ParamList.nextParamDecl, symTab, functionBST, arraySymTab);
            break;

        case NodeType_ParamDecl:
            printf("Semantic Analysis running on node of type: NodeType_ParamDecl\n");
            printf("ParamDecl Name: %s\n", node->value.ParamDecl.paramName);
            printf("ParamDecl Type: %s\n", node->value.ParamDecl.paramType);

            // Add the parameter to functionBST to check if the right parameters are being used
            addParameter(functionBST, currentFunctionName, node->value.ParamDecl.paramName, node->value.ParamDecl.paramType);

            // Add the parameters the the symbolBST to be able to check for overlapping delcarations
            addSymbol(symTab, node->value.ParamDecl.paramName, node->value.ParamDecl.paramType);
            printSymbolTable(symTab);
            break;

        case NodeType_Body:
            printf("Semantic Analysis running on node of type: NodeType_Body\n");
            semanticAnalysis(node->value.Body.VarDeclList, symTab, functionBST, arraySymTab);
            semanticAnalysis(node->value.Body.StmtList, symTab, functionBST, arraySymTab);
            semanticAnalysis(node->value.Body.FuncTail, symTab, functionBST, arraySymTab);
            break;

        case NodeType_FuncTail:
            printf("Semantic Analysis running on node of type: NodeType_FuncDecl\n");
            printf("FuncTail type: %s\n", node->value.FuncTail.type);
            semanticAnalysis(node->value.FuncTail.expr, symTab, functionBST, arraySymTab);
            break;

        case NodeType_VarDeclList:
            printf("Semantic Analysis running on node of type: NodeType_VarDeclList\n");
            semanticAnalysis(node->value.VarDeclList.VarDecl, symTab, functionBST, arraySymTab);
            semanticAnalysis(node->value.VarDeclList.nextVarDecl, symTab, functionBST, arraySymTab);
            break;

        case NodeType_VarDecl:
            printf("Semantic Analysis running on node of type: NodeType_VarDecl\n");
            printf("VarDecl Name: %s\n", node->value.VarDecl.varName);
            printf("VarDecl Type: %s\n", node->value.VarDecl.varType);
            addSymbol(symTab, node->value.VarDecl.varName, node->value.VarDecl.varType);
            if (node->value.VarDecl.isArray) {
                printf("Array Size: %d\n", node->value.VarDecl.arraySize);
                addArray(arraySymTab, node->value.VarDecl.varName, node->value.VarDecl.varType, node->value.VarDecl.arraySize);
                printArraySymbolTable(arraySymTab);
            }
            printSymbolTable(symTab);
            semanticAnalysis(node->value.VarDecl.initExpr, symTab, functionBST, arraySymTab);
            break;

        case NodeType_StmtList:
            printf("Semantic Analysis running on node of type: NodeType_StmtList\n");
            semanticAnalysis(node->value.StmtList.stmt, symTab, functionBST, arraySymTab);
            semanticAnalysis(node->value.StmtList.nextStmt, symTab, functionBST, arraySymTab);
            break;

        case NodeType_Assignment:
            printf("Semantic Analysis running on node of type: NodeType_Assignment\n");
            semanticAnalysis(node->value.assignment.expr, symTab, functionBST, arraySymTab);
            break;

        case NodeType_Print:
            printf("Semantic Analysis running on node of type: NodeType_Print\n");
            lookupSymbol(symTab, node->value.print.name);
            break;

        case NodeType_Expression:
            printf("Semantic Analysis running on node of type: NodeType_Expression\n");
            semanticAnalysis(node->value.Expression.right, symTab, functionBST, arraySymTab);
            semanticAnalysis(node->value.Expression.left, symTab, functionBST, arraySymTab);
            break;

        case NodeType_Number:
            printf("Semantic Analysis running on node of type: NodeType_Number\n");
            break;

        case NodeType_Identifier:
            printf("Semantic Analysis running on node of type: NodeType_Identifier\n");
            lookupSymbol(symTab, node->value.identifier.name);
            break;

        case NodeType_BinaryOp:
            printf("Semantic Analysis running on node of type: NodeType_BinaryOp\n");
            break;
            
        case NodeType_FunctionCall:
            printf("Semantic Analysis running on node of type: NodeType_FunctionCall\n");
            // Check if the function exists
            if (!lookupFunctionNode(functionBST, node->value.FunctionCall.funcName)) {
                fprintf(stderr, "Error: Function %s not declared\n", node->value.FunctionCall.funcName);
                exit(1);
            }

            currentFunctionCallName = node->value.FunctionCall.funcName;
            Parameter* currentParameter = lookupFunctionNode(functionBST, node->value.FunctionCall.funcName)->parameters;
            
            bool tempIsRight = isRight;
            tac = generateTACForExpr(node);

            //Semantic Analysis for CallParamList
            ASTNode* node2 = node->value.FunctionCall.CallParamList;
            while (node2 != NULL)
            {
                printf("Semantic Analysis running on node of type: NodeType_CallParamList\n");

                if (currentParameter == NULL) {
                    fprintf(stderr, "Error: Function call had too many inputs. Function call: %s\n", currentFunctionCallName);
                    exit(0);
                }

                if (currentParameter->next == NULL) {
                    currentParameter = NULL;
                } else {
                    currentParameter = currentParameter->next;
                }

                semanticAnalysis(node2->value.CallParamList.expr, symTab, functionBST, arraySymTab);
                semanticAnalysis(node2->value.CallParamList.nextParam, symTab, functionBST, arraySymTab);

                tac = generateTACForExpr(node2);

                node2->value.CallParamList.nextParam == NULL;

                if(node2->value.CallParamList.nextParam == NULL) {
                    node2 = NULL;
                } else {
                    node2 = node2->value.CallParamList.nextParam;
                }
            
            }

            if (currentParameter != NULL) {
                fprintf(stderr, "Error: Function call had too few inputs. Function call: %s\n", currentFunctionCallName);
                exit(0);
            }


            // Generate custome TAC for end of function call
            TAC* instruction = (TAC*)malloc(sizeof(TAC));
            if (!instruction) {
                fprintf(stderr, "Failed to create custom tac instruction for function call: %s\n", currentFunctionCallName);
                exit(0);
            }

            printf("Generating TAC for function call end\n");
            instruction->arg1 = strdup(node->value.FunctionCall.funcName);
            instruction->arg2 = strdup("");
            instruction->op = strdup("FuncCallEnd");
            instruction->result = "";
            if (tempIsRight) {
                instruction->result = strdup("$t1");;
            } else {
                instruction->result = strdup("$t0");
            }
            isRight = false;

            instruction->next = NULL; // Make sure to null-terminate the new instruction

            // Append to the global TAC list
            appendTAC(&tacHead, instruction);

            break;

        case NodeType_ArrayAccess:
            printf("Semantic Analysis running on node of type: NodeType_ArrayAccess\n");
            // Check if the array exists
            if (!lookupSymbol(symTab, node->value.ArrayAccess.varName)) {
                fprintf(stderr, "Error: Array %s not declared\n", node->value.ArrayAccess.varName);
                exit(1);
            }
            // Analyze the index expression
            semanticAnalysis(node->value.ArrayAccess.indexExpr, symTab, functionBST, arraySymTab);
            break;
        default:
            fprintf(stderr, "Unknown Node Type\n");
            printf("%u\n", node->type);
            break;

    }

    if (node->type == NodeType_ParamDecl ||
        node->type == NodeType_FuncTail ||
        node->type == NodeType_VarDecl || 
        node->type == NodeType_Assignment || 
        node->type == NodeType_Expression || 
        node->type == NodeType_Number || 
        node->type == NodeType_Print || 
        node->type == NodeType_Identifier
        ) {
        tac = generateTACForExpr(node);
    }

    return;
}

TAC* generateTACForExpr(ASTNode* expr) {
    if (!expr) return NULL;

    TAC* instruction = (TAC*)malloc(sizeof(TAC));
    if (!instruction) return NULL;

    switch (expr->type) {

        case NodeType_FuncDecl: {
            printf("Generating TAC for function declaration\n");
            instruction->arg1 = strdup(expr->value.FuncDecl.FuncType);
            instruction->arg2 = strdup(expr->value.FuncDecl.FuncName);
            instruction->op = strdup("FuncDecl");
            instruction->result = "";
            isRight = true;
            break;
        }

        case NodeType_MainFunc: {
            printf("Generating TAC for main function\n");
            instruction->arg1 = strdup("Main");
            instruction->arg2 = strdup("");
            instruction->op = strdup("Main");
            instruction->result = "";
            isRight = true;
            break;
        }

        case NodeType_ParamDecl: {
            printf("Generating TAC for parameter declaration\n");
            instruction->arg1 = strdup(expr->value.ParamDecl.paramType);
            instruction->arg2 = strdup(expr->value.ParamDecl.paramName);
            instruction->op = strdup("ParamDecl");
            instruction->result = getVariableReference(expr->value.ParamDecl.paramName);
            break;
        }

        case NodeType_FuncTail: {
            printf("Generating TAC for Function tail\n");
            instruction->arg1 = strdup(expr->value.FuncTail.type);
            instruction->arg2 = strdup("$t1");
            instruction->op = strdup("return");
            instruction->result = strdup("$t1");
            break;
        }

        case NodeType_VarDecl: {
            printf("Generating TAC for variable declaration\n");
            instruction->arg1 = strdup(expr->value.VarDecl.varType);
            instruction->arg2 = strdup(expr->value.VarDecl.varName);
            instruction->op = strdup("VarDecl");
            instruction->result = getVariableReference(expr->value.VarDecl.varName);
            break;
        }

        case NodeType_Assignment: {
            printf("Generating TAC for Assignment\n");
            instruction->arg1 = strdup(expr->value.assignment.varName);
            instruction->arg2 = strdup("$t1");
            instruction->op = strdup("=");
            instruction->result = getVariableReference(expr->value.assignment.varName);
            isRight = true;
            break;
        }

        case NodeType_Print: {
            printf("Generating TAC for print\n");
            instruction->arg1 = strdup(expr->value.print.name);
            instruction->arg2 = NULL;
            instruction->op = strdup("Print");
            instruction->result = getVariableReference(expr->value.print.name);
            break;
        }

        case NodeType_Expression: {
            printf("Generating TAC for expression\n");
            instruction->arg1 = strdup("$t0");
            instruction->arg2 = strdup("$t1");
            instruction->op = strdup("+");
            instruction->result = strdup("$t1");
            break;
        }

        case NodeType_Number: {
            printf("Generating TAC for number\n");
            char buffer[20];
            snprintf(buffer, 20, "%d", expr->value.Number.number);
            instruction->arg1 = strdup(buffer);
            instruction->arg2 = NULL;
            instruction->op = strdup("Num");
            if (isRight) {
                instruction->result = strdup("$t1");
                isRight = false;
            } else {
                instruction->result = strdup("$t0");
            }
            break;
        }

        case NodeType_Identifier: {
            printf("Generating TAC for identifier\n");
            printf("Identifier: %s\n", expr->value.identifier.name);
            instruction->arg1 = strdup(expr->value.identifier.name);
            // Line below causes seg fault with inpput file 2
            instruction->arg2 = getVariableReference(expr->value.identifier.name);
            instruction->op = strdup("ID");

            if (isRight) {
                instruction->result = strdup("$t1");
                isRight = false;
            } else {
                instruction->result = strdup("$t0");
            }
            break;
        }

        case NodeType_FunctionCall: {
            printf("Generating TAC for Function Call\n");
            instruction->arg1 = strdup(expr->value.FunctionCall.funcName);
            instruction->arg2 = strdup("");
            instruction->op = strdup("FuncCall");

            if (isRight) {
                instruction->result = strdup("$t1");
            } else {
                instruction->result = strdup("$t0");
            }
            isRight = true;
            break;
        }

        case NodeType_CallParamList: {
            if(expr->value.CallParamList.expr != NULL) {
                printf("Generating TAC for Call Parameter\n");
                instruction->arg1 = strdup("$t1");
                instruction->arg2 = strdup("");
                instruction->op = strdup("ParamCall");
                instruction->result = strdup("");
                isRight = true;
            }
            break;
        }

        default:
            free(instruction);
            return NULL;
    }

    instruction->next = NULL; // Make sure to null-terminate the new instruction

    // Append to the global TAC list
    appendTAC(&tacHead, instruction);

    return instruction;
}
char* getVariableReference(char* variable) {
    if (!variable) {
        printf("ERROR: Null variable name passed to getVariableReference\n");
        exit(0);
    }

    if (!currentFunctionName) {
        printf("ERROR: Null function name when getVariableReference was envoked\n");
        exit(0);
    }

    return  strcat(strdup(currentFunctionName), variable);
}

// TODO: make algo for if the number of temperary registers is exceeded
char* createTempVar() {
    char* tempVar = malloc(10); // Enough space for "t" + number
    sprintf(tempVar, "$t%d", count++);
    return tempVar;
}

void printTAC(TAC* tac) {
    if (strcmp(tac->op, "FuncDecl") == 0) {
        printf("Function: %s %s %s:\n", tac->arg1, tac->arg2, tac->result);
    } else if (strcmp(tac->op, "Main") == 0) {
        printf("Main Function:\n");
    } else if (strcmp(tac->op, "ParamDecl") == 0) {
        printf("\tParameter: %s %s ==> %s\n", tac->arg1, tac->arg2, tac->result);
    } else if (strcmp(tac->op, "return") == 0) {
        printf("\tReturn: %s %s\n\n", tac->arg1, tac->arg2);
    } else if (strcmp(tac->op, "VarDecl") == 0) {
        printf("\tVariable: %s %s ==> %s\n", tac->arg1, tac->arg2, tac->result);
    } else if (strcmp(tac->op, "=") == 0) {
        printf("\t%s (%s) = %s\n", tac->result, tac->arg1, tac->arg2);
    } else if (strcmp(tac->op, "Print") == 0) {
        printf("\tPrint(%s (%s))\n", tac->result, tac->arg1);
    } else if (strcmp(tac->op, "+") == 0) {
        printf("\t%s = %s + %s\n", tac->result, tac->arg1, tac->arg2);
    } else if (strcmp(tac->op, "-") == 0) {
        printf("\t%s = %s - %s\n", tac->result, tac->arg1, tac->arg2);
    } else if (strcmp(tac->op, "*") == 0) {
        printf("\t%s = %s * %s\n", tac->result, tac->arg1, tac->arg2);
    } else if (strcmp(tac->op, "/") == 0) {
        printf("\t%s = %s / %s\n", tac->result, tac->arg1, tac->arg2);
    } else if (strcmp(tac->op, "Num") == 0) {
        printf("\t%s = %s\n", tac->result, tac->arg1);
    } else if (strcmp(tac->op, "ID") == 0) {
        printf("\t%s = %s (%s)\n", tac->result, tac->arg2, tac->arg1);
    }  else if (strcmp(tac->op, "FuncCall") == 0) {
        printf("\tFunction Call: %s => %s\n", tac->arg1, tac->result);
    }  else if (strcmp(tac->op, "FuncCallEnd") == 0) {
        printf("\tFunction Call End: %s => %s\n", tac->arg1, tac->result);
    }else if (strcmp(tac->op, "ParamCall") == 0) {
        printf("\tParameter Call: %s\n", tac->arg1);
    }
}

void printTACToFile(const char* filename, TAC* tac) {
    FILE* file = fopen(filename, "w");
    if (!file) {
        perror("Failed to open file");
        return;
    }   
    TAC* current = tac;
    while (current != NULL) {
        if (strcmp(current->op, "VarDecl") == 0) {
            fprintf(file, "%s %s ==> %s\n", current->arg1, current->arg2, current->result);
        } else if (strcmp(current->op, "=") == 0) {
            fprintf(file, "%s (%s) = %s\n", current->result, current->arg1, current->arg2);
        } else if (strcmp(current->op, "Print") == 0) {
            fprintf(file, "Print(%s (%s))\n", current->result, current->arg1);
        } else if (strcmp(current->op, "+") == 0) {
            fprintf(file, "%s = %s + %s\n", current->result, current->arg1, current->arg2);
        } else if (strcmp(current->op, "-") == 0) {
            fprintf(file, "%s = %s - %s\n", current->result, current->arg1, current->arg2);
        } else if (strcmp(current->op, "*") == 0) {
            fprintf(file, "%s = %s * %s\n", current->result, current->arg1, current->arg2);
        } else if (strcmp(current->op, "/") == 0) {
            fprintf(file, "%s = %s / %s\n", current->result, current->arg1, current->arg2);
        } else if (strcmp(current->op, "Num") == 0) {
            fprintf(file, "%s = %s\n", current->result, current->arg1);
        } else if (strcmp(current->op, "ID") == 0) {
            fprintf(file, "%s = %s (%s)\n", current->result, current->arg2, current->arg1);
        }
        current = current->next;
    }   
    fclose(file);
    printf("TAC written to %s\n", filename);
}


void appendTAC(TAC** head, TAC* newInstruction) {
    if (!*head) {
        *head = newInstruction;
    } else {
        TAC* current = *head;
        while (current->next) {
            current = current->next;
        }
        current->next = newInstruction;
    }
}
