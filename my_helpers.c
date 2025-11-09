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

int my_strlen(const char* str) {
    int len = 0;
    while(*str) {
        len++;
        str++;
    }
    return len;
}

int my_strncmp(const char* str1, const char* str2, size_t n) {
    size_t i = 0;
    while(i < n && *str1 && *str2) {
        if(*str1 != *str2) {
            return (unsigned char)str1[i] - (unsigned char)str2[i];
        }
        str1++;
        str2++;
        i++;
    }

    if(i == n) return 0;
    
    return (unsigned char)str1[i] - (unsigned char)str2[i];
}

char* my_getenv(char* variable_name, char** env) {
    if(variable_name == NULL || env == NULL) {
        return NULL; 
    }

    size_t variable_name_length = my_strlen(variable_name);

    for(size_t i = 0; env[i] != NULL; i++) {
        // starts with variable_name
        if(my_strncmp(variable_name, env[i], variable_name_length) == 0 && env[i][variable_name_length] == '=') {
            return &env[i][variable_name_length + 1];
        }
    }

    return NULL;

}