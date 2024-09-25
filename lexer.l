%option noyywrap

%{
#include <stdio.h>
#include <string.h>
#include "parser.tab.h"

#define YY_DECL int yylex()

int words = 0;
int chars = 0;
int lines = 0;

%}

letter      [a-zA-Z]
digit       [0-9]
ID          {letter}({letter}|{digit})*
delim       [ \t\n]
NUMBER         {digit}+(\.{digit}+)?(E[+\-]?(digit)+)?
ws          {delim}+
operator    [+\-*/%]
paren       [()]
symbol      [!#$^&"'<>]
BAD_ID		{NUMBER}{ID}

%%
"/*"    				{
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
\"(\\\"|[^\"\n])*\" { words++; chars += strlen(yytext); printf("%-10s | %-10s\n", yytext, "STRING"); }

					
"int"|"float"|"char"  { words++; chars += strlen(yytext);
                        printf("%-10s | %-10s\n", yytext, "TYPE");
                      }

"print" { words++; chars += strlen(yytext);
    printf("%-10s | %-10s\n", yytext, "PRINT_KEYWORD");
}

"if" { words++; chars += strlen(yytext);
    printf("%-10s | %-10s\n", yytext, "IF_KEYWORD");
}

"else" { words++; chars += strlen(yytext);
    printf("%-10s | %-10s\n", yytext, "ELSE_KEYWORD");
}

"while" { words++; chars += strlen(yytext);
    printf("%-10s | %-10s\n", yytext, "WHILE_KEYWORD");
}

"return" { words++; chars += strlen(yytext);
    printf("%-10s | %-10s\n", yytext, "RETURN_KEYWORD");
}
        
{ID}	{words++; chars += strlen(yytext);
         printf("%-10s | %-10s\n", yytext, "IDENTIFIER");
        }
            
{NUMBER}	{words++; chars += strlen(yytext);
              printf("%-10s | %-10s\n", yytext, "NUMBER");
            }

{operator}  {words++; chars += strlen(yytext);
              printf("%-10s | %-10s\n", yytext, "OPERATOR");
            }	
{symbol}  {words++; chars += strlen(yytext);
              printf("%-10s | %-10s\n", yytext, "SYMBOL");
            }

{BAD_ID}	{words++; chars += strlen(yytext);
               printf("|ERROR| %s : Bad Identifier at line %d char %d\n", yytext,lines,chars);
            }

{paren}  {words++; chars += strlen(yytext);
              printf("%-10s | %-10s\n", yytext, "PARENTHESIS");
            }

";"		{chars++;
              printf("%-10s | %-10s\n", yytext, "SEMICOLON");
		}
		
"="		{chars++;
              printf("%-10s | %-10s\n", yytext, "EQ");
		}
"{"       {chars++;
			  printf("%-10s | %-10s\n", yytext, "LBRACE");
		}
"}"       {chars++;
			  printf("%-10s | %-10s\n", yytext, "RBRACE");
		}
		

\n		{lines++; chars=0;}
[ \t]	{chars++;}
.		{chars++;
         printf("|ERROR| %s : Unrecognized symbol at line %d char %d\n", yytext,lines,chars);
		}
		


%%