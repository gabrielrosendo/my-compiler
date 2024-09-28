#include "semantic.h"
#include <stdio.h>

// Initialize the global TAC list
TAC* tacHead = NULL;

// Initialize the temporary variables   
int tempVars[MAX_TEMP_VARS];

void initializeTempVars() {
    for (int i = 0; i < MAX_TEMP_VARS; i++) {
        tempVars[i] = 0;
    }
}

void semanticAnalysis(ASTNode* node, SymbolBST* symTab) {
    if (node == NULL) return;

    switch (node->type) {
        case NodeType_Program:
            printf("Starting semantic analysis\n");
            semanticAnalysis(node->value.program.VarDeclList, symTab);
            semanticAnalysis(node->value.program.StmtList, symTab);
            break;
        // TO DO: Implement other cases (VarDeclList, StmtList, ...)
        // Check names for node typed in AST.h
        // Not the same as Artzi's
}

