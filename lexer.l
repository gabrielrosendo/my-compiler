%option noyywrap
%{
#include <stdio.h>
#include <string.h>
#include "parser.tab.h"

int words = 0;
int chars = 0;
int lines = 0;
%}

letter [a-zA-Z]
digit [0-9]
ID {letter}({letter}|{digit})*
delim [ \t\n]
NUMBER {digit}+(\.{digit}+)?(E[+\-]?(digit)+)?
ws {delim}+
BAD_ID {NUMBER}{ID}

%option yylineno
%%

"/*" {
    int c;
    while((c = input()) != 0) {
        if(c == '*') {
            if((c = input()) == '/')
                break;
            else
                unput(c);
        }
    }
}

\"(\\\"|[^\"\n])*\" { 
    words++; chars += strlen(yytext);
    printf("%-10s | %-10s\n", yytext, "STRING");
    // You might want to add a STRING token if you need it in your grammar
}

"int"|"float"|"char" { 
    words++; chars += strlen(yytext);
    printf("%-10s | %-10s\n", yytext, "TYPE");
    yylval.string = strdup(yytext);
    return TYPE;
}
"void"      { 
    words++; chars += strlen(yytext);
    printf("%-10s | %-10s\n", yytext, "VOID");
    return VOID; }

"main"      { 
    words++; chars += strlen(yytext);
    printf("%-10s | %-10s\n", yytext, "MAIN");
    return MAIN; }

"return"    { 
    words++; chars += strlen(yytext);
    printf("%-10s | %-10s\n", yytext, "RETURN");
    return RETURN; }

"print" { 
    words++; chars += strlen(yytext);
    printf("%-10s | %-10s\n", yytext, "PRINT");
    return PRINT;
}

"write" { 
    words++; chars += strlen(yytext);
    printf("%-10s | %-10s\n", yytext, "PRINT");
    return PRINT;
}

{ID} {
    words++; chars += strlen(yytext);
    printf("%-10s | %-10s\n", yytext, "IDENTIFIER");
    yylval.string = strdup(yytext);
    return ID;
}

{NUMBER} {
    words++; chars += strlen(yytext);
    printf("%-10s | %-10s\n", yytext, "NUMBER");
    yylval.number = atoi(yytext);
    return NUMBER;
}
"["         { 
    words++; chars += strlen(yytext);
    printf("%-10s | %-10s\n", yytext, "LBRACKET");
    return LBRACKET; }

"]"         { 
    words++; chars += strlen(yytext);
    printf("%-10s | %-10s\n", yytext, "RBRACKET");
    return RBRACKET; }

"+" {
    words++; chars += strlen(yytext);
    printf("%-10s | %-10s\n", yytext, "ADD");
    yylval.string = strdup(yytext); 
    return ADD; }

"-" { 
    words++; chars += strlen(yytext);
    printf("%-10s | %-10s\n", yytext, "SUB");
    yylval.string = strdup(yytext); 
    return SUB; }
"*"         { 
    words++; chars += strlen(yytext);
    printf("%-10s | %-10s\n", yytext, "MUL");
    yylval.string = strdup(yytext); 
    return MUL; }

"/"         { 
    words++; chars += strlen(yytext);
    printf("%-10s | %-10s\n", yytext, "DIV");
    yylval.string = strdup(yytext); 
    return DIV; }

";" { 
    chars++;
    printf("%-10s | %-10s\n", yytext, "SEMICOLON");
    return SEMICOLON;
}

"=" { 
    chars++;
    printf("%-10s | %-10s\n", yytext, "EQ");
    yylval.string = strdup(yytext);
    return EQ;
}
"," { 
    chars++;
    printf("%-10s | %-10s\n", yytext, "COMMA");
    return COMMA;
}

"{" { 
    chars++;
    printf("%-10s | %-10s\n", yytext, "LBRACE");
    return LBRACE;
}

"}" { 
    chars++;
    printf("%-10s | %-10s\n", yytext, "RBRACE");
    return RBRACE;
}

"(" { 
    chars++;
    printf("%-10s | %-10s\n", yytext, "LPAREN");
    return LPAREN; 
}
")" { 
    chars++;
    printf("%-10s | %-10s\n", yytext, "RPAREN");
    return RPAREN; 
}

{BAD_ID} {
    words++; chars += strlen(yytext);
    printf("|ERROR| %s : Bad Identifier at line %d char %d\n", yytext, lines, chars);
    // You might want to return an ERROR token or handle this differently
}

\n { lines++; chars = 0;yylineno++;}
[ \t] { chars++; }

. {
    chars++;
    printf("|ERROR| %s : Unrecognized symbol at line %d char %d\n", yytext, lines, chars);
    // You might want to return an ERROR token or handle this differently
}

%%