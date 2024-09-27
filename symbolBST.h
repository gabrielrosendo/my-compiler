#ifndef SYMBOL_BST1_H
#define SYMBOL_BST1_H

#include <stdlib.h>
#include <string.h>

// Symbol structure

// Define the Symbol struct
typedef struct Symbol {
    char* name;
    char* type;
    struct Symbol* next;
    // Add other fields of Symbol
} Symbol;

// Define the SymbolTable struct
typedef struct SymbolBST {
    struct Symbol* symbol;
    unsigned int hash;
    struct SymbolBST* right;
    struct SymbolBST* left;
    struct SymbolBST* previous;
} SymbolBST;

// Function declarations
SymbolBST* createSymbolBST();
void addSymbol(SymbolBST* head, char* name, char* type);
Symbol* lookupSymbol(SymbolBST* head, char* name);
void freeSymbolTable(SymbolBST* head);
void printSymbolTable(SymbolBST* head);  // For debugging
void printSymbolTablePrivateLeft(SymbolBST* curNode, int indent);


#endif // SYMBOL_BST1_H
