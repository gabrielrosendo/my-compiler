#ifndef AST_H
#define AST_H


// Define Node Types
typedef enum {
    NodeType_Program,
    NodeType_VarDeclList,
    NodeType_StmtList,
    NodeType_Stmt,
    NodeType_VArDecl,
    NodeType_Expression,
    NodeType_Identifier,
    NodeType_BinaryOp,
    NodeType_Print,
    NodeType_Assignment    
} NodeType;

// Define AST Structure
typedef struct ASTNode {
    NodeType type;
    union {
        struct {
            struct ASTNode* varDeclList;
            struct ASTNode* StmtList;
        } program;

        struct {
            struct ASTNode* VarDecl;
            struct ASTNode* nextVarDecl;
        }VarDeclList;
        struct {
            char* varType;
            char* varName;
        }VarDecl;
    };
} ASTNode;


#endif // AST_H