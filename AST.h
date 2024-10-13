#ifndef AST_H
#define AST_H


// Define Node Types
typedef enum {
    NodeType_Program,
    NodeType_FuncDeclList,
    NodeType_FuncDecl,
    NodeType_MainFunc,
    NodeType_ParamList,
    NodeType_ParamDecl,
    NodeType_Body,
    NodeType_FunctionCall,
    NodeType_CallParamList,
    NodeType_VarDeclList,
    NodeType_VarDecl,
    NodeType_StmtList,
    NodeType_Stmt,
    NodeType_Assignment,
    NodeType_Print,
    NodeType_Expression,
    NodeType_Number,
    NodeType_Identifier,
    NodeType_BinaryOp
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
            struct ASTNode* VarDeclList;
            struct ASTNode* StmtList;
        }Body;

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
            struct ASTNode* stmt;
            struct ASTNode* nextStmt;
        } StmtList;

        struct {
            char* varName;
            char* op;
            struct ASTNode* expr;
        } assignment;

        struct{
            char* name;
        } print;

        struct {
            struct ASTNode* left;
            struct ASTNode* right;
            char* op;
        } Expression;

        struct {
           int number;
       } Number;

        struct {
            char* name;
        } identifier;

        struct {
            char* funcName;
            struct ASTNode* CallParamList;
        }FunctionCall;

        struct {
            char* op;
        } binaryOp;

        struct {
            struct ASTNode* expr;
            struct ASTNode* nextParam;
        }CallParamList;
    } value;
}ASTNode;

// Function prototypes for AST handling
ASTNode* createNode(NodeType type);
void traverseAST(ASTNode* node, int level);
void freeAST(struct ASTNode* node);
void printAST(struct ASTNode* node, int indent);


#endif // AST_H