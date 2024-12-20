#ifndef SEMANTIC_H
#define SEMANTIC_H

#include "AST.h"         
#include "symbolBST.h" // Ensure this header defines symbolBST, or include the correct header file
#include "functionSymbolBST.h"
#include "arraySymbolTable.h" // Ensure this header defines ArraySymbolTable, or include the correct header file


typedef struct TAC {
    char *op;
    char *arg1;
    char *arg2;
    unsigned int arg3;
    char arg4;
    char *result;
    struct TAC *next;
    int label;
} TAC;

extern TAC* tacHead;

// Temporary variable management
#define MAX_TEMP_VARS 20
extern int tempVars[MAX_TEMP_VARS];

void initializeTempVars();
int allocateNextAvailableTempVar();
void deallocateTempVar(int index);

// Semantic analysis functions
void semanticAnalysis(ASTNode* node, SymbolBST* symTab, FunctionSymbolBST* functionBST, ArraySymbolTable* arraySymTab);
TAC* generateTACForExpr(ASTNode* expr);
char* getVariableReference(char* variable);
char* createTempVar();
char* createOperand(ASTNode* node);
void appendTAC(TAC** head, TAC* newInstruction);
void printTAC(TAC* tac);
void printTACToFile(const char* filename, TAC* tac);
void moveRegisters(char* from, char* to);
char* processExpressionTypes(char* type1, char* type2);
void TACConvertIntToFloat(char* curRegister);
void TACConvertFloatToInt(char* curRegister);
void semanticAnalysisForIfStatements(ASTNode* node, SymbolBST* symTab, FunctionSymbolBST* functionBST, ArraySymbolTable* arraySymTab, char* finalJumpLoc);
void TACForIfStart(ASTNode* node, char* jumpLoc);
void TACForIfEnd(ASTNode* node, char* ifJumpLoc1, char* ifJumpFinalLoc);
void TACForIfSkip(char* ifJumpLoc2);
void TACForWhileLoopStart(char* whileJumpLocStart, char* whileJumpLocEnd);
void TACForWhileLoopEnd(char* whileJumpLocStart, char* whileJumpLocEnd);

#endif // SEMANTIC_H

