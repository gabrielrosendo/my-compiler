#include "optimizer.h"
#include <stdbool.h>
#include <stdio.h>


void optimizeTAC(TAC** head) {
    constantFolding(head);
    /*
    constantPropagation(head);
    copyPropagation(head);
    deadCodeElimination(head);
    */
}
bool isConstant(const char* str) {
    if (str == NULL || *str == '\0') {
        return false; // Empty string is not a constant
    }

    // Optional: Handle negative numbers
    if (*str == '-') {
        ++str;
    }

    // Check if string is numeric
    while (*str) {
        if (!isdigit((unsigned char)*str)) {
            return false; // Found a non-digit character
        }
        ++str;
    }

    return true; // All characters were digits
}


void constantFolding(TAC** head) {
    TAC* current = *head;
    TAC* prev = NULL;
    TAC* prev_prev = NULL;
    TAC* prev_prev_prev = NULL;
    char* t0 = '\0';
    char* t1 = '\0';
    while (current != NULL) {
        if (current->op != NULL && strcmp(current->op, "Num") == 0 || strcmp(current->op, "ID") == 0){
            printf("INSIDE\n");
            if(strcmp(current->result, "$t0") == 0) {
                t0 = current->arg1;
                printf("$t0: %s\n", t0);
            } else {
                t1 = current->arg1;
                printf("$t1: %s\n", t1);
            }
        }
        if (current->op != NULL && strcmp(current->op, "+") == 0 || strcmp(current->op, "-") == 0) {
        
            
            printf("%s", current->arg2);
            printf("\n");
            printf("$t0: %s\n", t0);
            printf("$t1: %s\n", t1);
        if (isConstant(t0) && isConstant(t1)) {
            // Needs to be optimized
            int result = 0;
            if (strcmp(current->op, "+") == 0) {
                result = atoi(t0) + atoi(t1);
            } else {
                result = atoi(t0) - atoi(t1);
            }
            char resultStr[20];
            sprintf(resultStr, "%d", result);
            printf("Printing result = %s\n", resultStr);
            free(t0);
            free(t1);
            // Remove unnecessary instructions
            if (prev_prev_prev != NULL) {
                free(prev);
                free(prev_prev);
                prev_prev_prev->next = current;
            } else {
                *head = current;
            }
            current->arg1 = strdup(resultStr);
            current->arg2 = NULL;
            current->op = "Num";
            current->result = strdup("$t1"); // Ensure the result is updated correctly
            printf("Constant folding applied\n");
            printCurrentOptimizedTAC(current);
        }
        t1 = t0;
        }
        prev_prev_prev = prev_prev;
        prev_prev = prev;
        prev = current;
        current = current->next;
    }
}

void printCurrentOptimizedTAC(TAC* tac) {
    if (strcmp(tac->op, "VarDecl") == 0) {
        printf("%s %s ==> %s\n", tac->arg1, tac->arg2, tac->result);
    } else if (strcmp(tac->op, "=") == 0) {
        printf("%s (%s) = %s\n", tac->result, tac->arg1, tac->arg2);
    } else if (strcmp(tac->op, "Print") == 0) {
        printf("Print(%s (%s))\n", tac->result, tac->arg1);
    } else if (strcmp(tac->op, "+") == 0) {
        printf("%s = %s + %s\n", tac->result, tac->arg1, tac->arg2);
    } else if (strcmp(tac->op, "Num") == 0) {
        printf("%s = %s\n", tac->result, tac->arg1);
    } else if (strcmp(tac->op, "ID") == 0) {
        printf("%s = %s (%s)\n", tac->result, tac->arg2, tac->arg1);
    }
}