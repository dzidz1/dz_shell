#include "my_helpers.h"
#include "common.h"

int my_strcmp(const char *str1, const char *str2) {
  if (!str1 || !str2)
    return -1;
  // iterate while *str1 exists and equals to *str2
  while (*str1 && (*str1 == *str2)) {
    str1++;
    str2++;
  }

  // == 0 when equals
  // < 0 when str1 < str2
  // > 0 when str1 > str2
  return (unsigned char)*str1 - (unsigned char)*str2;
}

int my_strlen(const char *str) {
  if (!str)
    return -1;

  int len = 0;
  while (*str) {
    len++;
    str++;
  }

  return len;
}

int my_strncmp(const char *str1, const char *str2, size_t n) {
  if (!str1 || !str2)
    return -1;

  size_t i = 0;
  while (i < n && *str1 && *str2) {
    if (*str1 != *str2) {
      return (unsigned char)*str1 - (unsigned char)*str2;
    }
    str1++;
    str2++;
    i++;
  }

  if (i == n)
    return 0;

  return (unsigned char)*str1 - (unsigned char)*str2;
}

char *my_getenv(char *variable_name, char **env) {
  if (!variable_name || !env)
    return NULL;

  size_t variable_name_length = my_strlen(variable_name);

  for (size_t i = 0; env[i] != NULL; i++) {
    // starts with variable_name
    if (my_strncmp(variable_name, env[i], variable_name_length) == 0 &&
        env[i][variable_name_length] == '=') {
      return &env[i][variable_name_length + 1];
    }
  }

  return NULL;
}

char *my_strcpy(char *dest, const char *src) {
  if (!src || !dest)
    return NULL;
  char *return_pointer = dest;
  while (*src) {
    *dest = *src;
    dest++;
    src++;
  }
  *dest = '\0';
  return return_pointer;
}

char *my_strncpy(char *dest, const char *src, size_t n) {
  if (!src || !dest)
    return NULL;
  char *return_pointer = dest;
  while (n > 0 && *src) {
    *dest = *src;
    dest++;
    src++;
    n--;
  }

  // padding with spaces
  while (n > 0) {
    *dest = '\0';
    dest++;
    n--;
  }

  return return_pointer;
}

char *my_strdup(const char *str) {
  if (!str)
    return NULL;
  size_t len = my_strlen(str);
  char *copy = malloc((len + 1) * sizeof(char));
  if (!copy)
    return NULL;
  return my_strcpy(copy, str);
}

char *my_strchr(const char *str, char c) {
  if (!str)
    return NULL;
  while (*str) {
    // first occurance
    if (*str == c)
      return (char *)str;
    str++;
  }
  return NULL;
}

char *my_strtok(char *input, const char *delim) {
  static char *next = NULL;
  if (input == NULL)
    input = next;
  if (input == NULL)
    return NULL;

  // removing leading delimiters
  while (*input && my_strchr(delim, *input))
    input++;

  // cheking if we reached the end
  if (*input == '\0') {
    next = NULL;
    return NULL;
  }

  char *token = input; // start of the token

  while (*input && !my_strchr(delim, *input))
    input++;

  // terminate and update
  if (*input) {
    *input = '\0';
    next = input + 1;
  } else {
    next = NULL;
  }

  return token;
}
