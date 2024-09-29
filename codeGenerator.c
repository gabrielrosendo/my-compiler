#include "codeGenerator.h"
#include <stdio.h>
#include <stdlib.h>

static FILE* outputFile;

void initCodeGenerator(const char* outputFilename) {
    outputFile = fopen(outputFilename, "w");
    if (outputFile == NULL) {
        perror("Failed to open output file");
        exit(EXIT_FAILURE);
    }
    // Write the data section header
    fprintf(outputFile, ".data\n");
}

void finalizeCodeGenerator(const char* outputFilename) {
    if (outputFile) {
        fclose(outputFile);
        printf("MIPS code generated and saved to file %s\n", outputFilename);
        outputFile = NULL;
    }
}

void generateMIPS(TAC* tacInstructions) {  
    TAC* tac = tacInstructions;
    fprintf(outputFile, ".text\n.globl main\nmain:\n");

    printf("Generating MIPS code...\n");
    while (tac != NULL) {
        if (strcmp(tac->op, "VarDecl") == 0) {
            printf("%s %s ==> %s\n", tac->arg1, tac->arg2, tac->result);
        } else if (strcmp(tac->op, "=") == 0) {
            printf("Generating MIPS for Assignment operation\n");
            printf("%s (%s) = %s\n", tac->result, tac->arg1, tac->arg2);
            printf("\tmove %s, %s\n", tac->result, tac->arg2);
            fprintf(outputFile, "\tmove %s, %s\n", tac->result, tac->arg2);
        } else if (strcmp(tac->op, "+") == 0) {
            printf("Generating MIPS for Addition\n");
            printf("%s = %s + %s\n", tac->result, tac->arg1, tac->arg2);
            printf("\tadd %s, %s, %s\n", tac->result, tac->arg1, tac->arg2);
            fprintf(outputFile, "\tadd %s, %s, %s\n", tac->result, tac->arg1, tac->arg2);
        } else if (strcmp(tac->op, "Num") == 0) {
            printf("Generating MIPS for Number\n");
            printf("%s = %s\n", tac->result, tac->arg1);
            printf("\tli %s, %s\n", tac->result, tac->arg1);
            fprintf(outputFile, "\tli %s, %s\n", tac->result, tac->arg1);
        } else if (strcmp(tac->op, "ID") == 0) {
            printf("Generating MIPS for Identifier\n");
            printf("%s = %s (%s)\n", tac->result, tac->arg2, tac->arg1);
            printf("\tmove %s, %s\n", tac->result, tac->arg2);
            fprintf(outputFile, "\tmove %s, %s\n", tac->result, tac->arg2);
        }
        tac = tac->next;
    }

    // Exit program
    fprintf(outputFile, "\tli $v0, 10\n\tsyscall\n");
}