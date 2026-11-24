#ifndef POLYNOM_H
#define POLYNOM_H

#define YYSTYPE int64_t
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <math.h>
#include <locale.h>
#include <limits.h>
#include "error_constants.h"
extern YYSTYPE yylineno;

// Структура для представления одночлена
typedef struct {
    int coefficient;
    int exponent;
    int variable;
} Term;

// Структура для представления полинома
typedef struct Polynomial{
    Term* terms;
    int num_terms;
    int letter;
} Polynomial;


#define ASSIGN_MODE 0 // Режим для assignPolynomial
#define GET_MODE 1    // Режим для getPolynomial
#define MAX_POLY_VARS 26 
extern Polynomial* poly_var_map[26]; 

Polynomial *createPolynomial(const int coefficient, const int exponent, const int variable);
Polynomial * addPolynomials(const Polynomial* poly1, const Polynomial* poly2);
Polynomial* subtractPolynomials(const Polynomial* poly1, const Polynomial* poly2);
Polynomial* multiplyPolynomials(const Polynomial* poly1, const Polynomial* poly2);
Polynomial* powerPolynomial(const Polynomial* poly, const int power);
Polynomial* polyHandler(char variable, Polynomial* poly, int mode);

void simplifyPolynomial(Polynomial* poly);
void sortTerms(Polynomial* poly);
void destroyPolynomial(Polynomial* poly);
void printPolynomial(Polynomial *poly, int variable);
void checkVariableEquality(const Polynomial* poly1, const Polynomial* poly2);
int getFirstCoefficient(const Polynomial* poly);

#endif