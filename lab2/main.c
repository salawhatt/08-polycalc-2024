#include "polycalc.h"
#include "parser.tab.h"
#include <stdio.h>


extern FILE *yyin;

int main(int argc, char *argv[]) {
    const char *filename = "input.txt"; // значение по умолчанию
    if (argc > 1) {
        filename = argv[1]; // если название файла введено, используем его
    }
    
    FILE *inpfile = fopen(filename, "r");
    if (!inpfile) {
        fprintf(stderr, "Failed to open input file '%s'\n", filename);
        return -1;
    }
    yyin = inpfile;
    yyparse();
    fclose(inpfile);
    return 0;
}
