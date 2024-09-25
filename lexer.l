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

"print" { 
    words++; chars += strlen(yytext);
    printf("%-10s | %-10s\n", yytext, "PRINT_KEYWORD");
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

"+" { return ADD; }
"-" { return SUB; }
"*" { return MUL; }
"/" { return DIV; }

";" { 
    chars++;
    printf("%-10s | %-10s\n", yytext, "SEMICOLON");
    return SEMICOLON;
}

"=" { 
    chars++;
    printf("%-10s | %-10s\n", yytext, "EQ");
    return EQ;
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
    return '('; 
}
")" { 
    chars++;
    printf("%-10s | %-10s\n", yytext, "RPAREN");
    return ')'; 
}
{BAD_ID} {
    words++; chars += strlen(yytext);
    printf("|ERROR| %s : Bad Identifier at line %d char %d\n", yytext, lines, chars);
    // You might want to return an ERROR token or handle this differently
}

\n { lines++; chars = 0; }
[ \t] { chars++; }

. {
    chars++;
    printf("|ERROR| %s : Unrecognized symbol at line %d char %d\n", yytext, lines, chars);
    // You might want to return an ERROR token or handle this differently
}

%%