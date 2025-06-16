#include "util.h"

uint32_t strtol(const char* str, const char** out_rest, int base) {
    uint32_t result = 0;
    while (*str >= '0' && *str <= '9') {
        result = result * base + (*str - '0');
        str++;
    }
    if (out_rest) *out_rest = str;
    return result;
}

void* memset(void* dest, int value, size_t len) {
    uint8_t* ptr = (uint8_t*)dest;
    while (len-- > 0) {
        *ptr++ = (uint8_t)value;
    }
    return dest;
}
