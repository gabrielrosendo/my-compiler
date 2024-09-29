#ifndef SEMANTIC_H
#define SEMANTIC_H

#include "AST.h"         
#include "symbolBST.h" // Ensure this header defines symbolBST, or include the correct header file

typedef struct TAC {
    char *op;
    char *arg1;
    char *arg2;
    char *result;
    struct TAC *next;
} TAC;

extern TAC* tacHead;

// Temporary variable management
#define MAX_TEMP_VARS 20
extern int tempVars[MAX_TEMP_VARS];

void initializeTempVars();
int allocateNextAvailableTempVar();
void deallocateTempVar(int index);

// Semantic analysis functions
void semanticAnalysis(ASTNode* node, SymbolBST* symTab);
TAC* generateTACForExpr(ASTNode* expr);
char* getVariableReference(char* variable);
char* createTempVar();
char* createOperand(ASTNode* node);
void appendTAC(TAC** head, TAC* newInstruction);
void printTAC(TAC* tac);
void printTACToFile(const char* filename, TAC* tac);


#endif // SEMANTIC_H

