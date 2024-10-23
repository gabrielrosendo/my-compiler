#include "functionSymbolBST.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static int indentMagnitude = 3;

FunctionSymbolBST* createFunctionSymbolBST() {
    printf("Log: creating symbolBST\n");
    FunctionSymbolBST* symbolBST = (FunctionSymbolBST*)malloc(sizeof(FunctionSymbolBST));
    if (!symbolBST) return NULL;
    symbolBST->right = NULL;
    symbolBST->left = NULL;
    symbolBST->hash = 0;
    symbolBST->symbol = NULL;
    symbolBST->parameters = NULL;
    return symbolBST;
}

unsigned int functionHash(char* name) {
    unsigned int hashval = 0;
    for (; *name != '\0'; name++) hashval = *name + (hashval << 5) - hashval;
    return hashval;
}

FunctionSymbolBST* addFunctionSymbolPrivate(FunctionSymbolBST* curNode, Symbol* newSymbol, unsigned int curHash, FunctionSymbolBST* prevNode) {
    if(curNode == NULL) {
        curNode = createFunctionSymbolBST();
        curNode->hash = curHash;
        curNode->symbol = newSymbol;
    } else if (curNode->hash == curHash) {
        fprintf(stderr,"SymbolBST Error in addFunctionSymbolPrivate(): added symbol already exists \n");
        free(newSymbol->name);
        free(newSymbol->type);
        free(newSymbol);
        exit(0);
    } else if (curNode->hash < curHash) {
        curNode->right = addFunctionSymbolPrivate(curNode->right, newSymbol, curHash, curNode);
    } else {
        curNode->left = addFunctionSymbolPrivate(curNode->left, newSymbol, curHash, curNode);
    }
    return curNode;
}

void addFunctionSymbol(FunctionSymbolBST* head, char* name, char* type) {
    Symbol* newSymbol = (Symbol*)malloc(sizeof(Symbol));
    if (!newSymbol) return;
    newSymbol->name = strdup(name);
    newSymbol->type = strdup(type);

    unsigned int curHash = functionHash(name);

    if (head == NULL) {
        fprintf(stderr,"SymbolBST Error in addFunctionSymbol(): input node not initialized \n");
        free(newSymbol->name);
        free(newSymbol->type);
        free(newSymbol);
        exit(0);
        return;
    }

    if (head->hash == curHash) {
        fprintf(stderr,"SymbolBST Error in addFunctionSymbol(): added symbol already esists \n");
        exit(0);
        return;
    }

    if (head->symbol == NULL) {
        head->hash = curHash;
        head->symbol = newSymbol;

    } else if (head->hash < curHash) {
        head->right = addFunctionSymbolPrivate(head->right,newSymbol, curHash, head);
    } else {
        head->left = addFunctionSymbolPrivate(head->left, newSymbol, curHash, head);
    }
}

FunctionSymbolBST* lookupFunctionNode(FunctionSymbolBST* head, char* name)   {
    if (head == NULL) {
        fprintf(stderr,"Use of undeclared symbol: %s\n", name);
        exit(0);
    }

    if (head->symbol == NULL) {
        fprintf(stderr,"FunctionSymbolBST Error in lookupFunctionNode(): symbol could not be found in FunctionSymbolBST \n");
        return NULL;
    }

    unsigned int curHash = functionHash(name);

    if (head->hash == curHash) {
        return head;
    }
    
    if (head->hash < curHash) {
        return lookupFunctionNode(head->right, name);
    }

    // else (head->hash > curHash)    
    if (head->left == NULL) {
        fprintf(stderr,"FunctionSymbolBST Error in lookupFunctionNode(): symbol could not be found in FunctionSymbolBST \n");
        exit(0);
    }
    return lookupFunctionNode(head->left, name);
}

void addParameterInternal(FunctionSymbolBST* node, Parameter* parameter, unsigned int curHash) {
    if (node == NULL) {
        fprintf(stderr,"Use of undeclared function: %s\n", parameter->name);
        free(parameter->name);
        free(parameter->type);
        free(parameter);
        exit(0);
    }

    if (node->symbol == NULL) {
        fprintf(stderr,"FucntionSymbolBST Error in addParameterInternal(): symbol could not be found in FunctionSymbolBST \n");
        exit(0);
    }

    if (node->hash == curHash) {
        addParamtersToFunctionInternal(node, parameter);
    } else if (node->hash < curHash) {
        addParameterInternal(node->right, parameter, curHash);
    } else {
        addParameterInternal(node->left, parameter, curHash);
    }
}

void addParameter(FunctionSymbolBST* head, char* functionName, char* parameterName, char* parameterType) {
    if (head == NULL) {
        fprintf(stderr,"Use of undeclared function: %s\n", functionName);
        exit(0);
    }

    if (head->symbol == NULL) {
        fprintf(stderr,"FucntionSymbolBST Error in addParameter(): symbol could not be found in FunctionSymbolBST \n");
        exit(0);
    }

    Parameter* newParameter = (Parameter*)malloc(sizeof(Parameter));
    if (!newParameter) return;
    newParameter->name = strdup(parameterName);
    newParameter->type = strdup(parameterType);
    newParameter->next = NULL;

    unsigned int curHash = functionHash(functionName);

    if (head->hash == curHash) {
        addParamtersToFunctionInternal(head, newParameter);
    } else if (head->hash < curHash) {
        addParameterInternal(head->right, newParameter, curHash);
    } else {
        addParameterInternal(head->left, newParameter, curHash);
    }

}

void addParamtersToFunctionInternal(FunctionSymbolBST* node,  Parameter* parameter) {
   
    if(node->parameters == NULL) {
        node->parameters = parameter;
        return;
    } 

    Parameter* curParam = node->parameters;

    while(curParam->next != NULL) {
        curParam = curParam->next;
    }

    curParam->next = parameter;
}

void freeParameters (Parameter* parameter) {
    if(parameter != NULL) {
        freeParameters(parameter->next);
        free(parameter->name);
        free(parameter->type);
        free(parameter);
    }
}

void freeFunctionSymbolTable(FunctionSymbolBST* node) {
    if (node == NULL) {
        return;
    }
    printf("LOG: freeing symbolBST\n");
    freeFunctionSymbolTable(node->right);
    freeFunctionSymbolTable(node->left);
    freeParameters(node->parameters);
    if (node->symbol != NULL) {
    if (node->symbol->name != NULL) {
        free(node->symbol->name);
    }
    if (node->symbol->type != NULL) {
        free(node->symbol->type);
    }
    if(node->parameters != NULL) {
        free(node->parameters);
    }
    free(node->symbol);
    }
    free(node);
}


void printFunctionSymbolTablePrivateRight(FunctionSymbolBST* curNode, int indent)  {

    //Pring null right node with formatting
    if (curNode->right == NULL) {
        printf("\n");
        for(int i = 0; i < indent; i++) printf("\t");
        printf("Right Node: { NULL }\n");
        printf("\n");


    } else {

        //Print right nodes first
        printFunctionSymbolTablePrivateRight(curNode->right, indent + indentMagnitude);

        //Print right node with formatting
        for(int i = 0; i < indent; i++) printf("\t");
        printf("            { name: %s }\n", curNode->right->symbol->name);
        for(int i = 0; i < indent; i++) printf("\t");
        printf("Right Node: { type: %s }\n", curNode->right->symbol->type);
        for(int i = 0; i < indent; i++) printf("\t");
        printf("            { hash: %u }\n", curNode->right->hash);
        for(int i = 0; i < indent; i++) printf("\t");
        printf("Parameters: {");
        Parameter* curParam = curNode->right->parameters;
        while (curParam != NULL) {
            printf("name: %s, type: %s", curParam->name, curParam->type);
            if (curParam->next != NULL) printf(", ");
            curParam = curParam->next;
        }
        printf("}\n");

        //Print left nodes last
        printFunctionSymbolTablePrivateLeft(curNode->right, indent + indentMagnitude);  
    }
    
}

void printFunctionSymbolTablePrivateLeft(FunctionSymbolBST* curNode, int indent)  {

    //Print null left node with formatting
    if (curNode->left == NULL) {
        printf("\n");
        for(int i = 0; i < indent; i++) printf("\t");
        printf("Left Node: { NULL }\n");
        printf("\n");

    } else {
        //Print right nodes first
        printFunctionSymbolTablePrivateRight(curNode->left, indent + indentMagnitude);

        //Print left node with formatting
        for(int i = 0; i < indent; i++) printf("\t");
        printf("           { name: %s }\n", curNode->left->symbol->name);
        for(int i = 0; i < indent; i++) printf("\t");
        printf("Left Node: { type: %s }\n", curNode->left->symbol->type);
        for(int i = 0; i < indent; i++) printf("\t");
        printf("           { hash: %u }\n", curNode->left->hash);
        for(int i = 0; i < indent; i++) printf("\t");
        printf("Parameters: {");
        Parameter* curParam = curNode->left->parameters;
        while (curParam != NULL) {
            printf("name: %s, type: %s", curParam->name, curParam->type);
            if (curParam->next != NULL) printf(", ");
            curParam = curParam->next;
        }
        printf("}\n");

        //Print left nodes last
        printFunctionSymbolTablePrivateLeft(curNode->left, indent + indentMagnitude);
    }
}

void printFunctionSymbolTable(FunctionSymbolBST* head)  {

    //Print empty head node
    if(head->symbol == NULL) {
        printf("\nHead: { NULL }\n\n");
        return;
    }

    //Print right nodes first
    printFunctionSymbolTablePrivateRight(head, indentMagnitude);

    //Print head with formatting
    printf("      { name: %s }\n", head->symbol->name);
    printf("Head: { type: %s }\n", head->symbol->type);
    printf("      { hash: %u }\n", head->hash);
    printf("Parameters: {");
    Parameter* curParam = head->parameters;
    while (curParam != NULL) {
        printf("name: %s, type: %s", curParam->name, curParam->type);
        if (curParam->next != NULL) printf(", ");
        curParam = curParam->next;
    }
    printf("}\n");

    //Print left nodes last
    printFunctionSymbolTablePrivateLeft(head, indentMagnitude);
}  