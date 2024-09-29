#include "semantic.h"
#include "symbolBST.h"
#include <stdio.h>

// Initialize the global TAC list
TAC* tacHead = NULL;

// Initialize the temporary variables   
int tempVars[MAX_TEMP_VARS];

void semanticAnalysis(ASTNode* node, SymbolBST* symTab) {
    if (node == NULL) return;
    printf("semanticAnalysis() start\n");
    switch (node->type) {
        case NodeType_Program:
            printf("Starting semantic analysis\n");
            printf("Semantic Analysis running on node of type: NodeType_Program\n");
            semanticAnalysis(node->value.program.VarDeclList, symTab);
            printf("WHAT1\n");
            semanticAnalysis(node->value.program.StmtList, symTab);
            break;
        case NodeType_VarDeclList:
            printf("Semantic Analysis running on node of type: NodeType_VarDeclList\n");
            semanticAnalysis(node->value.VarDeclList.VarDecl, symTab);
            printf("WHAT2\n");
            semanticAnalysis(node->value.VarDeclList.nextVarDecl, symTab);
            break;
        case NodeType_VarDecl:
            printf("Semantic Analysis running on node of type: NodeType_VarDecl\n");
            printf("VarDecl Name: %s\n", node->value.VarDecl.varName);
            printf("VarDecl Type: %s\n", node->value.VarDecl.varType);
            addSymbol(symTab, node->value.VarDecl.varName, node->value.VarDecl.varType);
            printSymbolTable(symTab);
            semanticAnalysis(node->value.VarDecl.initExpr, symTab);
            break;
        case NodeType_StmtList:
            printf("Semantic Analysis running on node of type: NodeType_StmtList\n");
            semanticAnalysis(node->value.StmtList.stmt, symTab);
            printf("WHAT3: %s\n", node->value.StmtList.nextStmt);
            semanticAnalysis(node->value.StmtList.nextStmt, symTab);
            break;
        case NodeType_Assignment:
            printf("Semantic Analysis running on node of type: NodeType_Assignment\n");
            semanticAnalysis(node->value.assignment.expr, symTab);
            break;
        case NodeType_Print:
            printf("Semantic Analysis running on node of type: NodeType_Print\n");
            semanticAnalysis(node->value.print.expr, symTab);
            break;
        case NodeType_Expression:
            printf("Semantic Analysis running on node of type: NodeType_Expression\n");
            semanticAnalysis(node->value.Expression.right, symTab);
            printf("WHAT4\n");
            semanticAnalysis(node->value.Expression.left, symTab);
            break;
        case NodeType_Number:
            printf("Semantic Analysis running on node of type: NodeType_Number\n");
            break;
        case NodeType_Identifier:
            printf("Semantic Analysis running on node of type: NodeType_Identifier\n");
            break;
        case NodeType_BinaryOp:
            printf("Semantic Analysis running on node of type: NodeType_BinaryOp\n");
            break;
        default:
            fprintf(stderr, "Unknown Node Type\n");
            printf("%s\n", node->type);
            break;
    }

    printf("semanticAnalysis() - made it outside of switch\n");

    return;
}

void printTAC(TAC* tac) {
    if (!tac) return;

    // Print TAC instruction

}

TAC* generateTACForExpr(ASTNode* expr) {

}

void initializeTempVars() {
    for (int i = 0; i < MAX_TEMP_VARS; i++) {
        tempVars[i] = 0;
    }
}




