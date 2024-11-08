#include "codeGenerator.h"
#include <stdio.h>
#include <stdlib.h>

char* currentFunctionNameMIPS = NULL;
TAC* currentFunctionTACHead = NULL;
static FILE* outputFile;

void initCodeGenerator(const char* outputFilename, TAC* tacInstructions) {
    outputFile = fopen(outputFilename, "w");
    if (outputFile == NULL) {
        perror("Failed to open output file");
        exit(EXIT_FAILURE);
    }
    // Write the data section header
    fprintf(outputFile, ".data\n");
    fprintf(outputFile, "newline: .asciiz \"\\n\"  # Define a new line character\n");

    TAC* tac = tacInstructions;

    while (tac != NULL) {
        if (strcmp(tac->op, "VarDecl") == 0) {
            printf("\tParameter: %s %s ==> %s\n", tac->arg1, tac->arg2, tac->result);
            if (strcmp(tac->arg1, "int") == 0) {
                fprintf(outputFile, "%s: .word 0\n", tac->result);
            } else if (strcmp(tac->arg1, "float") == 0) {
                fprintf(outputFile, "%s: .float 0.0\n", tac->result);
            }
        } else if (strcmp(tac->op, "ArrayDecl") == 0) {
            printf("\tArray: %s %s[%d] ==> %s[%d]\n", tac->arg1, tac->arg2,  tac->arg3, tac->result, tac->arg3);
            fprintf(outputFile, "%s: .word 0:%d\n", tac->result, tac->arg3);
        } else if (strcmp(tac->op, "ParamDecl") == 0) {
            printf("\tVariable: %s %s ==> %s\n", tac->arg1, tac->arg2, tac->result);
             if (strcmp(tac->arg1, "int") == 0) {
                fprintf(outputFile, "%s: .word 0\n", tac->result);
            } else if (strcmp(tac->arg1, "float") == 0) {
                fprintf(outputFile, "%s: .float 0.0\n", tac->result);
            }
        } 
        tac = tac->next;
    }
    
    fprintf(outputFile, "\n");
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
    fprintf(outputFile, ".text\n.globl main\n\n");

    printf("Generating MIPS code...\n");
    while (tac != NULL) {

        if (strcmp(tac->op, "FuncDecl") == 0) {
            printf("Generating MIPS for Function Declaration\n");
            printf("Function: %s %s %s:\n", tac->arg1, tac->arg2, tac->result);
            fprintf(outputFile, "%s:\n", tac->arg2);

            currentFunctionTACHead = tac;

            if(currentFunctionNameMIPS != NULL) {
                free(currentFunctionNameMIPS);
            }

            currentFunctionNameMIPS = strdup(tac->arg2);

            // Get values for parameters off stack
            unstackParameters(tac->next);

            // Save the Return address on the stack
            fprintf(outputFile, "\taddi $sp, $sp, -4\n");
            fprintf(outputFile, "\tsw $ra, 0($sp)\n\n");

        } else if (strcmp(tac->op, "Main") == 0) {
            printf("Generating MIPS for Main Function Declaration\n");
            printf("Main:\n");
            fprintf(outputFile, "main:\n");

            currentFunctionTACHead = tac;

            if(currentFunctionNameMIPS != NULL) {
                free(currentFunctionNameMIPS);
            }

            currentFunctionNameMIPS = strdup("main");

        } else if (strcmp(tac->op, "return") == 0 && strcmp(currentFunctionNameMIPS, "main") != 0) {
            printf("Generating MIPS for Return Statement\n");
            printf("\tReturn: %s %s\n", tac->arg1, tac->arg2);

            fprintf(outputFile, "\tlw $ra, 0($sp)\t\t# Get return address off stack\n");
            fprintf(outputFile, "\taddi $sp, $sp, 4\t\t# Dealloc stack\n\n");

            if(strcmp(tac->arg1, "int") == 0) {
                fprintf(outputFile, "\taddi $sp, $sp, -4\t\t# Add space on the stack\n");
                fprintf(outputFile, "\tsw $t1, 0($sp)\t\t# Put $t1 on the stack to return\n\n");
            } else if(strcmp(tac->arg1, "float") == 0) {
                fprintf(outputFile, "\tmfc1 $t1, $f1\n");
                fprintf(outputFile, "\taddi $sp, $sp, -4\t\t# Add space on the stack\n");
                fprintf(outputFile, "\tsw $t1, 0($sp)\t\t# Put $t1 on the stack to return\n\n");
            }

            fprintf(outputFile, "\tjr $ra\t\t# Call return address to end function\n\n");

        } else if (strcmp(tac->op, "=") == 0) {
            printf("Generating MIPS for Assignment operation\n");
            if (strcmp(tac->arg2, "$t1") == 0) {
                fprintf(outputFile, "\tla $t2, %s\t\t# Load address of %s into $t2\n", tac->result, tac->result);
                fprintf(outputFile, "\tsw $t1, 0($t2)\t\t# Save $t1 into variable\n\n");
            } else if (strcmp(tac->arg2, "$f1") == 0) {
                fprintf(outputFile, "\ts.s $f1, %s\n\n", tac->result);
            }

        } else if (strcmp(tac->op, "ArrayAssingment") == 0) {
            printf("Generating MIPS for Array Assignment operation\n");
            printf("\t%s (%s[%d]) = %s\n", tac->result, tac->arg1, tac->arg3, tac->arg2);
            fprintf(outputFile, "\tla $t2, %s\t\t# Load address of %s into $t2\n", tac->result, tac->result);
            fprintf(outputFile, "\tsw $t1, %d($t2)\t\t# Save $t1 into array with the offset calculated with the index\n\n", tac->arg3 * 4);

        } else if (strcmp(tac->op, "Print") == 0) {
            printf("Generating MIPS for Print operation\n");

            if (strcmp(tac->arg1, "$t1") == 0) {
                fprintf(outputFile, "\tmove $a0, %s\t\t# Move %s to $a0 for print output\n", tac->arg1, tac->arg1);
                fprintf(outputFile, "\tli $v0, 1\t\t# Load print int system function\n");   
            } else if (strcmp(tac->arg1, "$f1") == 0) {
                fprintf(outputFile, "\tmov.s $f12, %s\n", tac->arg1);
                fprintf(outputFile, "\tli $v0, 2\t\t# Load print float system function\n");  
            }
            fprintf(outputFile, "\tsyscall\t\t# Syscall to print int\n\n");

            fprintf(outputFile, "\tli $v0, 4\t\t# Load syscall code for print string\n");
            fprintf(outputFile, "\tla $a0, newline\t\t# Load address of newline into $a0\n");
            fprintf(outputFile, "\tsyscall\t\t# Syscall to print string\n\n");

        } else if (strcmp(tac->op, "+") == 0) {
            printf("Generating MIPS for Addition\n");
            if (strcmp(tac->arg1, "$t0") == 0) {
                fprintf(outputFile,"\tadd %s, %s, %s\n", tac->result, tac->arg1, tac->arg2);
            } else if (strcmp(tac->arg1, "$f0") == 0) {
                fprintf(outputFile,"\tadd.s %s, %s, %s\n", tac->result, tac->arg1, tac->arg2);
            }

        } else if (strcmp(tac->op, "-") == 0) {
            printf("Generating MIPS for Subtraction\n");
            printf("%s = %s - %s\n", tac->result, tac->arg1, tac->arg2);
            printf("\tsub %s, %s, %s\n", tac->result, tac->arg1, tac->arg2);
            if (strcmp(tac->arg1, "$t0") == 0) {
                fprintf(outputFile, "\tsub %s, %s, %s\n", tac->result, tac->arg1, tac->arg2);
            } else if (strcmp(tac->arg1, "$f0") == 0) {
                fprintf(outputFile, "\tsub.s %s, %s, %s\n", tac->result, tac->arg1, tac->arg2);
            }

        } else if (strcmp(tac->op, "*") == 0) {
            printf("Generating MIPS for Multiplication\n");
            printf("%s = %s * %s\n", tac->result, tac->arg1, tac->arg2);
            printf("\tmult %s, %s, %s\n", tac->result, tac->arg1, tac->arg2);
            if (strcmp(tac->arg1, "$t0") == 0) {
                fprintf(outputFile, "\tmul %s, %s, %s\n", tac->result, tac->arg1, tac->arg2);
            } else if (strcmp(tac->arg1, "$f0") == 0) {
                fprintf(outputFile, "\tmul.s %s, %s, %s\n", tac->result, tac->arg1, tac->arg2);
            }

        } else if (strcmp(tac->op, "/") == 0) {
            printf("Generating MIPS for Division\n");
            printf("%s = %s / %s\n", tac->result, tac->arg1, tac->arg2);
            printf("\tdiv %s, %s\n\tmflo %s\n", tac->arg1, tac->arg2, tac->result);
            if (strcmp(tac->arg1, "$t0") == 0) {
                fprintf(outputFile, "\tdiv %s, %s\n\tmflo %s\n", tac->arg1, tac->arg2, tac->result);
            } else if (strcmp(tac->arg1, "$f0") == 0) {
                fprintf(outputFile, "\tdiv.s %s, %s, %s\n", tac->result,tac->arg1, tac->arg2);
            }
            
        } else if (strcmp(tac->op, "move") == 0) {
            printf("Generating MIPS for move\n");
            if (strcmp(tac->arg1, "$t0") == 0 || strcmp(tac->arg1, "$t1") == 0 || strcmp(tac->arg1, "$t4") == 0) {
                fprintf(outputFile, "\tmove %s, %s\n", tac->result, tac->arg1);
            } else if (strcmp(tac->arg1, "$f0") == 0 || strcmp(tac->arg1, "$f1") == 0 || strcmp(tac->arg1, "$f4") == 0) {
                fprintf(outputFile, "\tmov.s %s, %s\n", tac->result, tac->arg1);
            }

        } else if (strcmp(tac->op, "Num") == 0) {
            printf("Generating MIPS for Number\n");
            printf("%s = %s\n", tac->result, tac->arg1);
            printf("\tli %s, %s\n", tac->result, tac->arg1);
            if (strcmp(tac->result, "$t0") == 0 || strcmp(tac->result, "$t1") == 0) {
                fprintf(outputFile, "\tli %s, %s\n", tac->result, tac->arg1);
            } else if (strcmp(tac->result, "$f0") == 0 || strcmp(tac->result, "$f1") == 0) {
                fprintf(outputFile, "\tli.s %s, %s\n", tac->result, tac->arg1);
            }

        } else if (strcmp(tac->op, "ID") == 0) {
            printf("Generating MIPS for Identifier\n");
            printf("%s = %s (%s)\n", tac->result, tac->arg2, tac->arg1);
            printf("\tla %s, %s\n", tac->result, tac->arg2);
            if (strcmp(tac->result, "$t0") == 0 || strcmp(tac->result, "$t1") == 0) {
                fprintf(outputFile, "\tla $t2, %s\n", tac->arg2);
                fprintf(outputFile, "\tlw %s, 0($t2)\n", tac->result);
            } else if (strcmp(tac->result, "$f0") == 0 || strcmp(tac->result, "$f1") == 0) {
                fprintf(outputFile, "\tl.s %s, %s\n", tac->result, tac->arg2);
            }

        } else if (strcmp(tac->op, "ArrayAccess") == 0) {
            printf("Generating MIPS for ArrayAccess\n");
             printf("\t%s = %s (%s[%d])\n", tac->result, tac->arg2, tac->arg1, tac->arg3);
            fprintf(outputFile, "\tla $t2, %s\n", tac->arg2);
            fprintf(outputFile, "\tlw %s, %d($t2)\n", tac->result, 4 * tac->arg3);

        } else if (strcmp(tac->op, "FuncCall") == 0) {
            printf("Generating MIPS for Function Call\n");
            printf("\tFunction Call: %s => %s\n", tac->arg1, tac->result);
            fprintf(outputFile, "\taddi $sp, $sp, -4\n");
            fprintf(outputFile, "\tsw $t1, 0($sp)\n\n");
            fprintf(outputFile, "\taddi $sp, $sp, -4\n");
            fprintf(outputFile, "\tmfc1 $t1, $f1\n");
            fprintf(outputFile, "\tsw $t1, 0($sp)\n\n");
            stackAllVariable();

        } else if (strcmp(tac->op, "FuncCallEnd") == 0) {
            printf("Generating MIPS for Function Call End\n");
            printf("\tFunction Call End: %s => %s\n", tac->arg1, tac->result);
            fprintf(outputFile, "\tjal %s\n\n", tac->arg1);
            fprintf(outputFile, "\tlw $t3, 0($sp)\n");
            fprintf(outputFile, "\taddi $sp, $sp, 4\n\n");
            unStackAllVariable(currentFunctionTACHead->next);
            fprintf(outputFile, "\tlw $t1, 0($sp)\n");
            fprintf(outputFile, "\tmtc1 $t1, $f1\n");
            fprintf(outputFile, "\taddi $sp, $sp, 4\n\n");
            fprintf(outputFile, "\tlw $t1, 0($sp)\n");
            fprintf(outputFile, "\taddi $sp, $sp, 4\n\n");
            if (strcmp(tac->result, "$t0") == 0 || strcmp(tac->result, "$t1") == 0) {
                fprintf(outputFile, "\tmove %s, $t3\n\n", tac->result);
            } else if (strcmp(tac->result, "$f0") == 0 || strcmp(tac->result, "$f1") == 0) {
                fprintf(outputFile, "\tmtc1 $t3, %s\n\n", tac->result);
            }

        } else if (strcmp(tac->op, "ParamCall") == 0) {
            printf("Generating MIPS for Parameter Call\n");
            printf("\tParameter Call: %s\n", tac->arg1);
            if (strcmp(tac->arg1, "$t1") == 0) {
                fprintf(outputFile, "\taddi $sp, $sp, -4\n");
                fprintf(outputFile, "\tsw $t1, 0($sp)\n\n");
            } else if (strcmp(tac->arg1, "$f1") == 0) {
                fprintf(outputFile, "\tmfc1 $t1, $f1\n");
                fprintf(outputFile, "\taddi $sp, $sp, -4\n");
                fprintf(outputFile, "\tsw $t1, 0($sp)\n\n");
            }
        
        } else if (strcmp(tac->op, "IntToFloat") == 0) {
            printf("Generating MIPS for Int to Float register\n");
            fprintf(outputFile, "\tmtc1 %s, %s\n", tac->arg1, tac->result);
            fprintf(outputFile, "\tcvt.s.w %s, %s\n", tac->result, tac->result);
        
        } else if (strcmp(tac->op, "FloatToInt") == 0) {
            printf("Generating MIPS for Float to Int register\n");
            fprintf(outputFile, "\tcvt.w.s %s, %s\n", tac->arg1, tac->arg1);
            fprintf(outputFile, "\tmfc1 %s, %s\n", tac->result, tac->arg1);

        } else {
            printf("Unknown TAC operation: %s\n", tac->op);
        }

        tac = tac->next;
    }

    // Exit program
    fprintf(outputFile, "\tli $v0, 10\n\tsyscall\n");
}

void unstackParameters(TAC* tac) {
    if(strcmp(tac->op, "ParamDecl") != 0) return;

    unstackParameters(tac->next);

    if (strcmp(tac->arg1, "int") == 0) {
        fprintf(outputFile, "\tlw $t1, 0($sp)\n");
        fprintf(outputFile, "\tla $t2, %s\n", tac->result);
        fprintf(outputFile, "\tsw $t1, 0($t2)\n");
        fprintf(outputFile, "\taddi $sp, $sp, 4\n\n"); 
    } else if (strcmp(tac->arg1, "float") == 0) {
        fprintf(outputFile, "\tlw $t1, 0($sp)\n");
        fprintf(outputFile, "\tmtc1 $t1, $f1\n");
        fprintf(outputFile, "\ts.s $f1, %s\n", tac->result);
        fprintf(outputFile, "\taddi $sp, $sp, 4\n\n");
    }
}

void stackAllVariable() {
    TAC* tac = currentFunctionTACHead;
    tac = tac->next;
    
    while (strcmp(tac->op, "ParamDecl") == 0 || strcmp(tac->op, "VarDecl") == 0) {
        if (strcmp(tac->arg1, "int") == 0) {
            fprintf(outputFile, "\tla $t2, %s\n", tac->result);
            fprintf(outputFile, "\tlw $t1, 0($t2)\n");
            fprintf(outputFile, "\taddi $sp, $sp, -4\n");
            fprintf(outputFile, "\tsw $t1, 0($sp)\n\n");  
        } else if (strcmp(tac->arg1, "float") == 0) {
            fprintf(outputFile, "\tl.s $f1, %s\n", tac->result);
            fprintf(outputFile, "\tmfc1 $t1, $f1\n");
            fprintf(outputFile, "\taddi $sp, $sp, -4\n");
            fprintf(outputFile, "\tsw $t1, 0($sp)\n\n"); 
        }
        tac = tac->next;
    }
}

void unStackAllVariable(TAC* tac) {
    if(strcmp(tac->op, "ParamDecl") != 0 && strcmp(tac->op, "VarDecl") != 0) return;

    unStackAllVariable(tac->next);

    if (strcmp(tac->arg1, "int") == 0) {
        fprintf(outputFile, "\tlw $t1, 0($sp)\n");
        fprintf(outputFile, "\tla $t2, %s\n", tac->result);
        fprintf(outputFile, "\tsw $t1, 0($t2)\n");
        fprintf(outputFile, "\taddi $sp, $sp, 4\n\n"); 
    } else if (strcmp(tac->arg1, "float") == 0) {
        fprintf(outputFile, "\tlw $t1, 0($sp)\n");
        fprintf(outputFile, "\tmtc1 $t1, $f1\n");
        fprintf(outputFile, "\ts.s $f1, %s\n", tac->result);
        fprintf(outputFile, "\taddi $sp, $sp, 4\n\n");
    }
}