#ifndef FUNCTION_SYMBOL_BST1_H
#define FUNCTION_SYMBOL_BST1_H

#include <stdlib.h>
#include <string.h>

// Symbol structure

// Define the Symbol struct
typedef struct Symbol {
    char* name;
    char* type;
} Symbol;

typedef struct Parameter {
    char* name;
    char* type;
    Parameter* next;
} Parameter;

// Define the FunctionSymbolTable struct
typedef struct FunctionSymbolBST {
    struct Symbol* symbol;
    unsigned int hash;
    struct FunctionSymbolBST* right;
    struct FunctionSymbolBST* left;
} FunctionSymbolBST;

// Function declarations
FunctionSymbolBST* createSymbolBST();
void addSymbol(FunctionSymbolBST* head, char* name, char* type);
Symbol* lookupSymbol(FunctionSymbolBST* head, char* name);
void addParameter(FunctionSymbolBST* head, char* functionName, char* parameterName, char* parameterType);
void freeSymbolTable(FunctionSymbolBST* head);
void printSymbolTable(FunctionSymbolBST* node);  // For debugging
void printSymbolTablePrivateLeft(FunctionSymbolBST* curNode, int indent);


#endif // FUNCTION_SYMBOL_BST1_H
