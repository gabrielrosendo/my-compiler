#include "AST.h"
#include <stdlib.h>
#include <stdio.h>



struct ASTNode* createNode(NodeType type) {
    struct ASTNode* newNode = (struct ASTNode*)malloc(sizeof(struct ASTNode));
    newNode->type = type;

    switch (type) {
        case NodeType_Program:
            newNode->value.program.FuncDeclList = NULL;
            newNode->value.program.MainFunc = NULL;
            break;
        case NodeType_FuncDeclList:
            newNode->value.FuncDeclList.FuncDecl = NULL;
            newNode->value.FuncDeclList.nextFuncDecl = NULL;
            break;
        case NodeType_FuncDecl:
            newNode->value.FuncDecl.FuncType = NULL;
            newNode->value.FuncDecl.FuncName = NULL;
            newNode->value.FuncDecl.ParamList = NULL;
            newNode->value.FuncDecl.Body = NULL;
            break;
        case NodeType_MainFunc:
            newNode->value.MainFunc.Body = NULL;
            break;
        case NodeType_ParamList:
            newNode->value.ParamList.ParamDecl = NULL;
            newNode->value.ParamList.nextParamDecl = NULL;
            break;
        case NodeType_ParamDecl:
            newNode->value.ParamDecl.paramType = NULL;
            newNode->value.ParamDecl.paramName = NULL;
            break;
        case NodeType_Body:
            newNode->value.Body.VarDeclList = NULL;
            newNode->value.Body.StmtList = NULL;
            break;
        case NodeType_CallParamList:
            newNode->value.CallParamList.expr = NULL;
            newNode->value.CallParamList.nextParam = NULL;
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
        case NodeType_ArrayDecl:
            newNode->value.ArrayDecl.arrayType = NULL;
            newNode->value.ArrayDecl.arrayName = NULL;
            newNode->value.ArrayDecl.arraySize = 0;
            break;
        case NodeType_StmtList:
            newNode->value.StmtList.stmt = NULL;
            newNode->value.StmtList.nextStmt = NULL;
            break;
        case NodeType_Assignment:
            newNode->value.assignment.op = NULL;
            newNode->value.assignment.varName = NULL;
            newNode->value.assignment.expr = NULL;
            break;
        case NodeType_Print:
            newNode->value.print.name = NULL;
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
        case NodeType_FunctionCall:
            newNode->value.FunctionCall.funcName = NULL;
            newNode->value.FunctionCall.CallParamList = NULL;
            break;
        case NodeType_BinaryOp:
            newNode->value.binaryOp.op = '\0';
            break;
        case NodeType_FuncTail:
            newNode->value.FuncTail.expr = NULL;
            newNode->value.FuncTail.type = NULL;
            break;
        case NodeType_ArrayAccess:
            newNode->value.ArrayAccess.varName = NULL;
            newNode->value.ArrayAccess.indexExpr = NULL;
            break;
        default:
            fprintf(stderr, "ERROR: unknown AST node type AST.c->createNode()\n");
            printf("node type: %d\n", type);
            exit(0);
            break;
    }

    return newNode;
}

void freeAST(struct ASTNode* node) {
    if (node == NULL) return;
    // Store the type before potentially corrupted memory access
    NodeType type = node->type;
    printf("Freeing node of type: %d\n", type);

    switch (node->type) {
        case NodeType_Program:
            freeAST(node->value.program.FuncDeclList);
            freeAST(node->value.program.MainFunc);
            break;
        case NodeType_FuncDeclList:
            freeAST(node->value.FuncDeclList.FuncDecl);
            freeAST(node->value.FuncDeclList.nextFuncDecl);
            break;
        case NodeType_FuncDecl:
            free(node->value.FuncDecl.FuncType);
            free(node->value.FuncDecl.FuncName);
            freeAST(node->value.FuncDecl.ParamList);
            freeAST(node->value.FuncDecl.Body);
            break;
        case NodeType_MainFunc:
            freeAST(node->value.MainFunc.Body);
            break;
        case NodeType_ParamList:
            freeAST(node->value.ParamList.ParamDecl);
            freeAST(node->value.ParamList.nextParamDecl);
            break;
        case NodeType_ParamDecl:
            free(node->value.ParamDecl.paramType);
            free(node->value.ParamDecl.paramName);
            break;
        case NodeType_Body:
            freeAST(node->value.Body.VarDeclList);
            freeAST(node->value.Body.StmtList);
            break;
        case NodeType_VarDeclList:
            freeAST(node->value.VarDeclList.VarDecl);
            freeAST(node->value.VarDeclList.nextVarDecl);
            break;
        case NodeType_VarDecl:
            free(node->value.VarDecl.varType);
            free(node->value.VarDecl.varName);
            break;
        case NodeType_ArrayDecl:
            free(node->value.ArrayDecl.arrayType);
            free(node->value.ArrayDecl.arrayName);
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
            free(node->value.print.name);
            break;
        case NodeType_Expression:
            freeAST(node->value.Expression.left);
            freeAST(node->value.Expression.right);
            free(node->value.Expression.op);
            break;
        case NodeType_Number:
            // No dynamic memory to free
            break;
        case NodeType_Identifier:
            free(node->value.identifier.name);
            break;
        case NodeType_BinaryOp:
            // No dynamic memory to free
            break;
        case NodeType_FunctionCall:
            free(node->value.FunctionCall.funcName);
            freeAST(node->value.FunctionCall.CallParamList);
            break;
        case NodeType_FuncTail:
            freeAST(node->value.FuncTail.expr);
            free(node->value.FuncTail.type);
            break;
        case NodeType_CallParamList:
            freeAST(node->value.CallParamList.expr);
            freeAST(node->value.CallParamList.nextParam);
            break;
        default:
            fprintf(stderr, "ERROR: unknown AST node type AST.c->freeAST()\n");
            printf("node type: %d\n", node->type);
            exit(0);
            break;
    }

    free(node);
}

void spaceOut(int indent) {
    for(int i = 1; i < indent; i++) {
        printf("   ");
    }
}


void printAST(struct ASTNode* node, int indent) {
    indent = indent + 1;
    if (!node) return;

    switch (node->type) {
        case NodeType_Program:
            spaceOut(indent);
            printf("AST Print: NodeType_Program\n");
            printAST(node->value.program.FuncDeclList, indent);
            printAST(node->value.program.MainFunc, indent);
            break;
        case NodeType_FuncDeclList:
            spaceOut(indent);
            printf("AST Print: NodeType_FuncDeclList\n");
            printAST(node->value.FuncDeclList.FuncDecl, indent);
            printAST(node->value.FuncDeclList.nextFuncDecl, indent);
            break;
        case NodeType_FuncDecl:
            spaceOut(indent);
            printf("AST Print: NodeType_FuncDecl\n");
            spaceOut(indent);
            printf("AST Print: FuncType = %s\n", node->value.FuncDecl.FuncType);
            spaceOut(indent);
            printf("AST Print: FuncName = %s\n", node->value.FuncDecl.FuncName);
            printAST(node->value.FuncDecl.ParamList, indent);
            printAST(node->value.FuncDecl.Body, indent);
            break;
        case NodeType_MainFunc:
            spaceOut(indent);
            printf("AST Print: NodeType_MainFunc\n");
            printAST(node->value.MainFunc.Body, indent);
            break;
        case NodeType_ParamList:
            spaceOut(indent);
            printf("AST Print: NodeType_ParamList\n");
            printAST(node->value.ParamList.ParamDecl, indent);
            printAST(node->value.ParamList.nextParamDecl, indent);
            break;
        case NodeType_ParamDecl:
            spaceOut(indent);
            printf("AST Print: NodeType_ParamDecl\n");
            spaceOut(indent);
            printf("AST Print: paramType = %s\n", node->value.ParamDecl.paramType);
            spaceOut(indent);
            printf("AST Print: paramName = %s\n", node->value.ParamDecl.paramName);
            break;
        case NodeType_Body:
            spaceOut(indent);
            printf("AST Print: NodeType_Body\n");
            printAST(node->value.Body.VarDeclList, indent);
            printAST(node->value.Body.StmtList, indent);
            printAST(node->value.Body.FuncTail, indent);
            break;
        case NodeType_FuncTail:
            spaceOut(indent);
            printf("AST Print: NodeType_FuncTail\n");
            spaceOut(indent);
            printf("AST Print: type = %s\n", node->value.FuncTail.type);
            printAST(node->value.FuncTail.expr, indent);
            break;
        case NodeType_VarDeclList:
            spaceOut(indent);
            printf("AST Print: NodeType_VarDeclList\n");
            printAST(node->value.VarDeclList.VarDecl, indent);
            printAST(node->value.VarDeclList.nextVarDecl, indent);
            break;
        case NodeType_VarDecl:
            spaceOut(indent);
            printf("AST Print: NodeType_VarDecl\n");
            spaceOut(indent);
            printf("AST Print: varType = %s\n", node->value.VarDecl.varType);
            spaceOut(indent);
            printf("AST Print: varName = %s\n", node->value.VarDecl.varName);
            break;
         case NodeType_ArrayDecl:
            spaceOut(indent);
            printf("AST Print: NodeType_ArrayDecl\n");
            spaceOut(indent);
            printf("AST Print: arrayType = %s\n", node->value.ArrayDecl.arrayType);
            spaceOut(indent);
            printf("AST Print: arrayName = %s\n", node->value.ArrayDecl.arrayName);
            spaceOut(indent);
            printf("AST Print: arraySize = %d\n", node->value.ArrayDecl.arraySize);
            break;
        case NodeType_StmtList:
            spaceOut(indent);
            printf("AST Print: NodeType_StmtList\n");
            printf("DEBUG: stmt pointer = %p\n", (void*)node->value.StmtList.stmt);
            printf("DEBUG: nextStmt pointer = %p\n", (void*)node->value.StmtList.nextStmt);
            printAST(node->value.StmtList.stmt, indent);
            printAST(node->value.StmtList.nextStmt, indent);
            break;
        case NodeType_Assignment:
            spaceOut(indent);
            printf("AST Print: NodeType_Assignment\n");
            if (node->value.assignment.varName) {
                spaceOut(indent);
                printf("AST Print: Array Name = %s\n", node->value.assignment.varName);
            }
            spaceOut(indent);
            printf("AST Print: op = %s\n", node->value.assignment.op);
            if (node->value.assignment.expr) {
                spaceOut(indent);
                printf("AST Print: Assigned Expression:\n");
                printAST(node->value.assignment.expr, indent + 1);
            }
            break;
        case NodeType_ArrayAccess:
            spaceOut(indent);
            printf("AST Print: NodeType_ArrayAccess\n");
            spaceOut(indent);
            printf("AST Print: Array Name = %s\n", node->value.ArrayAccess.varName);
            spaceOut(indent);
            printf("AST Print: Index Expression:\n");
            printAST(node->value.ArrayAccess.indexExpr, indent + 1);
            break;

        case NodeType_Print:
            spaceOut(indent);
            printf("AST Print: NodeType_Print\n");
            spaceOut(indent);
            printf("AST Print: name = %s\n", node->value.print.name);
            break;
        case NodeType_Expression:
            spaceOut(indent);
            printf("AST Print: NodeType_Expression\n");
            spaceOut(indent);
            printf("AST Print: op = %s\n", node->value.Expression.op);
            printAST(node->value.Expression.left, indent);
            printAST(node->value.Expression.right, indent);
            break;
        case NodeType_Number:
            spaceOut(indent);
            printf("AST Print: NodeType_Number\n");
            spaceOut(indent);
            printf("AST Print: number = %d\n", node->value.Number.number);
            break;
        case NodeType_Identifier:
            spaceOut(indent);
            printf("AST Print: NodeType_Identifier\n");
            spaceOut(indent);
            printf("AST Print: name = %s\n", node->value.identifier.name);
            break;
        case NodeType_BinaryOp:
            spaceOut(indent);
            printf("AST Print: NodeType_BinaryOp\n");
            spaceOut(indent);
            printf("AST Print: op = %s\n", node->value.binaryOp.op);
            break;
        default:
            break;
    }
}