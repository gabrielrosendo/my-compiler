#include "symbolBST_Test.h"
#include "symbolBST.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void symbolBST_TestCreation() {

    printf("\n---------------------------------------------------------\n");
    printf("\nTesting: symbolBST_TestCreation()\n");

    SymbolBST* symbolBST = createSymbolBST();
    printf("Test Log: created SymbolBST\n");
    printSymbolTable(symbolBST);
}

void symbolBST_Test_AddSymbol_SingleIntegerInput() {

    printf("\n---------------------------------------------------------\n");
    printf("\nTesting: symbolBST_Test_AddSymbol_SingleIntegerInput()\n");

    SymbolBST* symbolBST = createSymbolBST();
    printf("Test Log: created SymbolBST\n");

    addSymbol(symbolBST, "Test", "INT");
    printf("Test Log: added integer variable into SymbolBST\n");
    printSymbolTable(symbolBST);
}

void symbolBST_Test_AddSymbol_MultipleIntegerInputs() {

    printf("\n---------------------------------------------------------\n");
    printf("\nTesting: symbolBST_Test_AddSymbol_MultipleIntegerInputs()\n");

    SymbolBST* symbolBST = createSymbolBST();
    printf("Test Log: created SymbolBST\n");

    addSymbol(symbolBST, "Test2", "INT");
    printf("Test Log: added integer variable into SymbolBST\n");
    printSymbolTable(symbolBST);

    addSymbol(symbolBST, "Test4", "INT");
    printf("Test Log: added integer variable into SymbolBST\n");
    printSymbolTable(symbolBST);

    addSymbol(symbolBST, "Test1", "INT");
    printf("Test Log: added integer variable into SymbolBST\n");
    printSymbolTable(symbolBST);

    addSymbol(symbolBST, "Test3", "INT");
    printf("Test Log: added integer variable into SymbolBST\n");
    printSymbolTable(symbolBST);

    addSymbol(symbolBST, "Test5", "INT");
    printf("Test Log: added integer variable into SymbolBST\n");
    printSymbolTable(symbolBST);

    addSymbol(symbolBST, "Test6", "INT");
    printf("Test Log: added integer variable into SymbolBST\n");
    printSymbolTable(symbolBST);
}

void symbolBST_Test_GetSymbol_EmptySymbolBST() {
    printf("\n---------------------------------------------------------\n");
    printf("\nTesting: symbolBST_Test_GetSymbol_EmptySymbolBST()\n");

    SymbolBST* symbolBST = createSymbolBST();
    printf("Test Log: created SymbolBST\n");

    lookupSymbol(symbolBST, "NULL");
    printf("Test Log: looked up symbol \"NULL\"\n");
}

void printSymbol(Symbol* symbol) {
    printf("Found:\n");
    printf("Name: %s\n", symbol->name);
    printf("Type: %s\n", symbol->type);
}

void symbolBST_Test_GetSymbol_FoundAll() {
    
    printf("\n---------------------------------------------------------\n");
    printf("\nTesting:symbolBST_Test_GetSymbol_FoundAll()\n");

    SymbolBST* symbolBST = createSymbolBST();
    printf("Test Log: created SymbolBST\n");

    addSymbol(symbolBST, "Test2", "INT");
    printf("Test Log: added integer variable into SymbolBST\n");
    printSymbolTable(symbolBST);

    addSymbol(symbolBST, "Test4", "INT");
    printf("Test Log: added integer variable into SymbolBST\n");
    printSymbolTable(symbolBST);

    addSymbol(symbolBST, "Test1", "INT");
    printf("Test Log: added integer variable into SymbolBST\n");
    printSymbolTable(symbolBST);

    addSymbol(symbolBST, "Test3", "INT");
    printf("Test Log: added integer variable into SymbolBST\n");
    printSymbolTable(symbolBST);

    addSymbol(symbolBST, "Test5", "INT");
    printf("Test Log: added integer variable into SymbolBST\n");
    printSymbolTable(symbolBST);

    addSymbol(symbolBST, "Test6", "INT");
    printf("Test Log: added integer variable into SymbolBST\n");
    printSymbolTable(symbolBST);

    Symbol* temp = lookupSymbol(symbolBST, "Test1");
    printf("Test Log: looked up symbol \"Test1\"\n");
    printSymbol(temp);

    temp = lookupSymbol(symbolBST, "Test2");
    printf("Test Log: looked up symbol \"Test2\"\n");
    printSymbol(temp);

    temp = lookupSymbol(symbolBST, "Test3");
    printf("Test Log: looked up symbol \"Test3\"\n");
    printSymbol(temp);

    temp = lookupSymbol(symbolBST, "Test4");
    printf("Test Log: looked up symbol \"Test4\"\n");
    printSymbol(temp);

    temp = lookupSymbol(symbolBST, "Test5");
    printf("Test Log: looked up symbol \"Test5\"\n");
    printSymbol(temp);

    temp = lookupSymbol(symbolBST, "Test6");
    printf("Test Log: looked up symbol \"Test6\"\n");
    printSymbol(temp);
}

void symbolBST_Test_AddSymbol_DoublicateSymboleError1() {
    
    printf("\n---------------------------------------------------------\n");
    printf("\nTesting:symbolBST_Test_AddSymbol_DoublicateSymboleError1()\n");

    SymbolBST* symbolBST = createSymbolBST();
    printf("Test Log: created SymbolBST\n");

    addSymbol(symbolBST, "Test1", "INT");
    printf("Test Log: added integer variable into SymbolBST\n");
    printSymbolTable(symbolBST);

     addSymbol(symbolBST, "Test1", "INT");
    printf("Test Log: added same integer variable into SymbolBST\n");
    printSymbolTable(symbolBST);
}


void symbolBST_Test_AddSymbol_DoublicateSymboleError2() {
    
    printf("\n---------------------------------------------------------\n");
    printf("\nTesting:symbolBST_Test_AddSymbol_DoublicateSymboleError2()\n");

    SymbolBST* symbolBST = createSymbolBST();
    printf("Test Log: created SymbolBST\n");

    addSymbol(symbolBST, "Test1", "INT");
    printf("Test Log: added integer variable into SymbolBST\n");
    printSymbolTable(symbolBST);

    addSymbol(symbolBST, "Test2", "INT");
    printf("Test Log: added integer variable into SymbolBST\n");
    printSymbolTable(symbolBST);

    addSymbol(symbolBST, "Test2", "INT");
    printf("Test Log: added same integer variable into SymbolBST\n");
    printSymbolTable(symbolBST);
}

void symbolBST_Test_freeSymbolTable_freesSymbolTable() {
    
    printf("\n---------------------------------------------------------\n");
    printf("Testing:symbolBST_Test_freeSymbolTable_freesSymbolTable()\n");

    SymbolBST* symbolBST = createSymbolBST();
    printf("Test Log: created SymbolBST\n");

    addSymbol(symbolBST, "Test1", "INT");
    printf("Test Log: added integer variable into SymbolBST\n");
    printSymbolTable(symbolBST);

    freeSymbolTable(symbolBST);
    printf("Test Log: Freeing symbol bst\n");
    printSymbolTable(symbolBST);

    symbolBST = createSymbolBST();
    printf("Test Log: created SymbolBST\n");

    addSymbol(symbolBST, "Test1", "INT");
    printf("Test Log: added integer variable into SymbolBST\n");
    printSymbolTable(symbolBST);

    addSymbol(symbolBST, "Test2", "INT");
    printf("Test Log: added integer variable into SymbolBST\n");
    printSymbolTable(symbolBST);

    freeSymbolTable(symbolBST);
    printf("Test Log: Freeing symbol bst\n");
    printSymbolTable(symbolBST);
}