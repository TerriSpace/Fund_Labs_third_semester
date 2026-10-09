#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <float.h>

typedef enum {
    STATUS_OK = 0,
    ERR_ARGC,
    ERR_INVALID_INPUT,
    ERR_OVERFLOW,
    ERR_DIV_BY_ZERO,

    ERR_RESULT,
    ERR_X,
    ERR_EPS


} Status;