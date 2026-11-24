#include <limits.h>


#define PRINT_ENABLED 0

#define _PRINT_(RULE_NAME, TYPE) \
        if (PRINT_ENABLED) { \
            printf("Rule: %s -> %s  (line %d)\n", RULE_NAME, TYPE, __LINE__); \
            if (__LINE__ == 28 || __LINE__ == 35) { \
                printf("!!!END STR!!!\n\n"); \
            } \
        } \

#define HANDLE_ERROR(message, ...) \
        fprintf(stderr, "Error in %s:%d: ",  __FILE__, __LINE__); \
        fprintf(stderr, message, ##__VA_ARGS__); \
        fprintf(stderr, "\n"); exit(-1); 