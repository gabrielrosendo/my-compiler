#include "arraySymbolTable.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Hash function
static unsigned int hash(char* str) {
    unsigned int hash = 5381;
    int c;

    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c; // hash * 33 + c
    }

    return hash % HASH_TABLE_SIZE;
}

// Create new array symbol table
ArraySymbolTable* createArraySymbolTable() {
    ArraySymbolTable* table = (ArraySymbolTable*)malloc(sizeof(ArraySymbolTable));
    if (table == NULL) {
        fprintf(stderr, "Error: Failed to allocate memory for array symbol table\n");
        exit(1);
    }
    
    // Initialize all buckets to NULL
    for (int i = 0; i < HASH_TABLE_SIZE; i++) {
        table->buckets[i] = NULL;
    }
    
    table->totalArrays = 0;
    table->totalMemory = 0;
    return table;
}

// Create new array symbol
static ArraySymbol* createArraySymbol(char* name, char* elementType, int size) {
    ArraySymbol* symbol = (ArraySymbol*)malloc(sizeof(ArraySymbol));
    if (symbol == NULL) {
        fprintf(stderr, "Error: Failed to allocate memory for array symbol\n");
        exit(1);
    }
    
    symbol->name = strdup(name);
    symbol->elementType = strdup(elementType);
    symbol->size = size;
    symbol->baseAddress = -1;  // Will be set during code generation
    symbol->next = NULL;
    
    return symbol;
}

// Add array to symbol table
bool addArray(ArraySymbolTable* table, char* name, char* elementType, int size) {
    if (table == NULL || name == NULL || elementType == NULL || size <= 0) {
        return false;
    }

    // Check if array already exists
    if (findArray(table, name) != NULL) {
        return false;
    }

    // Create new array symbol
    ArraySymbol* newSymbol = createArraySymbol(name, elementType, size);
    
    // Calculate hash and insert into table
    unsigned int index = hash(name);
    newSymbol->next = table->buckets[index];
    table->buckets[index] = newSymbol;
    
    table->totalArrays++;
    table->totalMemory += (size * 4); // Assuming 4 bytes per element
    return true;
}

// Find array in symbol table
ArraySymbol* findArray(ArraySymbolTable* table, char* name) {
    if (table == NULL || name == NULL) {
        return NULL;
    }
    
    unsigned int index = hash(name);
    ArraySymbol* current = table->buckets[index];
    
    while (current != NULL) {
        if (strcmp(current->name, name) == 0) {
            return current;
        }
        current = current->next;
    }
    
    return NULL;
}

// Remove array from symbol table
void removeArray(ArraySymbolTable* table, char* name) {
    if (table == NULL || name == NULL) {
        return;
    }
    
    unsigned int index = hash(name);
    ArraySymbol* current = table->buckets[index];
    ArraySymbol* prev = NULL;
    
    while (current != NULL) {
        if (strcmp(current->name, name) == 0) {
            if (prev == NULL) {
                table->buckets[index] = current->next;
            } else {
                prev->next = current->next;
            }
            
            table->totalMemory -= (current->size * 4);
            table->totalArrays--;
            
            free(current->name);
            free(current->elementType);
            free(current);
            return;
        }
        prev = current;
        current = current->next;
    }
}

// Validate array access
bool validateArrayAccess(ArraySymbolTable* table, char* name, int index) {
    ArraySymbol* symbol = findArray(table, name);
    if (symbol == NULL) {
        fprintf(stderr, "Error: Array '%s' not declared\n", name);
        return false;
    }
    
    if (index < 0 || index >= symbol->size) {
        fprintf(stderr, "Error: Array index %d out of bounds for array '%s' of size %d\n",
                index, name, symbol->size);
        return false;
    }
    
    return true;
}

// Print array symbol table
void printArraySymbolTable(ArraySymbolTable* table) {
    if (table == NULL) {
        printf("Array Symbol Table is NULL\n");
        return;
    }
    
    printf("\n=== Array Symbol Table ===\n");
    printf("Total Arrays: %d\n", table->totalArrays);
    printf("Total Memory: %d bytes\n", table->totalMemory);
    printf("\nArrays:\n");
    
    for (int i = 0; i < HASH_TABLE_SIZE; i++) {
        ArraySymbol* current = table->buckets[i];
        while (current != NULL) {
            printf("\nBucket %d:\n", i);
            printf("  Name: %s\n", current->name);
            printf("  Type: %s\n", current->elementType);
            printf("  Size: %d\n", current->size);
            printf("  Base Address: %d\n", current->baseAddress);
            current = current->next;
        }
    }
    printf("=====================\n\n");
}

// Free array symbol table
void freeArraySymbolTable(ArraySymbolTable* table) {
    if (table == NULL) return;
    
    for (int i = 0; i < HASH_TABLE_SIZE; i++) {
        ArraySymbol* current = table->buckets[i];
        while (current != NULL) {
            ArraySymbol* next = current->next;
            free(current->name);
            free(current->elementType);
            free(current);
            current = next;
        }
    }
    
    free(table);
}
