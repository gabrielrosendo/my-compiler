#include "symbolBST_Test.h"
#include "symbolBST.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void symbolBST_TestCreation() {

    printf("\nTesting: symbolBST_TestCreation()\n");

    SymbolBST* symbolBST = createSymbolBST();
    printf("Test Log: created SymbolBST\n");
    printSymbolTable(symbolBST);
}

void symbolBST_Test_AddSymbol_SingleIntegerInput() {

    printf("\nTesting: symbolBST_Test_AddSymbol_SingleIntegerInput()\n");

    SymbolBST* symbolBST = createSymbolBST();
    printf("Test Log: created SymbolBST\n");

    addSymbol(symbolBST, "Test", "INT");
    printf("Test Log: added integer variable into SymbolBST\n");
    printSymbolTable(symbolBST);
}

void symbolBST_Test_AddSymbol_MultipleIntegerInputs() {

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

//TODO: test creating the same symbol?!?!