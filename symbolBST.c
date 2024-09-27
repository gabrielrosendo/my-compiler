#include "symbolBST.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static int indentMagnitude = 3;

SymbolBST* createSymbolBST() {
    printf("Log: creating symbolBST\n");
    SymbolBST* symbolBST = (SymbolBST*)malloc(sizeof(SymbolBST));
    if (!symbolBST) return NULL;
    symbolBST->right = NULL;
    symbolBST->left = NULL;
    symbolBST->previous = NULL;
    symbolBST->hash = 0;
    symbolBST->symbol = NULL;
    return symbolBST;
}

unsigned int hash(char* name) {
    unsigned int hashval = 0;
    for (; *name != '\0'; name++) hashval = *name + (hashval << 5) - hashval;
    return hashval;
}

SymbolBST* addSymbolPrivate(SymbolBST* curNode, Symbol* newSymbol, unsigned int curHash, SymbolBST* prevNode) {
    if(curNode == NULL) {
        curNode = createSymbolBST();
        curNode->previous=prevNode;
        curNode->hash = curHash;
        curNode->symbol = newSymbol;
    } else if (curNode->hash < curHash) {
        curNode->right = addSymbolPrivate(curNode->right, newSymbol, curHash, curNode);
        curNode->previous = prevNode;
    } else {
        curNode->left = addSymbolPrivate(curNode->left, newSymbol, curHash, curNode);
        curNode->previous = prevNode;
    }
    return curNode;
}

void addSymbol(SymbolBST* head, char* name, char* type) {
    Symbol* newSymbol = (Symbol*)malloc(sizeof(Symbol));
    if (!newSymbol) return;
    newSymbol->name = strdup(name);
    newSymbol->type = strdup(type);

    unsigned int curHash = hash(name);

    if (head == NULL) {
        fprintf(stderr,"SymbolBST Error in addSymbol(): input node not initialized \n");
        return;
    }

    if (head->symbol == NULL) {
        head->hash = curHash;
        head->symbol = newSymbol;

    } else if (head->hash < curHash) {
        head->right = addSymbolPrivate(head->right,newSymbol, curHash, head);
    } else {
        head->left = addSymbolPrivate(head->left, newSymbol, curHash, head);
    }
}


Symbol* lookupSymbol(SymbolBST* head, char* name)   {}

void freeSymbolTable(SymbolBST* head)   {}

void printSymbolTablePrivateRight(SymbolBST* curNode, int indent)  {

    //Pring null right node with formatting
    if (curNode->right == NULL) {
        printf("\n");
        for(int i = 0; i < indent; i++) printf("\t");
        printf("Right Node: { NULL }\n");
        printf("\n");


    } else {

        //Print right nodes first
        printSymbolTablePrivateRight(curNode->right, indent + indentMagnitude);

        //Print right node with formatting
        for(int i = 0; i < indent; i++) printf("\t");
        printf("            { name: %s }\n", curNode->right->symbol->name);
        for(int i = 0; i < indent; i++) printf("\t");
        printf("Right Node: { type: %s }\n", curNode->right->symbol->type);
        for(int i = 0; i < indent; i++) printf("\t");
        printf("            { hash: %u }\n", curNode->right->hash);

        //Print left nodes last
        printSymbolTablePrivateLeft(curNode->right, indent + indentMagnitude);  
    }
    
}

void printSymbolTablePrivateLeft(SymbolBST* curNode, int indent)  {

    //Print null left node with formatting
    if (curNode->left == NULL) {
        printf("\n");
        for(int i = 0; i < indent; i++) printf("\t");
        printf("Left Node: { NULL }\n");
        printf("\n");

    } else {
        //Print right nodes first
        printSymbolTablePrivateRight(curNode->left, indent + indentMagnitude);

        //Print left node with formatting
        for(int i = 0; i < indent; i++) printf("\t");
        printf("           { name: %s }\n", curNode->left->symbol->name);
        for(int i = 0; i < indent; i++) printf("\t");
        printf("Left Node: { type: %s }\n", curNode->left->symbol->type);
        for(int i = 0; i < indent; i++) printf("\t");
        printf("           { hash: %u }\n", curNode->left->hash);

        //Print left nodes last
        printSymbolTablePrivateLeft(curNode->left, indent + indentMagnitude);
    }
}

void printSymbolTable(SymbolBST* head)  {

    //Print empty head node
    if(head->symbol == NULL) {
        printf("\nHead: { NULL }\n\n");
        return;
    }

    //Print right nodes first
    printSymbolTablePrivateRight(head, indentMagnitude);

    //Print head with formatting
    printf("      { name: %s }\n", head->symbol->name);
    printf("Head: { type: %s }\n", head->symbol->type);
    printf("      { hash: %u }\n", head->hash);

    //Print left nodes last
    printSymbolTablePrivateLeft(head, indentMagnitude);
}  