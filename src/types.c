#include "types.h"

u32 pow(const u32 base, const u32 power) {
    if (!power)
        return 1;
    else if (power == 1)
        return base;

    u32 result = base;

    for (u32 i = 0; i < power - 1; i++) {
        result *= base;
    }

    return result;
}