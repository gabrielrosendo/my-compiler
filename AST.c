#include "AST.h"
#include <stdlib.h>
#include <stdio.h>


struct ASTNode* createNode(NodeType type) {
    struct ASTNode* newNode = (struct ASTNode*)malloc(sizeof(struct ASTNode));
    newNode->type = type;

    switch (type) {
        case NodeType_Program:
            newNode->value.program.VarDeclList = NULL;
            newNode->value.program.StmtList = NULL;
            break;
        case NodeType_VarDeclList:
            newNode->value.VarDeclList.VarDecl = NULL;
            newNode->value.VarDeclList.nextVarDecl = NULL;
            break;
        case NodeType_VarDecl:
            newNode->value.VarDecl.varType = NULL;
            newNode->value.VarDecl.varName = NULL;
            newNode->value.VarDecl.initExpr = NULL;
            break;
        case NodeType_StmtList:
            newNode->value.StmtList.stmt = NULL;
            newNode->value.StmtList.nextStmt = NULL;
            break;
        case NodeType_Assignment:
            newNode->value.assignment.op = NULL;
            newNode->value.assignment.varName = NULL;
            break;
        case NodeType_Print:
            newNode->value.print.expr = NULL;
            break;
        case NodeType_Expression:
            newNode->value.Expression.left = NULL;
            newNode->value.Expression.right = NULL;
            newNode->value.Expression.op = '\0';
            break;
        case NodeType_Number:
            newNode->value.Number.number = 0;
            break;
        case NodeType_Identifier:
            newNode->value.identifier.name = NULL;
            break;
        case NodeType_BinaryOp:
            newNode->value.binaryOp.op = '\0';
            break;
        default:
            fprintf(stderr, "ERROR: unknow AST node type AST.c->createNode()\n");
            exit(0);
            break;
    }

    return newNode;
}

void freeAST(struct ASTNode* node) {
    if (!node) return;

    switch (node->type) {
        case NodeType_Program:
            freeAST(node->value.program.VarDeclList);
            freeAST(node->value.program.StmtList);
            break;
        case NodeType_VarDeclList:
            freeAST(node->value.VarDeclList.VarDecl);
            freeAST(node->value.VarDeclList.nextVarDecl);
            break;
        case NodeType_VarDecl:
            free(node->value.VarDecl.varType);
            free(node->value.VarDecl.varName);
            freeAST(node->value.VarDecl.initExpr);
            break;
        case NodeType_StmtList:
            freeAST(node->value.StmtList.stmt);
            freeAST(node->value.StmtList.nextStmt);
            break;
        case NodeType_Assignment:
            free(node->value.assignment.op);
            free(node->value.assignment.varName);
            freeAST(node->value.assignment.expr);
            break;
        case NodeType_Print:
            freeAST(node->value.print.expr);
            break;
        case NodeType_Expression :
            freeAST(node->value.Expression.right);
            freeAST(node->value.Expression.left);
            free(node->value.Expression.op);
            break;
        case NodeType_Number :
            break;
        case NodeType_Identifier:
            free(node->value.identifier.name);
            break;
        case NodeType_BinaryOp :
            free(node->value.binaryOp.op);
            break;
        default:
            break;
    }

    free(node);
}