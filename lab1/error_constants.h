#include <limits.h>


#define PRINT_ENABLED 0

#define _PRINT_(...) \
        if (PRINT_ENABLED) { \
            printf(__VA_ARGS__); \
        } \

#define HANDLE_ERROR(message, ...) \
        fprintf(stderr, "Error in %s:%d: ",  __FILE__, __LINE__); \
        fprintf(stderr, message, ##__VA_ARGS__); \
        fprintf(stderr, "\n"); exit(-1); 