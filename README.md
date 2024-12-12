# my-compiler
For this final version, we have added support for while loops

Videos: <br>
Grant Burk: https://www.loom.com/share/d66ec9eafb674157bc3bea63b961759e?sid=4ac04077-d0e8-4888-989a-ba08b0a67d46 <br>
Gabriel Marcelino: https://www.loom.com/share/af0a103ac47f4f889d03a65d9f6833ea

## Comand to compile code from the input.txt file:
make parser

## Performance Metrics
Metrics were obtained by averaging the times for 6 simple test inputs:
- **input-array.txt**: Test for array operations and assignments.
- **input-bools.txt**: Test for boolean logic and conditions.
- **input-float.txt**: Test for float operations.
- **input-funcs.txt**: Test for function calls and nested function usage.
- **input-ifs.txt**: Test for `if` and `if-else` branches.
- **input-whiles.txt**: Test for `while` loops.

### a) Compilation Time
- Times(seconds): 0.016, 0.045, 0.032, 0.017, 0.021, 0.016
- Average time for code compilation: **0.0245 seconds**
- Factors affecting compilation time:
  - Code complexity
  - Use of conditional expressions and loops
  - Size of input program

### b) Execution Time of Compiler Code
- Times(seconds): 0.014, 0.017, 0.016, 0.016, 0.017, 0.017
- Average execution time of compiled programs: **0.0162 seconds**

## Usage and Syntax
While Loops: <br>

    Format: <br>
        While ((Conditional Expression Expansion)) { (Code Block) } <br>
        or: <br>
        While ((Expression)) { (Code Block) } <br>
        - The condition must result in a boolean value. <br>

    Uses: <br>
        - Inside functions after variable declarations: <br>
            while (x < 10) { Code Block } <br>
        - Inside if statements: <br>
            if (x > 5) { while (y < 10) { Code Block } } <br>
        - Nested inside other while loops: <br>
            while (x < 10) { while (y > 5) { Code Block } } <br>

    ** Can Use Logical operators: <br>
        - ex: while (x && y) { Code Block } <br>
            or while (x || y) { Code Block } <br>

Conditional Expression Expansion: <br>

    Use Operators: ||, && <br>
    
    Format: <br>
        (Conditional Expression) (Conditional Expansion Operator) (Conditional Expression) <br>
        also: (Conditional Expression Expansion) (Conditional Expansion Operator) (Conditional Expression) <br>

    Uses: <br>
        Assign to a variable: <br>
            bool x; <br>
            x = Conditional Expression Expansion; <br>

        If Statement: <br>
            if (Conditional Expression Expansion) {} <br> 
            else if (Conditional Expression Expansion) {} <br>

If Statements: <br>

    Starts with if: <br>
        if (Conditional Statement) { Statements } <br>

    Can be expanded: <br>
        if (Conditional Statement) { <br>
        } else if (Conditional Statement) { Statements } <br>

    Also: <br>
        if (Conditional Statement) { <br>
        } else { Statements } <br>

    So, start with if, then there can be as many else if statements after the initial if. Else statements only come at the end of the if statement. <br>

    Conditional Statements are expression and conditional expressions that have boolean values <br>
        - basic values (true, false) <br>
        - variable with bool type <br>
        - functions with bool type <br>
        - Conditional Expression <br>
        - Conditional Expression Expansions <br>

Compiler rules: <br>

Function decl: <br>
    type ID(parameters: type ID...) {body + return} <br>
    - Arrays cannot be returned or be parameters <br>

Parameter Decl: <br>
    type ID; <br>
    - Must occur before statements <br>

Array decl: <br>
    type ID[size]; <br>
    - Must occur before statements <br>

Statements: <br>
    Variable assignment: <br>
        ID = expression; <br>
        - ID has to be declared as a variable <br>
        - expression has to be the same type but float and int are interchangeable <br>
    
    Array assignment: <br>
        ID[index] = expression; <br>
        - ID has to be declared as an array; <br>
        - expression has to be the same type but float and int are interchangeable <br>
    
    Print: <br>
        print(expression); <br>

Expressions: <br>
    Use operations: +, -, *, / <br>

    Numbers: <br>
        1, 2, 3 <br>
    
    Floats: <br>
        1.2, 2.4, 5.3 <br>

    Characters: <br>
        'a', 'b', 'C' <br>
        - Doesn't work with operations <br>
    
    Boolean: <br>
        true, false <br>
        - Doesn't work with operations <br>
    
    Function call: <br>
        functionID(expression, expression...); <br>

    Var reference: <br>
        ID <br>
        - ID has to be a declared variable <br>

    Array reference: <br>
        ID[index] <br>
        - ID has to be a declared array <br>
        - index has to be 0 <= index < size <br>

Conditional Expressions: <br>

    Use operators: <, >, ==, <=, >=, and != <br>

    Format: <br>
        (Expression) (Conditional operator) (Expression) <br>

    Uses: <br>
        Assign to a variable: <br>
            bool x; <br>
            x = Conditional Expression; <br>

        If Statement: <br>
            if (Conditional Expression) {} <br> 
            else if (Conditional Expression) {} <br>

While Loops: <br>

    Format: <br>
        While ((Conditional Expression Expansion)) {(statements)} <br>
    Or: <br>
        While ((Expression)) {(statements)} <br>
    where the expression results in a boolean value <br>

    Uses: <br>
        Use anywhere statements are allowed <br>
            - Inside functions after variable declaration <br>
            - Inside if statements <br>
            - Inside other while loops <br>