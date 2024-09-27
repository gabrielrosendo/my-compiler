#ifndef AST_H
#define AST_H


// Define Node Types
typedef enum {
    NodeType_Program,
    NodeType_VarDeclList,
    NodeType_StmtList,
    NodeType_Stmt,
    NodeType_VarDecl,
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
            struct ASTNode* VarDeclList;
            struct ASTNode* StmtList;
        } program;

        struct {
            struct ASTNode* VarDecl;
            struct ASTNode* nextVarDecl;
        }VarDeclList;
        
        struct {
            struct ASTNode* stmt;
            struct ASTNode* nextStmt;
        } StmtList;

        struct {
            char* varType;
            char* varName;
        }VarDecl;

        struct {
            struct ASTNode* left;
            struct ASTNode* right;
            char op;
        } binaryOp;
        struct {
            char* name;
        } identifier;
        struct {
            char* op; 
            char* varName;
            struct ASTNode* expr;
        }assignment;
        struct{
            struct ASTNode* expr;
        }print;

    } value;
}ASTNode;

// Function prototypes for AST handling
ASTNode* createNode(NodeType type);
void traverseAST(ASTNode* node, int level);


#endif // AST_H