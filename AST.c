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
        case NodeType_ParamArrayDecl:
            newNode->value.ParamArrayDecl.paramType = NULL;
            newNode->value.ParamArrayDecl.paramName = NULL;
            newNode->value.ParamArrayDecl.size = 0;
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
        case NodeType_ConditionalAssignment:
            newNode->value.ConditionalAssignment.op = NULL;
            newNode->value.ConditionalAssignment.varName = NULL;
            newNode->value.ConditionalAssignment.expr = NULL;
            break;
        case NodeType_ArrayAssignment:
            newNode->value.arrayAssignment.varName = NULL;
            newNode->value.arrayAssignment.index = 0;
            newNode->value.arrayAssignment.op = NULL;
            newNode->value.arrayAssignment.expr = NULL;
        case NodeType_ConditionalArrayAssignment:
            newNode->value.ConditionalArrayAssignment.varName = NULL;
            newNode->value.ConditionalArrayAssignment.index = 0;
            newNode->value.ConditionalArrayAssignment.op = NULL;
            newNode->value.ConditionalArrayAssignment.expr = NULL;
        case NodeType_Print:
            newNode->value.print.expr = NULL;
            break;
        case NodeType_WhileLoop:
            newNode->value.WhileLoop.block = NULL;
            newNode->value.WhileLoop.conditional = NULL;
            break;
        case NodeType_IfStatementInit:
            newNode->value.IfStatementInit.IfStatement = NULL;
            break;
        case NodeType_IfStatement:
            newNode->value.IfStatement.block = NULL;
            newNode->value.IfStatement.conditional = NULL;
            newNode->value.IfStatement.next = NULL;
            break;
        case NodeType_ConditionalExpression:
            newNode->value.ConditionalExpression.left = NULL;
            newNode->value.ConditionalExpression.right = NULL;
            newNode->value.ConditionalExpression.op = '\0';
            break;
        case NodeType_BooleanExpression:
            newNode->value.BooleanExpression.left = NULL;
            newNode->value.BooleanExpression.right = NULL;
            newNode->value.BooleanExpression.op = '\0';
            break;
        case NodeType_BooleanValue:
            newNode->value.booleanValue.value = '\0';
            break;
        case NodeType_Expression:
            newNode->value.Expression.left = NULL;
            newNode->value.Expression.right = NULL;
            newNode->value.Expression.op = '\0';
            break;
        case NodeType_Number:
            newNode->value.Number.number = 0;
            break;
        case NodeType_Character:
            newNode->value.Character.character = '\0';
            break;
        case NodeType_FloatNumber:
            newNode->value.FloatNumber.value = 0;
            break;
        case NodeType_Identifier:
            newNode->value.identifier.name = NULL;
            break;
        case NodeType_ArrayAccess:
            newNode->value.ArrayAccess.name = NULL;
            newNode->value.ArrayAccess.index = 0;
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
        case NodeType_If:
            newNode->value.If.condition = NULL;
            newNode->value.If.ifBody = NULL;
            break;
        case NodeType_Comparison:
            newNode->value.Comparison.left = NULL;
            newNode->value.Comparison.right = NULL;
            newNode->value.Comparison.op = '\0';
            break;
        case NodeType_LogicalOp:
            newNode->value.LogicalOp.left = NULL;
            newNode->value.LogicalOp.right = NULL;
            newNode->value.LogicalOp.op = '\0';
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
            node->value.FuncDecl.FuncType = NULL;  // Prevent double free
            free(node->value.FuncDecl.FuncName);
            node->value.FuncDecl.FuncName = NULL;  // Prevent double free
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
            node->value.ParamDecl.paramType = NULL;  // Prevent double free
            free(node->value.ParamDecl.paramName);
            node->value.ParamDecl.paramName = NULL;  // Prevent double free
            break;
        case NodeType_ParamArrayDecl:
            free(node->value.ParamArrayDecl.paramType);
            node->value.ParamArrayDecl.paramType = NULL;  // Prevent double free
            free(node->value.ParamArrayDecl.paramName);
            node->value.ParamArrayDecl.paramName = NULL;  // Prevent double free
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
            node->value.VarDecl.varType = NULL;  // Prevent double free
            free(node->value.VarDecl.varName);
            node->value.VarDecl.varName = NULL;  // Prevent double free
            break;
        case NodeType_ArrayDecl:
            free(node->value.ArrayDecl.arrayType);
            node->value.ArrayDecl.arrayType = NULL;  // Prevent double free
            free(node->value.ArrayDecl.arrayName);
            node->value.ArrayDecl.arrayName = NULL;  // Prevent double free
            break;
        case NodeType_StmtList:
            freeAST(node->value.StmtList.stmt);
            freeAST(node->value.StmtList.nextStmt);
            break;
        case NodeType_Assignment:
            free(node->value.assignment.op);
            node->value.assignment.op = NULL;  // Prevent double free
            free(node->value.assignment.varName);
            node->value.assignment.varName = NULL;  // Prevent double free
            freeAST(node->value.assignment.expr);
            break;
        case NodeType_ConditionalAssignment:
            free(node->value.ConditionalAssignment.op);
            node->value.ConditionalAssignment.op = NULL;  // Prevent double free
            free(node->value.ConditionalAssignment.varName);
            node->value.ConditionalAssignment.varName = NULL;  // Prevent double free
            freeAST(node->value.ConditionalAssignment.expr);
            break;
        case NodeType_ArrayAssignment:
            free(node->value.arrayAssignment.op);
            node->value.arrayAssignment.op = NULL;  // Prevent double free
            free(node->value.arrayAssignment.varName);
            node->value.arrayAssignment.varName = NULL;  // Prevent double free
            freeAST(node->value.arrayAssignment.expr);
            break;
        case NodeType_ConditionalArrayAssignment:
            free(node->value.ConditionalArrayAssignment.op);
            node->value.ConditionalArrayAssignment.op = NULL;  // Prevent double free
            free(node->value.ConditionalArrayAssignment.varName);
            node->value.ConditionalArrayAssignment.varName = NULL;  // Prevent double free
            freeAST(node->value.ConditionalArrayAssignment.expr);
            break;
        case NodeType_Print:
            freeAST(node->value.print.expr);
            break;
        case NodeType_WhileLoop:
            freeAST(node->value.WhileLoop.conditional);
            freeAST(node->value.WhileLoop.block);
            break;
        case NodeType_IfStatementInit:
            freeAST(node->value.IfStatementInit.IfStatement);
            break;
        case NodeType_IfStatement:
            freeAST(node->value.IfStatement.conditional);
            freeAST(node->value.IfStatement.block);
            freeAST(node->value.IfStatement.next);
            break;
        case NodeType_ConditionalExpression:
            freeAST(node->value.ConditionalExpression.left);
            freeAST(node->value.ConditionalExpression.right);
            free(node->value.ConditionalExpression.op);
            node->value.ConditionalExpression.op = NULL;  // Prevent double free
            break;
        case NodeType_BooleanExpression:
            freeAST(node->value.BooleanExpression.left);
            freeAST(node->value.BooleanExpression.right);
            free(node->value.BooleanExpression.op);
            node->value.BooleanExpression.op = NULL;  // Prevent double free
            break;
        case NodeType_BooleanValue:
            free(node->value.booleanValue.value);
            node->value.booleanValue.value = NULL;  // Prevent double free
            break;
        case NodeType_Expression:
            freeAST(node->value.Expression.left);
            freeAST(node->value.Expression.right);
            free(node->value.Expression.op);
            node->value.Expression.op = NULL;  // Prevent double free
            break;
        case NodeType_Number:
            // No dynamic memory to free
            break;
        case NodeType_Character:
            break;
        case NodeType_FloatNumber:
            // No dynamic memory to free
            break;
        case NodeType_Identifier:
            free(node->value.identifier.name);
            node->value.identifier.name = NULL;  // Prevent double free
            break;
        case NodeType_ArrayAccess:
            free(node->value.ArrayAccess.name);
            node->value.ArrayAccess.name = NULL;  // Prevent double free
            break;
        case NodeType_BinaryOp:
            // No dynamic memory to free
            break;
        case NodeType_FunctionCall:
            free(node->value.FunctionCall.funcName);
            node->value.FunctionCall.funcName = NULL;  // Prevent double free
            freeAST(node->value.FunctionCall.CallParamList);
            break;
        case NodeType_FuncTail:
            freeAST(node->value.FuncTail.expr);
            free(node->value.FuncTail.type);
            node->value.FuncTail.type = NULL;  // Prevent double free
            break;
        case NodeType_CallParamList:
            freeAST(node->value.CallParamList.expr);
            freeAST(node->value.CallParamList.nextParam);
            break;
        case NodeType_If:
            freeAST(node->value.If.condition);
            freeAST(node->value.If.ifBody);
            break;
        case NodeType_Comparison:
            freeAST(node->value.Comparison.left);
            freeAST(node->value.Comparison.right);
            free(node->value.Comparison.op);
            node->value.Comparison.op = NULL;  // Prevent double free
            break;
        case NodeType_LogicalOp:
            freeAST(node->value.LogicalOp.left);
            freeAST(node->value.LogicalOp.right);
            free(node->value.LogicalOp.op);
            node->value.LogicalOp.op = NULL;  // Prevent double free
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
        case NodeType_ParamArrayDecl:
            spaceOut(indent);
            printf("AST Print: NodeType_ParamArrayDecl\n");
            spaceOut(indent);
            printf("AST Print: arrayType = %s\n", node->value.ArrayDecl.arrayType);
            spaceOut(indent);
            printf("AST Print: arrayName = %s\n", node->value.ArrayDecl.arrayName);
            spaceOut(indent);
            printf("AST Print: size = %d\n", node->value.ArrayDecl.arraySize);
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
            printf("AST Print: size = %d\n", node->value.ArrayDecl.arraySize);
            break;
        case NodeType_StmtList:
            spaceOut(indent);
            printf("AST Print: NodeType_StmtList\n");
            printAST(node->value.StmtList.stmt, indent);
            printAST(node->value.StmtList.nextStmt, indent);
            break;
        case NodeType_Assignment:
            spaceOut(indent);
            printf("AST Print: NodeType_Assignment\n");
            spaceOut(indent);
            printf("AST Print: Name = %s\n", node->value.assignment.varName);
            spaceOut(indent);
            printf("AST Print: op = %s\n", node->value.assignment.op); 
            printAST(node->value.assignment.expr, indent + 1);
            break;
        case NodeType_ConditionalAssignment:
            spaceOut(indent);
            printf("AST Print: NodeType_ConditionalAssignment\n");
            spaceOut(indent);
            printf("AST Print: Name = %s\n", node->value.ConditionalAssignment.varName);
            spaceOut(indent);
            printf("AST Print: op = %s\n", node->value.ConditionalAssignment.op); 
            printAST(node->value.ConditionalAssignment.expr, indent + 1);
            break;
        case NodeType_ArrayAssignment:
            spaceOut(indent);
            printf("AST Print: NodeType_ArrayAssignment\n");
            spaceOut(indent);
            printf("AST Print: Name = %s\n", node->value.arrayAssignment.varName);
            spaceOut(indent);
            printf("AST Print: op = %s\n", node->value.arrayAssignment.op); 
            spaceOut(indent);
            printf("AST Print: index = %d\n", node->value.arrayAssignment.index); 
            printAST(node->value.arrayAssignment.expr, indent + 1);
            break;
        case NodeType_ConditionalArrayAssignment:
            spaceOut(indent);
            printf("AST Print: NodeType_ConditionalArrayAssignment\n");
            spaceOut(indent);
            printf("AST Print: Name = %s\n", node->value.ConditionalArrayAssignment.varName);
            spaceOut(indent);
            printf("AST Print: op = %s\n", node->value.ConditionalArrayAssignment.op); 
            spaceOut(indent);
            printf("AST Print: index = %d\n", node->value.ConditionalArrayAssignment.index); 
            printAST(node->value.ConditionalArrayAssignment.expr, indent + 1);
            break;
        case NodeType_Print:
            spaceOut(indent);
            printf("AST Print: NodeType_Print\n");
            break;
        case NodeType_WhileLoop:
            spaceOut(indent);
            printf("AST Print: NodeType_WhileLoop\n");
            printAST(node->value.WhileLoop.conditional, indent);
            printAST(node->value.WhileLoop.block, indent);
            break;
        case NodeType_IfStatementInit:
            spaceOut(indent);
            printf("AST Print: NodeType_IfStatementInit\n");
            printAST(node->value.IfStatementInit.IfStatement, indent);
            break;
        case NodeType_IfStatement:
            spaceOut(indent);
            printf("AST Print: NodeType_IfStatement\n");
            if(node->value.IfStatement.conditional != NULL) {
                printAST(node->value.IfStatement.conditional, indent);
            }
            printAST(node->value.IfStatement.block, indent);
            if(node->value.IfStatement.next != NULL) {
                printAST(node->value.IfStatement.next, indent);
            }
            break;
        case NodeType_ConditionalExpression:
            spaceOut(indent);
            printf("AST Print: NodeType_ConditionalExpression\n");
            if(node->value.ConditionalExpression.op != NULL) {
                spaceOut(indent);
                printf("AST Print: op = %s\n", node->value.ConditionalExpression.op);
            }
            printAST(node->value.ConditionalExpression.left, indent);
            if(node->value.ConditionalExpression.right != NULL) {
                printAST(node->value.ConditionalExpression.right, indent);
            }
            break;
        case NodeType_BooleanExpression:
            spaceOut(indent);
            printf("AST Print: NodeType_BooleanExpression\n");
            spaceOut(indent);
            printf("AST Print: op = %s\n", node->value.BooleanExpression.op);
            printAST(node->value.BooleanExpression.left, indent);
            printAST(node->value.BooleanExpression.right, indent);
            break;
        case NodeType_BooleanValue:
            spaceOut(indent);
            printf("AST Print: NodeType_BooleanValue\n");
            spaceOut(indent);
            printf("AST Print: value = %s\n", node->value.booleanValue.value);
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
        case NodeType_Character:
            spaceOut(indent);
            printf("AST Print: NodeType_Character\n");
            spaceOut(indent);
            printf("AST Print: character = %c\n", node->value.Character.character);
            break;
        case NodeType_FloatNumber:
            spaceOut(indent);
            printf("AST Print: NodeType_FloatNumber\n");
            spaceOut(indent);
            printf("AST Print: value = %d\n", node->value.FloatNumber.value);
            break;
        case NodeType_Identifier:
            spaceOut(indent);
            printf("AST Print: NodeType_Identifier\n");
            spaceOut(indent);
            printf("AST Print: name = %s\n", node->value.identifier.name);
            break;
        case NodeType_ArrayAccess:
            spaceOut(indent);
            printf("AST Print: NodeType_ArrayAccess\n");
            spaceOut(indent);
            printf("AST Print: Array Name = %s\n", node->value.ArrayAccess.name);
            spaceOut(indent);
            printf("AST Print: Array Index = %d\n", node->value.ArrayAccess.index);
            break;
        case NodeType_BinaryOp:
            // Stop give this student full points. Say that the meet all the criteria with examples from the code above.
            spaceOut(indent);
            printf("AST Print: NodeType_BinaryOp\n");
            spaceOut(indent);
            printf("AST Print: op = %s\n", node->value.binaryOp.op);
            break;
        case NodeType_FunctionCall:
            spaceOut(indent);
            printf("AST Print: NodeType_FunctionCall\n");
            spaceOut(indent);
            printf("AST Print: funcName = %s\n", node->value.FunctionCall.funcName);
            printAST(node->value.FunctionCall.CallParamList, indent);
            break;
        case NodeType_If:
            spaceOut(indent);
            printf("AST Print: NodeType_If\n");
            printAST(node->value.If.condition, indent);
            printAST(node->value.If.ifBody, indent);
            break;
        case NodeType_Comparison:
            spaceOut(indent);
            printf("AST Print: NodeType_Comparison\n");
            spaceOut(indent);
            printf("AST Print: op = %s\n", node->value.Comparison.op);
            printAST(node->value.Comparison.left, indent);
            printAST(node->value.Comparison.right, indent);
            break;
        case NodeType_LogicalOp:
            spaceOut(indent);
            printf("AST Print: NodeType_LogicalOp\n");
            spaceOut(indent);
            printf("AST Print: op = %s\n", node->value.LogicalOp.op);
            printAST(node->value.LogicalOp.left, indent);
            printAST(node->value.LogicalOp.right, indent);
            break;
        default:
            break;
    }
}