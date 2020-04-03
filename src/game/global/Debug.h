#ifndef DEBUG_H
#define DEBUG_H
#include <stdio.h>

//! Prints if condition is false
#define DBG_CHECK(cond)                                                 \
    if (!(cond)) {                                                      \
        printf("Check failed on line %d in %s.\n", __LINE__, __FILE__); \
    }

#ifndef NDEBUG //cmake generated
#define DEBUG
#define DBG_LOG(fmt, ...)                    \
    do {                                     \
        fprintf(stderr, "[LS] ");            \
        fprintf(stderr, fmt, ##__VA_ARGS__); \
    } while (0)

#else
#define DBG_LOG(a, ...)
#endif

#endif
