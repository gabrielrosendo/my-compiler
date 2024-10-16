#include "functionSymbolBST.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static int indentMagnitude = 3;

FunctionSymbolBST* createSymbolBST() {
    printf("Log: creating symbolBST\n");
    FunctionSymbolBST* symbolBST = (FunctionSymbolBST*)malloc(sizeof(FunctionSymbolBST));
    if (!symbolBST) return NULL;
    symbolBST->right = NULL;
    symbolBST->left = NULL;
    symbolBST->hash = 0;
    symbolBST->symbol = NULL;
    return symbolBST;
}

unsigned int hash(char* name) {
    unsigned int hashval = 0;
    for (; *name != '\0'; name++) hashval = *name + (hashval << 5) - hashval;
    return hashval;
}

FunctionSymbolBST* addSymbolPrivate(FunctionSymbolBST* curNode, Symbol* newSymbol, unsigned int curHash, FunctionSymbolBST* prevNode) {
    if(curNode == NULL) {
        curNode = createSymbolBST();
        curNode->hash = curHash;
        curNode->symbol = newSymbol;
    } else if (curNode->hash == curHash) {
        fprintf(stderr,"SymbolBST Error in addSymbolPrivate(): added symbol already exists \n");
        free(newSymbol->name);
        free(newSymbol->type);
        free(newSymbol);
        exit(0);
    } else if (curNode->hash < curHash) {
        curNode->right = addSymbolPrivate(curNode->right, newSymbol, curHash, curNode);
    } else {
        curNode->left = addSymbolPrivate(curNode->left, newSymbol, curHash, curNode);
    }
    return curNode;
}

void addSymbol(FunctionSymbolBST* head, char* name, char* type) {
    Symbol* newSymbol = (Symbol*)malloc(sizeof(Symbol));
    if (!newSymbol) return;
    newSymbol->name = strdup(name);
    newSymbol->type = strdup(type);

    unsigned int curHash = hash(name);

    if (head == NULL) {
        fprintf(stderr,"SymbolBST Error in addSymbol(): input node not initialized \n");
        free(newSymbol->name);
        free(newSymbol->type);
        free(newSymbol);
        exit(0);
        return;
    }

    if (head->hash == curHash) {
        fprintf(stderr,"SymbolBST Error in addSymbolPrivate(): added symbol already esists \n");
        exit(0);
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

Symbol* lookupSymbol(FunctionSymbolBST* head, char* name)   {
    if (head == NULL) {
        fprintf(stderr,"Use of undeclared symbol: %s\n", name);
        exit(0);
    }

    if (head->symbol == NULL) {
        fprintf(stderr,"SymbolBST Error in lookupSymbol(): symbol could not be found in SymbolBST \n");
        return NULL;
    }

    unsigned int curHash = hash(name);

    if (head->hash == curHash) {
        return head->symbol;
    }
    
    if (head->hash < curHash) {
        return lookupSymbol(head->right, name);
    }

    // else (head->hash > curHash)    
    if (head->left == NULL) {
        fprintf(stderr,"SymbolBST Error in lookupSymbol(): symbol could not be found in SymbolBST \n");
        exit(0);
    }
    return lookupSymbol(head->left, name);
}

void addParameter(FunctionSymbolBST* head, char* functionName, char* parameterName, char* parameterType) {
    Parameter* newParameter = (Symbol*)malloc(sizeof(Symbol));
    if (!newParameter) return;
    newParameter->name = strdup(parameterName);
    newParameter->type = strdup(parameterType);

    
}

void freeSymbolTable(FunctionSymbolBST* node) {
    if (node == NULL) {
        return;
    }
    printf("LOG: freeing symbolBST\n");
    freeSymbolTable(node->right);
    freeSymbolTable(node->left);
    if (node->symbol != NULL) {
    if (node->symbol->name != NULL) {
        free(node->symbol->name);
    }
    if (node->symbol->type != NULL) {
        free(node->symbol->type);
    }
    free(node->symbol);
    }
    free(node);
}


void printSymbolTablePrivateRight(FunctionSymbolBST* curNode, int indent)  {

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

void printSymbolTablePrivateLeft(FunctionSymbolBST* curNode, int indent)  {

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

void printSymbolTable(FunctionSymbolBST* head)  {

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