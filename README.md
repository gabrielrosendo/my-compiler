# my-compiler
For part 3, we added support for if, else if, and else constructs with both ConditionalExpr and general expressions (Expr). Logical Operators were also implemented. See below for more information about improvements.

Videos:
Grant Burk: https://www.loom.com/share/1937df6e04a94d91a454d45fda6c4cca?sid=ed101db7-8458-4084-bd73-fbb27b1b548e
Gabriel Marcelino: 
pt 1: https://www.loom.com/share/e1cc89d0cefe418b9d4a24df5604ece4
pt 2(ran out of time for last test): https://www.loom.com/share/74d489c7b80f499abe341840b3ef7467 

## Improvements Made (Part 3)
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

