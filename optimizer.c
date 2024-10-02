#include "optimizer.h"
#include <stdbool.h>
#include <stdio.h>


void optimizeTAC(TAC** head) {
    constantPropagation(head);
    constantFolding(head); // This is currently giving a segmentation fault

    /*
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
        getVal(current, &t0, &t1);

        if (current != NULL && current->op != NULL && strcmp(current->op, "+") == 0 || strcmp(current->op, "-") == 0) {
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

                printf("----Constant Folding----\n");
                printf("prev_prev_prev = ");
                printCurrentOptimizedTAC(prev_prev_prev);
                printf("prev_prev = ");
                printCurrentOptimizedTAC(prev_prev);
                printf("prev = ");
                printCurrentOptimizedTAC(prev);
                printf("current = ");
                printCurrentOptimizedTAC(current);
                printf("----END OF Constant Folding----\n");

                printf("Printing result = %s\n", resultStr);

                // Remove unnecessary instructions
                if (prev_prev_prev != NULL) {
                    free(prev);
                    free(prev_prev);
                    prev = NULL;
                    prev_prev = NULL;

                    prev_prev_prev->next = current;
                } else {
                    if(prev != NULL) {
                        free(prev);
                    }
                    if(prev_prev != NULL) {
                        free(prev_prev);
                    }
                    *head = current;
                }
                current->arg1 = strdup(resultStr);
                current->arg2 = NULL;
                current->op = "Num";

                if(strcmp(prev_prev_prev->op, "+") == 0 || strcmp(prev_prev_prev->op, "+") == 0 || strcmp(prev_prev_prev->op, "ID") == 0) {
                    current->result = strdup("$t0");
                        t1 = resultStr;
                        t0 = '\0';
                } else {
                    current->result = strdup("$t1");
                    t1 = resultStr;
                    t0 = '\0';
                }
                printf("Constant folding applied\n");
                printCurrentOptimizedTAC(current);
            }

            if (t0 != '\0') {
                t1 = t0;
            }

        } else if (prev != NULL && prev->op != NULL && (strcmp(prev->op, "+") == 0 || strcmp(prev->op, "-") == 0)) {
            printf("----AHHHH----\n");
            printf("t1 = %s\n", t1);
            printf("t0 = %s\n", t0);
            if (isConstant(t0) && isConstant(t1)) {
                // Needs to be optimized
                int result = 0;
                if (strcmp(prev->op, "+") == 0) {
                    result = atoi(t0) + atoi(t1);
                } else {
                    result = atoi(t0) - atoi(t1);
                }
                char resultStr[20];
                sprintf(resultStr, "%d", result);

                printf("----Constant Folding CASE 2----\n");
                printf("prev_prev_prev = ");
                printCurrentOptimizedTAC(prev_prev_prev);
                printf("prev_prev = ");
                printCurrentOptimizedTAC(prev_prev);
                printf("prev = ");
                printCurrentOptimizedTAC(prev);
                printf("current = ");
                printCurrentOptimizedTAC(current);
                printf("t1 = %s\n", t1);
                printf("t0 = %s\n", t0);
                printf("----END OF Constant Folding CASE 2----\n");

                printf("Printing result = %s\n", resultStr);

                free(prev);
                free(prev_prev);
                prev = NULL;
                prev_prev = NULL;
                
                prev_prev_prev->next = current;

                current->arg1 = strdup(resultStr);
                current->arg2 = NULL;
                current->op = "Num";
                current->result = strdup("$t0");
                t1 = resultStr;
                t0 = '\0';

                printf("Constant folding applied CASE 2\n");
                printCurrentOptimizedTAC(current);
            }

        }

        printf("----PRINTING PREVS----\n");
        printf("prev_prev_prev = ");
        printCurrentOptimizedTAC(prev_prev_prev);
        printf("prev_prev = ");
        printCurrentOptimizedTAC(prev_prev);
        printf("prev = ");
        printCurrentOptimizedTAC(prev);
        printf("----END PRINTING PREVS----\n");

        if(prev_prev != NULL) {
            prev_prev_prev = prev_prev;
        }
        prev_prev = prev;
        prev = current;
        current = current->next;
    }

    free(t1);
}

void updateTemp(TAC* current, char** t0, char** t1) {
    if (current->op != NULL && (strcmp(current->op, "Num") == 0 || strcmp(current->op, "ID") == 0)) {
        printf("INSIDE\n");
        if (strcmp(current->result, "$t0") == 0) {
            *t0 = current->arg1;
            printf("$t0: %s\n", *t0);
        } else {
            *t1 = current->arg1;
            printf("$t1: %s\n", *t1);
        }
    }
}
void getVal(TAC* current, char** t0, char** t1) {
    if (current->op != NULL && (strcmp(current->op, "Num") == 0 || strcmp(current->op, "ID") == 0)) {
        printf("INSIDE\n");
        if (strcmp(current->result, "$t0") == 0) {
            *t0 = current->arg1;
            printf("$t0: %s\n", *t0);
        } else {
            *t1 = current->arg1;
            printf("$t1: %s\n", *t1);
        }
    }
}

void constantPropagation(TAC** head) {
    printf("Inside constant propagation\n");
    TAC* current = *head;
    char* t0 = NULL;
    char* t1 = NULL;
    while (current != NULL) {
        printf("Current TAC: op=%s, arg1=%s, result=%s\n", current->op, current->arg1, current->result);\
        // Check if the current TAC is an assignment
        // and the right-hand side is a constant
        if (current->op != NULL && strcmp(current->op, "Num") == 0) {
            printf("result: %s\n", current->result);
            printf("Num: %s\n", current->arg1);
            // Num found
            // CHange every instance of current->result to current->arg1
            TAC* temp = *head;

        }
        current = current->next;
    }
}
void printCurrentOptimizedTAC(TAC* tac) {
    if (tac == NULL) {
        printf("NULL\n");
    } else if (strcmp(tac->op, "VarDecl") == 0) {
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