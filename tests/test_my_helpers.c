#include "common.h"
#include "my_helpers.h"

int test_my_strlen() {
  int passed = 0;
  int total = 0;

  // Test 1: empty string
  total++;
  int result = my_strlen("");
  if (result != 0) {
    printf("Test 1 FAILED: empty string, expected 0, got %d\n", result);
  } else {
    printf("Test 1 passed: empty string\n");
    passed++;
  }

  // Test 2: normal string "gamarjoba"
  total++;
  result = my_strlen("gamarjoba");
  if (result != 9) {
    printf("Test 2 FAILED: 'gamarjoba', expected 9, got %d\n", result);
  } else {
    printf("Test 2 passed: normal string\n");
    passed++;
  }

  // Test 3: NULL pointer
  total++;
  result = my_strlen(NULL);
  if (result != -1) {
    printf("Test 3 FAILED: NULL, expected -1, got %d\n", result);
  } else {
    printf("Test 3 passed: NULL pointer\n");
    passed++;
  }

  // Test 4: single character
  total++;
  result = my_strlen("a");
  if (result != 1) {
    printf("Test 4 FAILED: 'a', expected 1, got %d\n", result);
  } else {
    printf("Test 4 passed: single character\n");
    passed++;
  }

  printf("\nmy_strlen: %d/%d tests passed\n", passed, total);
  return (passed == total) ? 1 : 0;
}

int test_my_strcmp() {
  int passed = 0;
  int total = 0;

  // Test 1: equal strings
  total++;
  int result = my_strcmp("hala madrid", "hala madrid");
  if (result != 0) {
    printf("Test 1 FAILED: equal strings, expected 0, got %d\n", result);
  } else {
    printf("Test 1 passed: equal strings\n");
    passed++;
  }

  // Test 2: different strings with same length
  total++;
  result = my_strcmp("abc", "adc");
  if (result >= 0) {
    printf("Test 2 FAILED: different strings with same length, expected "
           "negative number, got %d\n",
           result);
  } else {
    printf("Test 2 passed: different strings with same length\n");
    passed++;
  }

  // Test 3: different strings with different lengths
  total++;
  result = my_strcmp("z", "kkk");
  if (result <= 0) {
    printf("Test 3 FAILED: different strings with different lengths, expected "
           "positive number, got %d\n",
           result);
  } else {
    printf("Test 3 passed: different strings with different lengths\n");
    passed++;
  }

  // Test 4: NULL case
  total++;
  result = my_strcmp("string", NULL);
  if (result != -1) {
    printf("Test 4 FAILED: NULL case, expected -1, got %d\n", result);
  } else {
    printf("Test 4 passed: NULL case\n");
    passed++;
  }

  printf("\nmy_strcmp: %d/%d tests passed\n", passed, total);
  return (passed == total) ? 1 : 0;
}

int test_my_strncmp() {
  int passed = 0;
  int total = 0;

  // Test 1: equal strings with equal length
  total++;
  int result = my_strncmp("string", "string", 6);
  if (result != 0) {
    printf("Test 1 FAILED: equal strings, expected 0, got %d\n", result);
  } else {
    printf("Test 1 passed: equal strings\n");
    passed++;
  }

  // Test 2: strings with equal n size prefixes
  total++;
  result = my_strncmp("abchahaha", "abcjajaja", 3);
  if (result != 0) {
    printf("Test 2 FAILED: equal prefixes, expected 0, got %d\n", result);
  } else {
    printf("Test 2 passed: equal prefixes\n");
    passed++;
  }

  // Test 3: different prefixes
  total++;
  result = my_strncmp("aabkkk", "acbkkk", 3);
  if (result >= 0) {
    printf(
        "Test 3 FAILED: different prefixes, expected negative number, got %d\n",
        result);
  } else {
    printf("Test 3 passed: different prefixes\n");
    passed++;
  }

  // Test 4: NULL case
  total++;
  result = my_strncmp(NULL, ":))", 10);
  if (result != -1) {
    printf("Test 4 FAILED: NULL case, expected -1, got %d\n", result);
  } else {
    printf("Test 4 passed: NULL case\n");
    passed++;
  }

  // Test 5: n = 0
  total++;
  result = my_strncmp("xxx", "yyy", 0);
  if (result != 0) {
    printf("Test 6 FAILED: n=0, expected 0, got %d\n", result);
  } else {
    printf("Test 6 passed: n=0\n");
    passed++;
  }

  // Test 6: n larger than string lengths
  total++;
  result = my_strncmp("hi", "hi", 10);
  if (result != 0) {
    printf("Test 6 FAILED: n larger than strings, expected 0, got %d\n",
           result);
  } else {
    printf("Test 6 passed: n larger than strings\n");
    passed++;
  }

  printf("\nmy_strncmp: %d/%d tests passed\n", passed, total);
  return (passed == total) ? 1 : 0;
}

int test_my_getenv() {
  int total = 0;
  int passed = 0;

  char *env[] = {"PATH=/usr/bin:/bin", "HOME=/home/ako", "USER=ako", NULL};

  // Test 1: existing variable PATH
  total++;
  int result = my_strcmp(my_getenv("PATH", env), "/usr/bin:/bin");
  if (result == 0) {
    printf("Test 1 passed: PATH\n");
    passed++;
  } else {
    printf("Test 1 FAILED: PATH, expected 0, got %d\n", result);
  }

  // Test 2: existing variable HOME
  total++;
  result = my_strcmp(my_getenv("HOME", env), "/home/ako");
  if (result == 0) {
    printf("Test 2 passed: HOME\n");
    passed++;
  } else {
    printf("Test 2 FAILED: HOME, expected 0, got %d\n", result);
  }

  // Test 3: non-existent variable
  total++;
  char *answer = my_getenv("NONEXISTENT", env);
  if (answer == NULL) {
    printf("Test 3 passed: NONEXISTENT\n");
    passed++;
  } else {
    printf("Test 3 FAILED: NONEXISTENT, expected NULL, got %s\n", answer);
  }

  // Test 4: edge case variable is NULL
  total++;
  answer = my_getenv(NULL, env);
  if (answer == NULL) {
    printf("Test 4 passed: variable NULL\n");
    passed++;
  } else {
    printf("Test 4 FAILED: variable NULL, expected NULL, got %s\n", answer);
  }

  // Test 5: edge case env is NULL
  total++;
  answer = my_getenv("PATH", NULL);
  if (answer == NULL) {
    printf("Test 5 passed: env NULL\n");
    passed++;
  } else {
    printf("Test 5 FAILED: env NULL, expected NULL, got %s\n", answer);
  }

  printf("\nmy_getenv: %d/%d tests passed\n", passed, total);
  return (passed == total) ? 1 : 0;
}

int test_my_strcpy() {
  int total = 0;
  int passed = 0;
  char dest[20];

  // Test 1: normal copy
  total++;
  char *result = my_strcpy(dest, "yo hablo espanol");
  if (result != dest || my_strcmp(dest, "yo hablo espanol") != 0) {
    printf("Test 1 FAILED: normal copy\n");
  } else {
    passed++;
    printf("Test 1 passed: normal copy\n");
  }

  // Test 2: empty string
  total++;
  result = my_strcpy(dest, "");
  if (my_strcmp(dest, "") != 0) {
    printf("Test 2 FAILED: empty string");
  } else {
    passed++;
    printf("Test 2 passed: empty copy\n");
  }

  // Test 3: NULL source
  total++;
  if (my_strcpy(dest, NULL) != NULL) {
    printf("Test 3 FAILED: NULL source");
  } else {
    printf("Test 3 passed: NULL source");
    passed++;
  }

  printf("\nmy_strcpy: %d/%d tests passed\n", passed, total);
  return (passed == total) ? 1 : 0;
}

int test_my_strdup() {
  int total = 0;
  int passed = 0;

  // Test 1: normal duplication
  total++;
  char *original = "Hello World";
  char *duplicate = my_strdup(original);
  if (duplicate != NULL && my_strcmp(original, duplicate) == 0 &&
      duplicate != original) {
    printf("Test 1 passed: normal duplication\n");
    passed++;
  } else {
    printf("Test 1 FAILED: normal duplication\n");
  }

  // Test 2: empty string
  total++;
  char *empty = my_strdup("");
  if (empty != NULL && my_strcmp(empty, "") == 0) {
    printf("Test 2 passed: empty string\n");
    passed++;
  } else {
    printf("Test 2 FAILED: empty string");
  }

  // Test 3: NULL input
  total++;
  if (my_strdup(NULL) == NULL) {
    printf("Test 3 passed: NULL input\n");
    passed++;
  } else {
    printf("Test 3 FAILED: NULL input");
  }

  free(duplicate);
  free(empty);

  printf("\nmy_strdup: %d/%d tests passed\n", passed, total);
  return (passed == total) ? 1 : 0;
}

int test_my_strchr() {
  int total = 0;
  int passed = 0;
  char str[] = "good night";

  // Test 1: character exists
  total++;
  char *result = my_strchr(str, 'd');
  if (result != NULL && *result == 'd' && result == &str[3]) {
    printf("Test 1 passed: character exists\n");
    passed++;
  } else {
    printf("Test 1 FAILED: character exists, expected 'd' at position 3\n");
  }

  // Test 2: character not found
  total++;
  result = my_strchr(str, 'k');
  if (result == NULL) {
    printf("Test 2 passed: character not found\n");
    passed++;
  } else {
    printf("Test 2 FAILED: character not found, expected NULL\n");
  }

  // Test 3: NULL string input
  total++;
  result = my_strchr(NULL, 'a');
  if (result == NULL) {
    printf("Test 3 passed: NULL string input\n");
    passed++;
  } else {
    printf("Test 5 FAILED: NULL string input, expected NULL\n");
  }

  // Test 4: empty string
  total++;
  char empty[] = "";
  result = my_strchr(empty, 'x');
  if (result == NULL) {
    printf("Test 4 passed: empty string\n");
    passed++;
  } else {
    printf("Test 4 FAILED: empty string, expected NULL\n");
  }

  // Test 5: first occurence
  total++;
  result = my_strchr(str, 'o');
  if (result != NULL && *result == 'o' && result == &str[1]) {
    printf("Test 5 passed: first occurence found\n");
    passed++;
  } else {
    printf(
        "Test 5 FAILED: first occurence, expected first 'o' at position 1\n");
  }

  printf("\nmy_strchr: %d/%d tests passed\n", passed, total);
  return (passed == total) ? 1 : 0;
}

int test_my_strtok() {
  int total = 0;
  int passed = 0;
  char buffer[100];

  // Test 1: normal tokenization with space delimiter
  total++;
  my_strcpy(buffer, "hello world test");
  char *token1 = my_strtok(buffer, " ");
  char *token2 = my_strtok(NULL, " ");
  char *token3 = my_strtok(NULL, " ");
  char *token4 = my_strtok(NULL, " ");
  if (token1 != NULL && my_strcmp(token1, "hello") == 0 && token2 != NULL &&
      my_strcmp(token2, "world") == 0 && token3 != NULL &&
      my_strcmp(token3, "test") == 0 && token4 == NULL) {
    printf("Test 1 passed: normal tokenization with spaces\n");
    passed++;
  } else {
    printf("Test 1 FAILED: normal tokenization, expected 'hello', 'world', "
           "'test', NULL\n");
  }

  // Test 2: multiple delimiters
  total++;
  my_strcpy(buffer, ";;;;,,hello,,world;test,,,;");
  token1 = my_strtok(buffer, ",;");
  token2 = my_strtok(NULL, ",;");
  token3 = my_strtok(NULL, ",;");
  token4 = my_strtok(NULL, ",;");
  if (token1 != NULL && my_strcmp(token1, "hello") == 0 && token2 != NULL &&
      my_strcmp(token2, "world") == 0 && token3 != NULL &&
      my_strcmp(token3, "test") == 0 && token4 == NULL) {
    printf("Test 2 passed: multiple delimiters\n");
    passed++;
  } else {
    printf("Test 2 FAILED: multiple delimiters\n");
  }

  // Test 3: only delimiters
  total++;
  my_strcpy(buffer, ",,,");
  token1 = my_strtok(buffer, ",");
  if (token1 == NULL) {
    printf("Test 6 passed: only delimiters\n");
    passed++;
  } else {
    printf("Test 6 FAILED: only delimiters, expected NULL\n");
  }

  // Test 4: empty string
  total++;
  my_strcpy(buffer, "");
  token1 = my_strtok(buffer, ",");
  if (token1 == NULL) {
    printf("Test 7 passed: empty string\n");
    passed++;
  } else {
    printf("Test 7 FAILED: empty string, expected NULL\n");
  }

  // Test 5: NULL input on first call
  total++;
  token1 = my_strtok(NULL, ",");
  if (token1 == NULL) {
    printf("Test 8 passed: NULL input on first call\n");
    passed++;
  } else {
    printf("Test 8 FAILED: NULL input on first call, expected NULL\n");
  }

  printf("\nmy_strtok: %d/%d tests passed\n", passed, total);
  return (passed == total) ? 1 : 0;
}

int main() {
  int passed = 0;
  int total = 0;

  printf("Running tests...\n\n");

  total++;
  passed += test_my_strlen();
  printf("\n=========================\n");

  total++;
  passed += test_my_strcmp();
  printf("\n=========================\n");

  total++;
  passed += test_my_strncmp();
  printf("\n=========================\n");

  total++;
  passed += test_my_getenv();
  printf("\n=========================\n");

  total++;
  passed += test_my_strdup();
  printf("\n=========================\n");

  total++;
  passed += test_my_strcpy();
  printf("\n=========================\n");

  total++;
  passed += test_my_strchr();
  printf("\n=========================\n");

  total++;
  passed += test_my_strtok();
  printf("\n=========================\n");

  printf("Overall: %d/%d tests functions passed\n", passed, total);

  return (passed == total) ? 0 : 1;
}