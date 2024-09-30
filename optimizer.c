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
        
            
            printf(current->arg2);
            printf("\n");
            printf("$t0: %s\n", t0);
            printf("$t1: %s\n", t1);
            t1 = t0;
        }
        current = current->next;
    }
}