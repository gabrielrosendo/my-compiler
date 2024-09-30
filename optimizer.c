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
    while (current != NULL) {
        if (current->op != NULL && strcmp(current->op, "+") == 0 || strcmp(current->op, "-") == 0) {
            printf(current->arg2);
            printf("\n");
        }
        current = current->next;
    }
}