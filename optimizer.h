/* The optimizer should implement the following:
- constant folding
- constant propagation
- dead code elimination
- generate optimized TAC
*/

#ifndef OPTIMIZER_H
#define OPTIMIZER_H

#include "semantic.h"
#include <stdbool.h>
#include <ctype.h>

void optimizeTAC(TAC** head);
bool isConstant(const char* str);
bool isVariable(const char* str);
void constantFolding(TAC** head);
void getVal(TAC* current, char** t0, char** t1);
void constantPropagation(TAC** head);
void copyPropagation(TAC** head);
void deadCodeElimination(TAC** head);
void printOptimizedTAC(const char* filename, TAC* head);
void printCurrentOptimizedTAC(TAC* current);

#endif // OPTIMIZER_H