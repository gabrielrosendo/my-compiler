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
While Loops:

Format:
    While ((Conditional Expression Expansion)) { (Code Block) }
    or:
    While ((Expression)) { (Code Block) }
    - The condition must result in a boolean value.

Uses:
    - Inside functions after variable declarations:
        while (x < 10) { Code Block }
    - Inside if statements:
        if (x > 5) { while (y < 10) { Code Block } }
    - Nested inside other while loops:
        while (x < 10) { while (y > 5) { Code Block } }

** Can Use Logical operators: 
    - ex: while (x && y) { Code Block }
         or while (x || y) { Code Block }

Conditional Expression Expansion:

    Use Operators: ||, &&
    
    Format:
        (Conditional Expression) (Conditional Expansion Operator) (Conditional Expression)
        also: (Conditional Expression Expansion) (Conditional Expansion Operator) (Conditional Expression)

    Uses: 
        Assign to a varible:
            bool x;
            x = Conditional Expression Expansion;

        If Statement:
            if (Conditional Expression Expansion) {} 
            else if (Conditional Expression Expansion) {}

If Statements: 

    Starts with if:
        if (Conditional Statement) { Statements }

    Can be expanded:
        if (Conditional Statement) {
        } else if (Conditional Statement) { Statements }

    Also:
        if (Conditional Statement) {
        } else { Statements }

    So, start with if, then there can be as many else if statements after the initial if. Else statements only come at the end of the if statement.

    Conditional Statements are expression and conditional expressions that have boolean values
        - basic vales (true, false)
        - variable with bool type
        - fuctions with bool type
        - Conditional Expression
        - Conditional Expression Expansions
    


Compiler rules:

Function decl:
    type ID(parameters: type ID...) {body + return}
    - Arrays cannote be returned or be parameters

Parameter Decl:
    type ID;
    - Must occur before statements

Array decl:
    type ID[size];
    - Must occur before statements

Statements:
    Variable assignment:
        ID = expression;
        - ID has to be declared as a variable
        - epression has to be the same type but float and int are interchanable
    
    Array assignment:
        ID[index] = expression;
        - ID has to be declared as an array;
        - epression has to be the same type but float and int are interchanable
    
    Print:
        print(expression);

Expressions:
    Use operations: +, -, *, /

    Numbers:
        1, 2, 3
    
    Floats:
        1.2, 2.4, 5.3

    Characters:
        'a', 'b', 'C'
        - Doesn't work with operations
    
    Boolean:
        true, false
        - Doesn't work with operations
    
    Function call:
        functionID(expression, expression...);

    Var reference:
        ID
        - ID has to be a delcared variable

    Array reference:
        ID[index]
        - ID has to be a delcared array
        - index has to be 0 <= index < size

Conditional Expressions:

    Use operators: <, >, ==, <=, >=, and !=

    Format:
        (Expression) (Conditional operator) (Expression)

    Uses: 
        Assign to a varible:
            bool x;
            x = Conditional Expression;

        If Statement:
            if (Conditional Expression) {} 
            else if (Conditional Expression) {}

While Loops:

    Format:
        While ((Conditional Expression Expansion)) {(statements)}
    Or:
        While ((Expression)) {(statements)}
    where the expression results in a boolean value

    Uses:
        Use anywhere statements are allowed
            - Inside functions after variable declaration
            - Inside if statments
            - Inside other while loops