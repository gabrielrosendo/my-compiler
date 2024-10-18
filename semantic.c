#include <stdio.h>
#include <stdbool.h>
#include "semantic.h"
#include "symbolBST.h"

// Initialize the global TAC list
TAC* tacHead = NULL;

int count = 2;
bool isRight = true;

void semanticAnalysis(ASTNode* node, SymbolBST* symTab, SymbolBST* functionBST) {
    if (node == NULL) return;
    switch (node->type) {
        case NodeType_Program:
            printf("Starting semantic analysis\n");
            printf("Semantic Analysis running on node of type: NodeType_Program\n");
            semanticAnalysis(node->value.program.FuncDeclList, symTab, functionBST);
            semanticAnalysis(node->value.program.MainFunc, symTab, functionBST);
            break;
        case NodeType_FuncDeclList:
            printf("Semantic Analysis running on node of type: NodeType_FuncDeclList\n");
            semanticAnalysis(node->value.FuncDeclList.FuncDecl, symTab, functionBST);
            semanticAnalysis(node->value.FuncDeclList.nextFuncDecl, symTab, functionBST);
            break;
        case NodeType_FuncDecl:
            printf("Semantic Analysis running on node of type: NodeType_FuncDecl\n");
            printf("FuncDecl Name: %s\n", node->value.FuncDecl.FuncName);
            printf("FuncDecl Type: %s\n", node->value.FuncDecl.FuncType);

            //Check if function already exists with sybolBST specifically for functions
            addSymbol(functionBST, node->value.FuncDecl.FuncName, node->value.FuncDecl.FuncType);
            printSymbolTable(functionBST);

            //Create a new symbol table to contain the scope of the next function
            symTab->next = createSymbolBST();
            symTab = symTab->next;

            semanticAnalysis(node->value.FuncDecl.ParamList, symTab, functionBST);
            semanticAnalysis(node->value.FuncDecl.Body, symTab, functionBST);
            break;
        case NodeType_MainFunc:
            printf("Semantic Analysis running on node of type: NodeType_MainFunc\n");
            semanticAnalysis(node->value.MainFunc.Body, symTab, functionBST);
            break;
        case NodeType_ParamList:
            printf("Semantic Analysis running on node of type: NodeType_ParamList\n");
            semanticAnalysis(node->value.ParamList.ParamDecl, symTab, functionBST);
            semanticAnalysis(node->value.ParamList.nextParamDecl, symTab, functionBST);
            break;
        case NodeType_ParamDecl:
            printf("Semantic Analysis running on node of type: NodeType_ParamDecl\n");
            printf("ParamDecl Name: %s\n", node->value.ParamDecl.paramName);
            printf("ParamDecl Type: %s\n", node->value.ParamDecl.paramType);

            // Add the parameters the the AST to be able to check for overlapping delcarations
            addSymbol(symTab, node->value.ParamDecl.paramName, node->value.ParamDecl.paramType);
            printSymbolTable(symTab);
            break;
        case NodeType_Body:
            printf("Semantic Analysis running on node of type: NodeType_Body\n");
            semanticAnalysis(node->value.Body.VarDeclList, symTab, functionBST);
            semanticAnalysis(node->value.Body.StmtList, symTab, functionBST);
            break;
        case NodeType_VarDeclList:
            printf("Semantic Analysis running on node of type: NodeType_VarDeclList\n");
            semanticAnalysis(node->value.VarDeclList.VarDecl, symTab, functionBST);
            semanticAnalysis(node->value.VarDeclList.nextVarDecl, symTab, functionBST);
            break;
        case NodeType_VarDecl:
            printf("Semantic Analysis running on node of type: NodeType_VarDecl\n");
            printf("VarDecl Name: %s\n", node->value.VarDecl.varName);
            printf("VarDecl Type: %s\n", node->value.VarDecl.varType);
            addSymbol(symTab, node->value.VarDecl.varName, node->value.VarDecl.varType);
            printSymbolTable(symTab);
            semanticAnalysis(node->value.VarDecl.initExpr, symTab, functionBST);
            break;
        case NodeType_StmtList:
            printf("Semantic Analysis running on node of type: NodeType_StmtList\n");
            semanticAnalysis(node->value.StmtList.stmt, symTab, functionBST);
            semanticAnalysis(node->value.StmtList.nextStmt, symTab, functionBST);
            break;
        case NodeType_Assignment:
            printf("Semantic Analysis running on node of type: NodeType_Assignment\n");
            semanticAnalysis(node->value.assignment.expr, symTab, functionBST);
            break;
        case NodeType_Print:
            printf("Semantic Analysis running on node of type: NodeType_Print\n");
            break;
        case NodeType_Expression:
            printf("Semantic Analysis running on node of type: NodeType_Expression\n");
            semanticAnalysis(node->value.Expression.right, symTab, functionBST);
            semanticAnalysis(node->value.Expression.left, symTab, functionBST);
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
        default:
            fprintf(stderr, "Unknown Node Type\n");
            printf("%u\n", node->type);
            break;

    }

    if (node->type == NodeType_VarDecl || 
        node->type == NodeType_Assignment || 
        node->type == NodeType_Expression || 
        node->type == NodeType_Number || 
        node->type == NodeType_Print || 
        node->type == NodeType_Identifier) {
        TAC* tac = generateTACForExpr(node);
    }

    return;
}

TAC* generateTACForExpr(ASTNode* expr) {
    if (!expr) return NULL;

    TAC* instruction = (TAC*)malloc(sizeof(TAC));
    if (!instruction) return NULL;

    switch (expr->type) {
        case NodeType_VarDecl: {
            printf("Generating TAC for variable declaration\n");
            instruction->arg1 = strdup(expr->value.VarDecl.varType);
            instruction->arg2 = strdup(expr->value.VarDecl.varName);
            instruction->op = strdup("VarDecl");
            instruction->result = createTempVar();
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
            instruction->arg1 = strdup(expr->value.identifier.name);
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

        case NodeType_FuncDeclList: {
            printf("Generating TAC for function declaration list\n");
            // Generate TAC for each function declaration in the list
            ASTNode* funcDecl = expr->value.FuncDeclList.FuncDecl;
            while (funcDecl) {
                generateTACForExpr(funcDecl);
                funcDecl = funcDecl->value.FuncDeclList.nextFuncDecl;
            }
            break;
        }

        case NodeType_FuncDecl: {
            printf("Generating TAC for function declaration\n");
            // Generate TAC for the function body
            generateTACForExpr(expr->value.FuncDecl.Body);
            break;
        }

        case NodeType_MainFunc: {
            printf("Generating TAC for main function\n");
            // Generate TAC for the main function body
            generateTACForExpr(expr->value.MainFunc.Body);
            break;
        }

        // Add cases for other expression types...

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
    TAC* current = tacHead;
    while(current != NULL) {
        if (strcmp(current->arg2, variable) == 0) {
            return current->result;
            break;
        }
        current = current->next;
    }
    printf("ERROR: Could not find variable %s in TAC, exiting program.", variable);
    exit(0);
}

// TODO: make algo for if the number of temperary registers is exceeded
char* createTempVar() {
    char* tempVar = malloc(10); // Enough space for "t" + number
    sprintf(tempVar, "$t%d", count++);
    return tempVar;
}

void printTAC(TAC* tac) {
    if (strcmp(tac->op, "VarDecl") == 0) {
        printf("%s %s ==> %s\n", tac->arg1, tac->arg2, tac->result);
    } else if (strcmp(tac->op, "=") == 0) {
        printf("%s (%s) = %s\n", tac->result, tac->arg1, tac->arg2);
    } else if (strcmp(tac->op, "Print") == 0) {
        printf("Print(%s (%s))\n", tac->result, tac->arg1);
    } else if (strcmp(tac->op, "+") == 0) {
        printf("%s = %s + %s\n", tac->result, tac->arg1, tac->arg2);
    } else if (strcmp(tac->op, "-") == 0) {
        printf("%s = %s - %s\n", tac->result, tac->arg1, tac->arg2);
    } else if (strcmp(tac->op, "*") == 0) {
        printf("%s = %s * %s\n", tac->result, tac->arg1, tac->arg2);
    } else if (strcmp(tac->op, "/") == 0) {
        printf("%s = %s / %s\n", tac->result, tac->arg1, tac->arg2);
    }
    else if (strcmp(tac->op, "Num") == 0) {
        printf("%s = %s\n", tac->result, tac->arg1);
    } else if (strcmp(tac->op, "ID") == 0) {
        printf("%s = %s (%s)\n", tac->result, tac->arg2, tac->arg1);
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
