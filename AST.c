#include "AST.h"
#include <stdlib.h>

ASTNode* createNode(NodeType type) {
    ASTNode* newNode = (ASTNode*)malloc(sizeof(ASTNode));
    if (newNode == NULL) {
        // Handle memory allocation failure if needed
        return NULL;
    }
    newNode->type = type;
    switch(type){
        case NodeType_Program:
            newNode->program.varDeclList = NULL;
            newNode->program.stmtList = NULL;
            break;
        case NodeType_VarDeclList:
            newNode->VarDeclList.VarDecl = NULL;
            newNode->VarDeclList.nextVarDecl = NULL;
            break;
        case NodeType_StmtList:
            newNode->StmtList.stmt = NULL;
            newNode->StmtList.nextStmt = NULL;
        case NodeType_Stmt:
            // Print statement
            newNode->print.expr = NULL;
            break;
        case NodeType_VarDecl:
            newNode->VarDecl.varType = NULL;
            newNode->VarDecl.varName = NULL;
            break;
        case NodeType_Expression:
            newNode->binaryOp.left = NULL;
            newNode->binaryOp.right = NULL;
            newNode->binaryOp.op = '\0';
            break;
        case NodeType_Identifier:
            newNode->identifier.name = NULL;
            break;
        case NodeType_BinaryOp:
            newNode->binaryOp.left = NULL;
            newNode->binaryOp.right = NULL;
            newNode->binaryOp.op = '\0';
            break;
        case NodeType_Print:
            newNode->print.expr = NULL;
            break;
        case NodeType_Assignment:
            newNode->assignment.op = NULL;
            newNode->assignment.varName = NULL;
            newNode->assignment.expr = NULL;
            break;
    }

    return newNode;
}