#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "typedef.h"

int main(void) {

    srand((unsigned)time(NULL));

    // Generate two random numbers using /dev/urandom
    int8 a = rand() % ((INT8_MAX / 2) + 1);
    int8 b = rand() % ((INT8_MAX / 2) + 1);

    printf("Hello, World! Calculate: a(%" PRId8 ") + b(%" PRId8 ") = %" PRId8
           "\n",
           a, b, a + b);
    return 0;
}
