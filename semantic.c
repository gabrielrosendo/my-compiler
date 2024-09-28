#include "semantic.h"
#include <stdio.h>

// Initialize the global TAC list
TAC* tacHead = NULL;

// Initialize the temporary variables   
int tempVars[MAX_TEMP_VARS];

void semanticAnalysis(ASTNode* node, SymbolBST* symTab) {
    if (node == NULL) return;

    switch (node->type) {
        case NodeType_Program:
            printf("Starting semantic analysis\n");
            printf("S\n");
            semanticAnalysis(node->value.program.VarDeclList, symTab);
            semanticAnalysis(node->value.program.StmtList, symTab);
            break;
        case NodeType_VarDeclList:
            semanticAnalysis(node->value.VarDeclList.VarDecl, symTab);
            semanticAnalysis(node->value.VarDeclList.nextVarDecl, symTab);
            break;
        case NodeType_VarDecl:
        // Maybe change calls to symbol table from parser to here?
            if (lookupSymbol(symTab, node->value.VarDecl.varName) != NULL) {
                fprintf(stderr, "Semantic error: Variable %s has already been declared\n", node->value.VarDecl.varName);
            }
            break;
        case NodeType_StmtList:
            semanticAnalysis(node->value.StmtList.stmt, symTab);
            semanticAnalysis(node->value.StmtList.nextStmt, symTab);
            break;
        case NodeType_Stmt:
            // TO DO: Implement
            break;
        case NodeType_Expression:
            // TO DO: Implement
            break;
        case NodeType_Identifier:
            // TO DO: Implement
            break;
        case NodeType_BinaryOp:
            // TO DO: Implement
            break;
        case NodeType_Print:
            // TO DO: Implement
            break;
        case NodeType_Assignment:
            // TO DO: Implement
            break;

        default:
            fprintf(stderr, "Unknown Node Type\n");
    }
    // Generate TAC for relevant node types
    if (node->type == NodeType_Expression || 
        node->type == NodeType_BinaryOp || 
        node->type == NodeType_Assignment || 
        node->type == NodeType_Print) {
        TAC* tac = generateTACForExpr(node);
        // Process or store the generated TAC
        printTAC(tac);
    }
}

// Implement all functions in AST.h

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




