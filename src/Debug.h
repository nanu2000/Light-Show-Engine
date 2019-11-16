#ifndef NDEBUG //cmake generated
#define DEBUG
#define DBG_LOG(fmt, ...) fprintf(stderr, fmt, ##__VA_ARGS__)

#else
#define DBG_LOG(a, ...)
#endif
