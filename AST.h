#ifndef AST_H
#define AST_H

#include <stdbool.h>


// Define Node Types
typedef enum {
    NodeType_Program,
    NodeType_FuncDeclList,
    NodeType_FuncDecl,
    NodeType_MainFunc,
    NodeType_ParamList,
    NodeType_ParamDecl,
    NodeType_ParamArrayDecl,
    NodeType_Body,
    NodeType_FuncTail,
    NodeType_VarDeclList,
    NodeType_VarDecl,
    NodeType_ArrayDecl,
    NodeType_StmtList,
    NodeType_Stmt,
    NodeType_Assignment,
    NodeType_ConditionalAssignment,
    NodeType_ArrayAssignment,
    NodeType_ConditionalArrayAssignment,
    NodeType_Print,
    NodeType_WhileLoop,
    NodeType_IfStatementInit,
    NodeType_IfStatement,
    NodeType_ConditionalExpression,
    NodeType_BooleanExpression,
    NodeType_BooleanValue,
    NodeType_Expression,
    NodeType_Number,
    NodeType_Character,
    NodeType_FloatNumber,
    NodeType_Identifier,
    NodeType_ArrayAccess,
    NodeType_BinaryOp,
    NodeType_FunctionCall,
    NodeType_CallParamList,
    NodeType_If,
    NodeType_Comparison,
    NodeType_LogicalOp
} NodeType;

// Define AST Structure
typedef struct ASTNode {
    NodeType type;
    union {
        struct {
            struct ASTNode* FuncDeclList;
            struct ASTNode* MainFunc;
        }program;

        struct {
            struct ASTNode* FuncDecl;
            struct ASTNode* nextFuncDecl;
        }FuncDeclList;

        struct {
            char* FuncType;
            char* FuncName;
            struct ASTNode* ParamList;
            struct ASTNode* Body;
        }FuncDecl;

        struct {
            struct ASTNode* Body;
        }MainFunc;

        struct {
            struct ASTNode* ParamDecl;
            struct ASTNode* nextParamDecl;
        }ParamList;

        struct {
            char* paramType;
            char* paramName;
        }ParamDecl;

        struct {
            char* paramType;
            char* paramName;
            unsigned int size;
        }ParamArrayDecl;

        struct {
            struct ASTNode* VarDeclList;
            struct ASTNode* StmtList;
            struct ASTNode* FuncTail;
        }Body;

        struct {
            struct ASTNode* expr;
            char* type;
        }FuncTail;

        struct {
            struct ASTNode* VarDecl;
            struct ASTNode* nextVarDecl;
        }VarDeclList;

        struct {
            char* varType;
            char* varName;
            // initial value if provided numerical
            struct ASTNode* initExpr;
        }VarDecl;

        struct {
            char* arrayType;
            char* arrayName;
            unsigned int arraySize;
        }ArrayDecl;
        
        struct {
            struct ASTNode* stmt;
            struct ASTNode* nextStmt;
        } StmtList;

        struct {
            char* varName;
            char* op;
            struct ASTNode* expr;
        } assignment;

        struct {
            char* varName;
            char* op;
            struct ASTNode* expr;
        } ConditionalAssignment;

        struct {
            char* varName;
            unsigned int index;
            char* op;
            struct ASTNode* expr;
        } arrayAssignment;

        struct {
            char* varName;
            unsigned int index;
            char* op;
            struct ASTNode* expr;
        } ConditionalArrayAssignment;

        struct{
            struct ASTNode* expr;
        } print;

        struct {
            struct ASTNode* conditional;
            struct ASTNode* block;
        } WhileLoop;

        struct {
            struct ASTNode* IfStatement;
        } IfStatementInit;

        struct {
            struct ASTNode* block;
            struct ASTNode* next;
            struct ASTNode* conditional;
        } IfStatement;

        struct {
            struct ASTNode* left;
            struct ASTNode* right;
            char* op;
        } ConditionalExpression;

        struct {
            struct ASTNode* left;
            struct ASTNode* right;
            char* op;
        } BooleanExpression;

        struct {
            char* value;
        } booleanValue;

        struct {
            struct ASTNode* left;
            struct ASTNode* right;
            char* op;
        } Expression;

        struct {
           int number;
        } Number;

        struct {
            char character;
        } Character;

       struct {
           float value;
       } FloatNumber;

        struct {
            char* name;
        } identifier;

        struct {
            char* name;
            unsigned int index;
        }ArrayAccess;

        struct {
            char* op;
        } binaryOp;

        struct {
            char* funcName;
            struct ASTNode* CallParamList;
        }FunctionCall;

        struct {
            struct ASTNode* expr;
            struct ASTNode* nextParam;
        }CallParamList;

        struct {
            struct ASTNode* condition;
            struct ASTNode* ifBody;
        }If;
        struct{
            struct ASTNode* left;
            struct ASTNode* right;
            char* op;
        }Comparison;
        struct{
            struct ASTNode* left;
            struct ASTNode* right;
            char* op;
        }LogicalOp;

    } value;
}ASTNode;

// Function prototypes for AST handling
ASTNode* createNode(NodeType type);
void traverseAST(ASTNode* node, int level);
void freeAST(struct ASTNode* node);
void printAST(struct ASTNode* node, int indent);


#endif // AST_H