#include "optimizer.h"
#include <stdbool.h>
#include <stdio.h>

void optimizeTAC(TAC** head) {
    constantFolding(head);
    constantPropagation(head);
    copyPropagation(head);
    deadCodeElimination(head);
    optimizeIfs(head);
}
void optimizeIfs(TAC** head) {
    if (!head || !*head) return;
    
    TAC* current = *head;
    TAC* prev = NULL;
    
    while (current) {
        // Case 1: Constant condition jumps
        if (current->op && strcmp(current->op, "if") == 0) {
            if (current->next && current->arg1) {
                // If we have a constant true condition
                if (strcmp(current->arg1, "true") == 0) {
                    // Convert to direct jump
                    free(current->op);
                    current->op = strdup("jump");
                    free(current->arg1);
                    current->arg1 = NULL;
                    
                    // Remove the following jump if it exists
                    if (current->next && strcmp(current->next->op, "jump") == 0) {
                        TAC* temp = current->next;
                        current->next = temp->next;
                        free(temp);
                    }
                }
                // If we have a constant false condition
                else if (strcmp(current->arg1, "false") == 0) {
                    // Remove the if statement and keep the following jump
                    if (prev) {
                        prev->next = current->next;
                        free(current);
                        current = prev->next;
                        continue;
                    } else {
                        *head = current->next;
                        free(current);
                        current = *head;
                        continue;
                    }
                }
            }
        }
        
        // Case 2: Remove redundant jal-jump sequences
        if (current->op && strcmp(current->op, "jal") == 0 && current->next) {
            TAC* next = current->next;
            if (strcmp(next->op, "jump") == 0 && 
                current->label == next->label) {
                // Remove the redundant jump
                current->next = next->next;
                free(next);
                continue;
            }
        }
        
        // Case 3: Remove jump to next instruction
        if (current->op && strcmp(current->op, "jump") == 0 && current->next) {
            if (current->next->label == current->label) {
                // Remove the jump
                if (prev) {
                    prev->next = current->next;
                    free(current);
                    current = prev->next;
                    continue;
                } else {
                    *head = current->next;
                    free(current);
                    current = *head;
                    continue;
                }
            }
        }

        // Case 4: Remove empty jal blocks
        if (current->op && strcmp(current->op, "jal") == 0 && 
            current->next && current->next->op && 
            strcmp(current->next->op, "jal") == 0) {
            // Remove empty block
            TAC* temp = current->next;
            current->next = temp->next;
            free(temp);
            continue;
        }

        prev = current;
        current = current->next;
    }
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
    char* t0 = NULL;
    char* t1 = NULL;
    while (current != NULL) {
        // Free previous values before getting new ones
        if (t0 != NULL) {
            free(t0);
            t0 = NULL;
        }
        if (t1 != NULL) {
            free(t1);
            t1 = NULL;
        }
        
        getVal(current, &t0, &t1);

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

    // Clean up at the end
    if (t0 != NULL) free(t0);
    if (t1 != NULL) free(t1);

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
        if (strcmp(current->result, "$t0") == 0) {
            *t0 = strdup(current->arg1);  // Make a copy of the string
        } else {
            *t1 = strdup(current->arg1);  // Make a copy of the string
        }
    }
}
void constantPropagation(TAC** head) {
    printf("Inside constant propagation\n");
    TAC* current = *head;

    while (current != NULL) {
        printf("Current TAC: op=%s, arg1=%s, result=%s\n", current->op, current->arg1, current->result);
        if (current->arg2 != NULL) {
            printf("arg2: %s \n", current->arg2);
        }

        // Check if the current TAC is an assignment of a constant (Num)
        if (current->op != NULL && strcmp(current->op, "Num") == 0) {
            char* constantValue = current->arg1;
            char* targetVar = current->result;

            // Now propagate this constant through subsequent TACs
            TAC* temp = current->next;
            while (temp != NULL) {
                // Replace targetVar in arg1 or arg2 with constantValue
                if (temp->arg1 != NULL && strcmp(temp->arg1, targetVar) == 0) {
                    printf("Replacing arg1 %s with constant %s\n", temp->arg1, constantValue);
                    temp->arg1 = constantValue;
                }
                if (temp->arg2 != NULL && strcmp(temp->arg2, targetVar) == 0) {
                    printf("Replacing arg2 %s with constant %s\n", temp->arg2, constantValue);
                    temp->arg2 = constantValue;
                }

                // Stop propagating if the targetVar is reassigned
                if (temp->result != NULL && strcmp(temp->result, targetVar) == 0) {
                    printf("Stopping propagation since %s is reassigned\n", targetVar);
                    break;
                }

                temp = temp->next;
            }
        }

        current = current->next;
    }
}

void copyPropagation(TAC** head) {
    printf("Inside copy propagation\n");
    TAC* current = *head;

    while (current != NULL) {
        // Check if the current TAC is a copy assignment (e.g., t1 = t2)
        if (current->op != NULL && strcmp(current->op, "=") == 0 && current->arg1 != NULL && current->result != NULL) {
            char* sourceVar = current->arg1;
            char* targetVar = current->result;

            // Now propagate this copy through subsequent TACs
            TAC* temp = current->next;
            while (temp != NULL) {
                // Replace targetVar in arg1 or arg2 with sourceVar
                if (temp->arg1 != NULL && strcmp(temp->arg1, targetVar) == 0) {
                    printf("Replacing arg1 %s with %s\n", temp->arg1, sourceVar);
                    free(temp->arg1);
                    temp->arg1 = strdup(sourceVar);
                }
                if (temp->arg2 != NULL && strcmp(temp->arg2, targetVar) == 0) {
                    printf("Replacing arg2 %s with %s\n", temp->arg2, sourceVar);
                    free(temp->arg2);
                    temp->arg2 = strdup(sourceVar);
                }

                // Stop propagating if the targetVar is reassigned
                if (temp->result != NULL && strcmp(temp->result, targetVar) == 0) {
                    printf("Stopping propagation since %s is reassigned\n", targetVar);
                    break;
                }

                temp = temp->next;
            }
        }

        current = current->next;
    }
}
void deadCodeElimination(TAC** head){ 
    TAC* current = *head;
    TAC* prev = NULL;
    
    while (current != NULL) {
        if(current->op != NULL && strcmp(current->op, "VarDecl") == 0) {

            char* curVar = current->arg2;
            TAC* useChecker = current->next;
            bool isUsed = false;

            while (useChecker != NULL) {   
                printCurrentOptimizedTAC(useChecker);
                printf("Current Var = %s\n", curVar);
                printf("useChecker->op != NULL && strcmp(useChecker->op, \"=\") == 0 = %u\n", useChecker->op != NULL && strcmp(useChecker->op, "=") == 0);
                printf("(useChecker->arg1 != NULL &&  strcmp(useChecker->arg1, curVar) == 0) = %u\n", (useChecker->arg1 != NULL &&  strcmp(useChecker->arg1, curVar) == 0));
                if (useChecker->op != NULL && strcmp(useChecker->op, "=") == 0 &&
                    (useChecker->arg1 != NULL && strcmp(useChecker->arg1, curVar) == 0)) {
                     isUsed = true;
                     break;   
                }
                useChecker = useChecker->next;
            }

            if (!isUsed) {
                if(prev != NULL) {
                    prev->next = current->next;
                    free(current);
                    current = prev->next;
                    continue;
                } else {
                    *head = current->next;
                    free(current);
                    current = *head;
                    continue;
                }
            }
        }
        prev = current;
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
        printf("Print(%s (%s))\n", tac->arg1);
    } else if (strcmp(tac->op, "+") == 0) {
        printf("%s = %s + %s\n", tac->result, tac->arg1, tac->arg2);
    } else if (strcmp(tac->op, "Num") == 0) {
        printf("%s = %s\n", tac->result, tac->arg1);
    } else if (strcmp(tac->op, "ID") == 0) {
        printf("%s = %s (%s)\n", tac->result, tac->arg2, tac->arg1);
    }
}