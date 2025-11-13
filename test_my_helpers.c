#include "dz_shell.h"

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

  printf("Overall: %d/%d tests functions passed\n", passed, total);

  return (passed == total) ? 0 : 1;
}