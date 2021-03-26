#if !defined(__FUZZ_LOG_HEADER__)
#define __FUZZ_LOG_HEADER__

#ifndef NDEBUG
#define DEBUG_LOG(X)                                                           \
    do                                                                         \
    {                                                                          \
        std::cout << X;                                                        \
    } while (false)
#else
#define DEBUG_LOG(X)                                                           \
    do                                                                         \
    {                                                                          \
    } while (false)
#endif

#ifndef NDEBUG
#define IS_DEBUG true
#else
#define IS_DEBUG false
#endif

#endif // __FUZZ_LOG_HEADER__