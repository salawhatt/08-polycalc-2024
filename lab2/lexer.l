%{
#include <stdio.h>
#include <errno.h>
#include "parser.tab.h"
#include "error_constants.h"
int prev_val = 0;
%}

%option yylineno
%option noyywrap

%%

"//"(.)* ;
[a-z]   { yylval = *yytext; prev_val = 'v'; return VARIABLE; }
[A-Z]   { yylval = yytext[0]; prev_val = 'v'; return POLY_VARIABLE; }
[0-9]+ {
    errno = 0;
    char *endptr;
    long long_val = strtol(yytext, &endptr, 10);
    if ((errno == ERANGE && (long_val == LONG_MAX || long_val == LONG_MIN)) || (long_val > INT_MAX || long_val < INT_MIN)) {
        HANDLE_ERROR("Integer overflow occurred");
    }
    else {
        yylval = (int)long_val;
    }
    return DIGIT;
}
[\t\r\n\r ] ; 

"+"     {return OP_ADD;}
"*"     {return OP_MUL;}
"^"     {return OP_POW;}
";"     {return EOC; }
")"     {return OP_RPAREN;}
"("     {prev_val= yytext[0]; return OP_LPAREN;}
"<<"    {prev_val= yytext[0]; return PRINT;}
"="     {prev_val= yytext[0]; return OP_ASSIGN;}
"-"     {
            int next_char = input();  
            unput(next_char); 
            if ((prev_val == '<' || prev_val == '=' || prev_val == '(') && 
                ((next_char >= 'a' && next_char <= 'z') || 
                 (next_char >= 'A' && next_char <= 'Z') || 
                 next_char == '(')) {
                prev_val = 0; 
                return OP_UMINUS;
            } else {
                prev_val = 0; 
                return OP_MINUS;
            }
        }
.       {HANDLE_ERROR("Unknown character encountered: '%c', at line %d", *yytext, yylineno);}



%%
