#ifndef FUNCTION_SYMBOL_BST1_H
#define FUNCTION_SYMBOL_BST1_H

#include <stdlib.h>
#include <string.h>
#include "symbolBST.h"

typedef struct Parameter {
    char* name;
    char* type;
    struct Parameter* next;
} Parameter;

// Define the FunctionSymbolTable struct
typedef struct FunctionSymbolBST {
    struct Symbol* symbol;
    struct Parameter* parameters;
    unsigned int hash;
    struct FunctionSymbolBST* right;
    struct FunctionSymbolBST* left;
} FunctionSymbolBST;

// Function declarations
FunctionSymbolBST* createFunctionSymbolBST();
void addFunctionSymbol(FunctionSymbolBST* head, char* name, char* type);
FunctionSymbolBST* lookupFunctionNode(FunctionSymbolBST* head, char* name);
void addParameter(FunctionSymbolBST* head, char* functionName, char* parameterName, char* parameterType);
void addParameterInternal(FunctionSymbolBST* node, Parameter* parameter, unsigned int curHash);
void addParamtersToFunctionInternal(FunctionSymbolBST* node,  Parameter* parameter);
void freeFunctionSymbolTable(FunctionSymbolBST* head);
void printFunctionSymbolTable(FunctionSymbolBST* node);
void printFunctionSymbolTablePrivateRight(FunctionSymbolBST* curNode, int indent);
void printFunctionSymbolTablePrivateLeft(FunctionSymbolBST* curNode, int indent);


#endif // FUNCTION_SYMBOL_BST1_H