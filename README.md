# my-compiler

Videos:
Grant Burk: https://www.loom.com/share/7fe7cc1c159b4cf0b7c498503c9edb0a?sid=dd5dea37-54e7-457b-bd34-e5b23c23c118 

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
