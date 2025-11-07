#include "dz_shell.h"

int my_strcmp(const char* str1, const char* str2)
{
    // iterate while *str1 exists and equals to *str2
    while(*str1 && (*str1 == *str2)) {
        str1++;
        str2++;
    }

    // == 0 when equals
    // < 0 when str1 < str2
    // > 0 when str1 > str2
    return (unsigned char)*str1 - (unsigned char)*str2;
}