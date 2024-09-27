#include <stdio.h>
#include <stdlib.h>
#include "symbolBST_Test.h"

// Declare external functions from the generated files
extern int yyparse();   // Bison's parser function
extern int yylex();     // Flex's lexer function
extern FILE *yyin;      // Input file for the lexer
extern int yydebug;     // Debug mode for Bison

int main(int argc, char **argv) {
    #ifdef YYDEBUG
        yydebug = 1;  // Enable Bison debug if compiled with YYDEBUG
    #endif

    printf("Compiler started. \n\n");

    // Check if an input file is provided
    if (argc > 1) {
        yyin = fopen(argv[1], "r");
        if (!yyin) {
            perror(argv[1]);  // Report error if file cannot be opened
            return 1;
        }
    }

    // Choose whether to run the parser (yyparse) or lexer (yylex)
    // For this example, we’ll use yyparse() as the primary function.
    yyparse();  // Call parser (which may call yylex() internally)

    // Optionally, if you want to use yylex directly:
    // yylex();  // Directly call the lexer if needed

    printf("\n\nTESTING\n\n");

    // Testing the symbolBST
    printf("symbolBST testing: \n");
    symbolBST_TestCreation();
    symbolBST_Test_AddSymbol_SingleIntegerInput();
    symbolBST_Test_AddSymbol_MultipleIntegerInputs();

    return 0;
}
