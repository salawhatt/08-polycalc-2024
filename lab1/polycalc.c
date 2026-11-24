#include "polycalc.h"
int poly_count = 0;

bool checkAdditionOverflow(int a, int b) {
    if ((b > 0 && a > INT_MAX - b) || (b < 0 && a < INT_MIN - b)) {
        // Переполнение произошло
        return true;
    } else {
        // Переполнение не произошло
        return false;
    }
}

bool checkSubtractionOverflow(int a, int b) {
    if ((b > 0 && a < INT_MIN + b) || (b < 0 && a > INT_MAX + b)) {
        // Переполнение произошло
        return true;
    } else {
        // Переполнение не произошло
        return false;
    }
}

bool checkMultiplicationOverflow(int a, int b) {
    if (a != 0 && b != 0 && (a > INT_MAX / b || a < INT_MIN / b)) {
        // Переполнение произошло
        return true;
    } else {
        // Переполнение не произошло
        return false;
    }
}
void checkVariableEquality(const Polynomial* poly1, const Polynomial* poly2) {
    if (poly1 == NULL || poly2 == NULL) {
        HANDLE_ERROR("One or both polynomials are NULL");
    }
    for (int i = 0; i < poly1->num_terms; ++i) {
        char var1 = poly1->terms[i].variable;
        for (int j = 0; j < poly2->num_terms; ++j) {
            char var2 = poly2->terms[j].variable;
            if (var1 == 0 && var2 == 0) {
                continue;
            }
            if (var1 != var2 && (var1 != 0 && var2 != 0)) {
                HANDLE_ERROR("Operations between polynomials with different coefficients are not supported, at line %ld", yylineno); 
            }
        }
    }
}

Polynomial* polyHandler(char variable, Polynomial* poly, int mode) 
{
    if (mode == 0) {
        poly->letter = (int)variable;
        poly_var_map[variable - 'A'] = poly;
        return NULL;
    } else if (mode == 1) {
        if (poly_var_map[variable - 'A'] == NULL) {
            HANDLE_ERROR("No polynomial assigned to variable '%c', at line %ld", variable, yylineno);  
            return NULL;
        }
        return poly_var_map[variable - 'A'];
    }
}

void destroyPolynomial(Polynomial* poly) {
    free(poly->terms);
    free(poly);
}

Polynomial *createPolynomial(const int coefficient, const int exponent, const int variable)
{
    Polynomial* poly = (Polynomial*)malloc(sizeof(Polynomial));
    poly->num_terms = 1;
    poly->letter = 0;
    poly->terms = (Term*)malloc(1 * sizeof(Term));
    poly->terms->coefficient = coefficient;
    poly->terms->exponent = exponent;
    poly->terms->variable = variable;
    return poly;
}
// Функция для вывода полинома на экран
void printPolynomial(Polynomial *poly, int variable) {
    int firstTerm = 1;
    if (!poly->letter)
    {
        printf("Line %ld result = ",yylineno );
    }
    else
    {
        printf("Line %ld variable '%c': ", yylineno, (char)poly->letter);
    }
    
    for (int i = 0; i < poly->num_terms; ++i) {
        int coeff = poly->terms[i].coefficient;
        int exp = poly->terms[i].exponent;
        char var = poly->terms[i].variable;
        if (coeff != 0) {
            char *sign = "";
            if (!firstTerm) { // Если это не первый элемент
                sign = (coeff > 0) ? "+" : "-";
            } else { // Если это первый элемент
                sign = (coeff < 0) ? "-" : "";
                firstTerm = 0; // Помечаем, что первый элемент уже выведен
            }
            if (exp == 0) {
                printf("%s%d", sign, abs(coeff));
            } else if (exp == 1) {
                if (abs(coeff) == 1) {
                    printf("%s%c", sign, var);
                } else {
                    printf("%s%d%c", sign, abs(coeff), var);
                }
            } else {
                if (abs(coeff) == 1) {
                    printf("%s%c^%d", sign, var,exp);
                } else {
                    printf("%s%d%c^%d", sign, abs(coeff), var, exp);
                }
            }
        }
    }
    if (firstTerm) {
        printf("0"); // Если полином пустой
    }
    printf("\n");
}


Polynomial * addPolynomials(const Polynomial* poly1,  const Polynomial* poly2){
    checkVariableEquality(poly1, poly2);
    if (poly1 == NULL || poly2 == NULL) {
        HANDLE_ERROR("One or both polynomials are NULL");
    }
    Polynomial* result = (Polynomial*)malloc(sizeof(Polynomial));
    if (result == NULL) {
        HANDLE_ERROR("Memory allocation failed");
    }
    result->num_terms = poly1->num_terms + poly2->num_terms;
    result->terms = (Term*)malloc(result->num_terms * sizeof(Term));
    if (result->terms == NULL) {
        HANDLE_ERROR("Memory allocation failed");
    }
    int i = 0, j = 0, index = 0;

    while (i < poly1->num_terms && j < poly2->num_terms) {
        if (poly1->terms[i].exponent == poly2->terms[j].exponent) {
            if (checkAdditionOverflow(poly1->terms[i].coefficient, poly2->terms[j].coefficient)) {
                HANDLE_ERROR("Integer overflow occurred during addition");
            }
            result->terms[index].coefficient = poly1->terms[i].coefficient + poly2->terms[j].coefficient;
            result->terms[index].exponent = poly1->terms[i].exponent;
            result->terms[index].variable = poly1->terms[i].variable;
            i++;
            j++;
        }
        else if (poly1->terms[i].exponent > poly2->terms[j].exponent) {
            result->terms[index] = poly1->terms[i];
            i++;
        }
        else {
            result->terms[index] = poly2->terms[j];
            j++;
        }
        index++;
    }
    while (i < poly1->num_terms) {
        result->terms[index] = poly1->terms[i];
        i++;
        index++;
    }
    while (j < poly2->num_terms) {
        result->terms[index] = poly2->terms[j];
        j++;
        index++;
    }
    simplifyPolynomial(result);
    return result;
}



// Функция для вычитания одного полинома из другого
Polynomial* subtractPolynomials(const Polynomial* poly1, const Polynomial* poly2) {
    checkVariableEquality(poly1, poly2);
    if (poly1 == NULL || poly2 == NULL) {
        HANDLE_ERROR("One or both polynomials are NULL");
    }
    Polynomial* result = (Polynomial*)malloc(sizeof(Polynomial));
    if (result == NULL) {
        HANDLE_ERROR("Memory allocation failed");
    }
    result->num_terms = poly1->num_terms + poly2->num_terms;
    result->terms = (Term*)malloc(result->num_terms * sizeof(Term));
    if (result->terms == NULL) {
        HANDLE_ERROR("Memory allocation failed");
    }
    int i = 0, j = 0, k = 0;
    while (i < poly1->num_terms && j < poly2->num_terms) {
        if (poly1->terms[i].exponent == poly2->terms[j].exponent) {
            if (checkSubtractionOverflow(poly1->terms[i].coefficient, poly2->terms[j].coefficient)) {
                HANDLE_ERROR("Integer overflow occurred during subtraction");
            }
            result->terms[k].coefficient = poly1->terms[i].coefficient - poly2->terms[j].coefficient;
            result->terms[k].exponent = poly1->terms[i].exponent;
            result->terms[k].variable = poly1->terms[i].variable;
            i++;
            j++;
        }
        else if (poly1->terms[i].exponent > poly2->terms[j].exponent) {
            result->terms[k] = poly1->terms[i];
            i++;
        }
        else {
            if (checkSubtractionOverflow(0, poly2->terms[j].coefficient)) {
                HANDLE_ERROR("Integer overflow occurred during subtraction");
            }
            result->terms[k].coefficient = -poly2->terms[j].coefficient;
            result->terms[k].exponent = poly2->terms[j].exponent;
            result->terms[k].variable = poly2->terms[j].variable;
            j++;
        }
        k++;
    }
    while (i < poly1->num_terms) {
        result->terms[k] = poly1->terms[i];
        i++;
        k++;
    }
    while (j < poly2->num_terms) {
        if (checkSubtractionOverflow(0, poly2->terms[j].coefficient)) {
                HANDLE_ERROR("Integer overflow occurred during subtraction");
            }
        result->terms[k].coefficient = -poly2->terms[j].coefficient;
        result->terms[k].exponent = poly2->terms[j].exponent;
        result->terms[k].variable = poly2->terms[j].variable;
        j++;
        k++;
    }
    simplifyPolynomial(result);
    return result;
}

// Функция для умножения двух полиномов
Polynomial* multiplyPolynomials(const Polynomial* poly1, const Polynomial* poly2) {
    checkVariableEquality(poly1, poly2);
    if (poly1 == NULL || poly2 == NULL) {
        HANDLE_ERROR("One or both polynomials are NULL");
    }
    Polynomial* result = (Polynomial*)malloc(sizeof(Polynomial));
    if (result == NULL) {
        HANDLE_ERROR("Memory allocation failed");
    }
    result->num_terms = poly1->num_terms * poly2->num_terms;
    result->terms = (Term*)malloc(result->num_terms * sizeof(Term));
    if (result->terms == NULL) {
        HANDLE_ERROR("Memory allocation failed");
    }
    int index = 0;
    for (int i = 0; i < poly1->num_terms; ++i) {
        for (int j = 0; j < poly2->num_terms; ++j) {
            if (checkMultiplicationOverflow(poly1->terms[i].coefficient, poly2->terms[j].coefficient) ||
                checkMultiplicationOverflow(poly1->terms[i].exponent, poly2->terms[j].exponent)) {
                //HANDLE_ERROR("Integer overflow occurred during multiplication");
            }
            result->terms[index].coefficient = poly1->terms[i].coefficient * poly2->terms[j].coefficient;
            result->terms[index].exponent = poly1->terms[i].exponent + poly2->terms[j].exponent;
            if (poly1->terms[i].variable)
                result->terms[index].variable = poly1->terms[i].variable;
            else
                result->terms[index].variable = poly2->terms[j].variable;
            index++;
        }
    }
    simplifyPolynomial(result);
    return result;
}

void sortTerms(Polynomial* poly) {
    for (int i = 0; i < poly->num_terms - 1; ++i) {
        for (int j = 0; j < poly->num_terms - i - 1; ++j) {
            if (poly->terms[j].exponent < poly->terms[j + 1].exponent) {
                Term temp = poly->terms[j];
                poly->terms[j] = poly->terms[j + 1];
                poly->terms[j + 1] = temp;
            }
        }
    }
}

void simplifyPolynomial(Polynomial* poly) {
    if (poly == NULL) {
        HANDLE_ERROR("Polynomial is NULL");
    }
    sortTerms(poly);
    for (int i = 0; i < poly->num_terms - 1; ++i) {
        for (int j = i + 1; j < poly->num_terms; ++j) {
            if (poly->terms[i].exponent == poly->terms[j].exponent) {
                poly->terms[i].coefficient += poly->terms[j].coefficient;
                for (int k = j; k < poly->num_terms - 1; ++k) {
                    poly->terms[k] = poly->terms[k + 1];
                }
                poly->num_terms--; 
                j--; 
            }
        }
    }
}

Polynomial* powerPolynomial(const Polynomial* poly, const int power) {
    //printf("!\ndadda=%ld\n", yylineno);
    if (poly == NULL) {
        HANDLE_ERROR("Polynomial is NULL");
    }
    Polynomial* result = (Polynomial*)malloc(sizeof(Polynomial));
    if (result == NULL) {
        HANDLE_ERROR("Memory allocation failed");
    }
    result->num_terms = 1;
    result->terms = (Term*)malloc(result->num_terms * sizeof(Term));
    if (result->terms == NULL) {
        HANDLE_ERROR("Memory allocation failed");
    }
    result->terms[0].coefficient = 1;
    result->terms[0].exponent = 0;
    result->terms[0].variable = poly->terms[0].variable;
    for (int i = 0; i < power; ++i) {
        Polynomial* temp = multiplyPolynomials(result, poly);
        if (temp == NULL) {
            HANDLE_ERROR("Multiplication failed");
        }
        result = temp;
    }
    simplifyPolynomial(result);
    return result;
}
int getFirstCoefficient(const Polynomial* poly) {
    if (poly == NULL || poly->num_terms == 0) {
        HANDLE_ERROR("Polynomial is NULL");
    }
    if (poly->num_terms == 1 && poly->terms[0].exponent == 0 && poly->terms[0].coefficient >= 0) {
        return poly->terms[0].coefficient; 
    } else {
        HANDLE_ERROR("The operation of exponentiating a polynomial is not supported, at line %ld", yylineno);
        return 0;
    }
    return 0;
}
