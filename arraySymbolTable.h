#ifndef ARRAY_SYMBOL_TABLE_H
#define ARRAY_SYMBOL_TABLE_H

#include <stdbool.h>

#define HASH_TABLE_SIZE 101  // Prime number for better distribution

typedef struct ArraySymbol {
    char* name;         // Name of the array
    char* elementType;  // Type of array elements (int, char, etc.)
    int size;          // Number of elements
    int baseAddress;   // Base address in memory
    struct ArraySymbol* next;  // For handling collisions via chaining
} ArraySymbol;

typedef struct {
    ArraySymbol* buckets[HASH_TABLE_SIZE];
    int totalArrays;    // Track total number of arrays
    int totalMemory;    // Track total memory allocated
} ArraySymbolTable;

// Core functions
ArraySymbolTable* createArraySymbolTable();
void freeArraySymbolTable(ArraySymbolTable* table);
bool addArray(ArraySymbolTable* table, char* name, char* elementType, int size);
ArraySymbol* findArray(ArraySymbolTable* table, char* name);
void removeArray(ArraySymbolTable* table, char* name);

// Utility functions
void printArraySymbolTable(ArraySymbolTable* table);
bool isArrayDeclared(ArraySymbolTable* table, char* name);
int getArraySize(ArraySymbolTable* table, char* name);
int getArrayBaseAddress(ArraySymbolTable* table, char* name);
bool validateArrayAccess(ArraySymbolTable* table, char* name, int index);

#endif // ARRAY_SYMBOL_TABLE_H

