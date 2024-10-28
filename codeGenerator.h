// codeGenerator.h

#ifndef CODE_GENERATOR_H
#define CODE_GENERATOR_H

#include "AST.h" // Include your AST definition
#include "semantic.h" // Include your TAC definition
#include <stdbool.h>

#define NUM_TEMP_REGISTERS 10

// Initializes code generation, setting up any necessary structures
void initCodeGenerator(const char* outputFilename, TAC* tacInstructions);

// Generates MIPS assembly code from the provided TAC
void generateMIPS(TAC* tacInstructions);

// Finalizes code generation, closing files and cleaning up
void finalizeCodeGenerator(const char* outputFilename);

// Get put all variables onto the stack
void stackAllVariable();

// Get all variables and parameters off the stack
void unStackAllVariable(TAC* tac);

void unstackParameters(TAC* tac);

#endif // CODE_GENERATOR_H
