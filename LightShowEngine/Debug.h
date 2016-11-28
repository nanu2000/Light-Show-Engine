#define DEBUG
#ifdef DEBUG
#define DBG_LOG(fmt, ...) fprintf(stderr, fmt, __VA_ARGS__)
#else
#define DBG_LOG
#endif

