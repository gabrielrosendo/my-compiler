#include "optimizer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function prototypes
void optimizeTAC(TAC** head);
void constantFolding(TAC** head);
void printCurrentOptimizedTAC(TAC* tac);
TAC* readTACFromFile(const char* filename);

// Helper functions
bool isConstant(const char* str);
bool isNumber(const char* str);
const char* getTempValue(TAC* head, const char* temp);

int main() {
    // Read the TAC list from the file
    TAC* tacHead = readTACFromFile("TAC.ir");

    // Print the original TAC list
    printf("Original TAC List:\n");

    // Optimize the TAC list
    optimizeTAC(&tacHead);

    // Print the optimized TAC list
    printf("\nOptimized TAC List:\n");

    // Free the TAC list
    TAC* current = tacHead;
    while (current != NULL) {
        TAC* next = current->next;
        free(current->op);
        free(current->arg1);
        free(current->arg2);
        free(current->result);
        free(current);
        current = next;
    }

    return 0;
}

TAC* readTACFromFile(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Failed to open file");
        exit(EXIT_FAILURE);
    }

    TAC* head = NULL;
    TAC* tail = NULL;
    char line[256];

    while (fgets(line, sizeof(line), file)) {
        TAC* newTAC = (TAC*)malloc(sizeof(TAC));
        newTAC->next = NULL;

        char* token = strtok(line, " ");
        newTAC->op = strdup(token);

        token = strtok(NULL, " ");
        newTAC->arg1 = strdup(token);

        token = strtok(NULL, " ");
        if (token) {
            newTAC->arg2 = strdup(token);
        } else {
            newTAC->arg2 = NULL;
        }

        token = strtok(NULL, "\n");
        newTAC->result = strdup(token);

        if (head == NULL) {
            head = newTAC;
            tail = newTAC;
        } else {
            tail->next = newTAC;
            tail = newTAC;
        }
    }

    fclose(file);
    return head;
}

