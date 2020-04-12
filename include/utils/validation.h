#ifndef TS_UTIL_VALIDATION_H
#define TS_UTIL_VALIDATION_H

#include "error.h"

#ifndef TS_DISABLE_ARGS_CHECKS
#    define CHECK_NULL_PARAMS_1(p1)                                            \
        if (!(p1)) {                                                           \
            return TS_ERR_NULL_ARGUMENTS;                                      \
        }
#    define CHECK_NULL_PARAMS_2(p1, p2)     CHECK_NULL_PARAMS_1(p1 &&p2)
#    define CHECK_NULL_PARAMS_3(p1, p2, p3) CHECK_NULL_PARAMS_1(p1 &&p2 &&p3)
#    define CHECK_NULL_PARAMS_4(p1, p2, p3, p4)                                \
        CHECK_NULL_PARAMS_1(p1 &&p2 &&p3 &&p4)
#    define CHECK_NULL_PARAMS_5(p1, p2, p3, p4, p5)                            \
        CHECK_NULL_PARAMS_1(p1 &&p2 &&p3 &&p4 &&p4)

#    define CHECK_UNINITIALIZED_DATA_1(d1)                                     \
        if (!(d1)) {                                                           \
            return TS_ERR_UNINITIALIZED_DATA;                                  \
        }
#    define CHECK_UNINITIALIZED_DATA_2(p1, p2)                                 \
        CHECK_UNINITIALIZED_DATA_1(p1 &&p2)
#    define CHECK_UNINITIALIZED_DATA_3(p1, p2, p3)                             \
        CHECK_UNINITIALIZED_DATA_1(p1 &&p2 &&p3)
#    define CHECK_UNINITIALIZED_DATA_4(p1, p2, p3, p4)                         \
        CHECK_UNINITIALIZED_DATA_1(p1 &&p2 &&p3 &&p4)
#    define CHECK_UNINITIALIZED_DATA_5(p1, p2, p3, p4, p5)                     \
        CHECK_UNINITIALIZED_DATA_1(p1 &&p2 &&p3 &&p4 &&p4)
#else
#    define CHECK_NULL_PARAMS_1(p1)                        ((void)0)
#    define CHECK_NULL_PARAMS_2(p1, p2)                    ((void)0)
#    define CHECK_NULL_PARAMS_3(p1, p2, p3)                ((void)0)
#    define CHECK_NULL_PARAMS_4(p1, p2, p3, p4)            ((void)0)
#    define CHECK_NULL_PARAMS_5(p1, p2, p3, p4, p5)        ((void)0)
#    define CHECK_UNINITIALIZED_DATA_1(p1)                 ((void)0)
#    define CHECK_UNINITIALIZED_DATA_2(p1, p2)             ((void)0)
#    define CHECK_UNINITIALIZED_DATA_3(p1, p2, p3)         ((void)0)
#    define CHECK_UNINITIALIZED_DATA_4(p1, p2, p3, p4)     ((void)0)
#    define CHECK_UNINITIALIZED_DATA_5(p1, p2, p3, p4, p5) ((void)0)
#endif

#endif
