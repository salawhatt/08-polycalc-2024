%{
    #include <stdio.h>
    #include <math.h>
    #include "polycalc.h"
    void yyerror(char *msg) 
    {
       HANDLE_ERROR("%s, at line %d", msg, yylineno);
    }
    char current_variable; 

%}

%left OP_ADD OP_MINUS OP_UMINUS
%left OP_MUL 
%right OP_POW 
%left OP_LPAREN
%right OP_RPAREN


%token DIGIT VARIABLE EOC PRINT
%token OP_ASSIGN POLY_VARIABLE
%%

line: save_pol | line expression | expression | line save_pol | YYEOF 

expression:     PRINT polynom EOC
                {
                  _PRINT_("Current line: %d\n\n", __LINE__);
                   printPolynomial((Polynomial*)$2, 0); 
                };

save_pol:       get_poly_var polynom EOC
                {
                   polyHandler($1, (Polynomial*)$2, ASSIGN_MODE);
                  _PRINT_("Current line: %d\n\n", __LINE__);
                }

get_poly_var:   POLY_VARIABLE OP_ASSIGN
                {
                  _PRINT_("Current line: %d\n", __LINE__);
                  $$=$1;
                };
polynom:        monomial
                {
                  _PRINT_("Current line: %d\n", __LINE__);
                  $$ = $1;
                }
		            | polynom OP_MINUS polynom
                {
                  _PRINT_("Current line: %d\n", __LINE__);
                  $$ = (int64_t)subtractPolynomials((Polynomial*)$1, (Polynomial*)$3);
                }
                | polynom OP_ADD polynom
                {
                  _PRINT_("Current line: %d\n", __LINE__);
                  $$ = (int64_t)addPolynomials((Polynomial*)$1, (Polynomial*)$3);
                }
                | polynom OP_POW monomial
                {
                  _PRINT_("Current line: %d\n", __LINE__); 
                  $$ = (int64_t)powerPolynomial((Polynomial*)$1, getFirstCoefficient((Polynomial*)$3));
                }
                | polynom OP_MUL monomial
                {
                  _PRINT_("Current line: %d\n", __LINE__);
                  $$ = (int64_t)multiplyPolynomials((Polynomial*)$1, (Polynomial*)$3);
                }
                | polynom monomial
                {
                  _PRINT_("Current line: %d\n", __LINE__);
                  $$ = (int64_t)multiplyPolynomials((Polynomial*)$1, (Polynomial*)$2);
                }
                // PSEUDO-RULES
                | polynom OP_MINUS OP_MINUS polynom
                {
                  HANDLE_ERROR ("Two operation signs in a row");
                }
                | polynom OP_ADD OP_ADD polynom
                {
                  HANDLE_ERROR ("Two operation signs in a row");
                }
                | polynom OP_POW OP_POW monomial
                {
                  HANDLE_ERROR ("Two operation signs in a row");
                }
                | polynom OP_MUL OP_MUL monomial
                {
                  HANDLE_ERROR ("Two operation signs in a row");
                }
                | polynom OP_MINUS OP_ADD polynom
                {
                  HANDLE_ERROR ("Two operation signs in a row"); 
                }
                | polynom OP_ADD OP_MINUS polynom
                {
                  HANDLE_ERROR ("Two operation signs in a row"); 
                }
                
monomial:       var_in_power
                {
                  _PRINT_("Current line: %d\n", __LINE__);
                  $$ = $1;
                }
                | OP_LPAREN polynom OP_RPAREN
                {
                  $$ = $2;
                  _PRINT_("Current line: %d\n", __LINE__);
                }
                | monomial OP_POW monomial
                {
                   $$ = (int64_t)powerPolynomial((Polynomial*)$1, getFirstCoefficient((Polynomial*)$3));
                }
                |
                POLY_VARIABLE
                {
                  _PRINT_("Current line: %d\n", __LINE__);
                  $$ = (int64_t)polyHandler((char)$1, NULL, GET_MODE);
                }
                | 
                OP_UMINUS monomial
                {
                  $$ = (int64_t)multiplyPolynomials((Polynomial*)createPolynomial(-1, 0, 0), (Polynomial*)$2);
                }
              

var_in_power:   var_coefficient 
                {
                  _PRINT_("Current line: %d\n", __LINE__);
                  $$ = (int64_t)createPolynomial($1, 1,  current_variable);
                }
                |
                num_in_power
                {
                  _PRINT_("Current line: %d\n", __LINE__);
                  $$ = (int64_t)createPolynomial((int)$1, 0, 0);
                }
              /*| var_coefficient OP_POW num_in_power
                {
                  _PRINT_("Current line: %d\n", __LINE__);
                  $$ = (int64_t)createPolynomial($1, $3, current_variable);
                }  */
                | var_coefficient OP_POW monomial
                {
                  _PRINT_("Current line: %d\n", __LINE__);
                  $$ = (int64_t)createPolynomial($1, getFirstCoefficient((Polynomial*)$3), current_variable);
                }
                
var_coefficient:  num_in_power VARIABLE
                {
                  _PRINT_("Current line: %d\n", __LINE__);
                  current_variable = $2; 
                  $$ = $1;
                }
                | VARIABLE
                {
                  _PRINT_("Current line: %d\n", __LINE__);
                  current_variable = $1; 
                  $$ = 1;
                };
                
num_in_power:   number
                {
                  _PRINT_("Current line: %d\n", __LINE__);
                  $$ = $1;
                }
                | number OP_POW num_in_power  
                {
                  _PRINT_("Current line: %d\n", __LINE__);
                  if ($1 == 0 && $3 ==0)
                  {
                    HANDLE_ERROR("An attempt to raise 0 to the zero degree");
                  }
                  $$ = pow($1, $3);
                }
                | number OP_POW POLY_VARIABLE 
                {
                  _PRINT_("Current line: %d\n", __LINE__); 
                  if ($1 == 0 && getFirstCoefficient(polyHandler((char)$3, NULL, GET_MODE)) ==0)
                  {
                    HANDLE_ERROR("An attempt to raise 0 to the zero degree");
                  }
                  $$ = pow($1, getFirstCoefficient(polyHandler((char)$3, NULL, GET_MODE)));
                }
                ;

number:         DIGIT   
                {
                  _PRINT_("Current line: %d\n", __LINE__);
                  $$ = $1;
                }
                | OP_MINUS DIGIT
                {
                  _PRINT_("Current line: %d\n", __LINE__);
                  $$ = $2 * (-1);
                }



%%
