#ifndef AST_H
#define AST_H

typedef struct ast {
    char* type;  // Type of the node (e.g., "number", "identifier", "operator")
    union {
        int number;
        char* identifier;
        char* operator;
    } value;
} ast;

#endif // AST_H