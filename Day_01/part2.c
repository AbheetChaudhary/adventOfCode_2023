#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#define LINE_LENGTH 256
#define MAX_OCCURANCE LINE_LENGTH

struct Digit {
  int digit;
  int index;
};

char *digitspellings[10] = {"zero", "one", "two", "three", "four", "five", "six",
  "seven", "eight", "nine"};

void digitsweep(struct Digit **, char *);
void spellingsweep(struct Digit **, char *);
void adddigit(struct Digit **, struct Digit *);  // no error handling, it must always succeed
int superstrstr(char *, char *, int *);
void printdigit(struct Digit *);

int main(int argc, char **argv) {
  if(argc != 2) {
    printf("usage: %s input_file\n", argv[0]);
    return 0;
  }

  FILE *fp = fopen(argv[1], "r");
  if(fp == NULL) {
    printf("incorrect filename: %s\n", argv[0]);
    return 0;
  }

  int calibration = 0;

  char line[LINE_LENGTH];
  while(fgets(line, LINE_LENGTH, fp)) {
    struct Digit *dp[LINE_LENGTH] = {NULL};
    digitsweep(dp, line);
    spellingsweep(dp, line);

    // get first & last digit
    int first = 0, first_index = LINE_LENGTH;
    int last = 0, last_index = 0;
    for(int i = 0; dp[i] != NULL && i < MAX_OCCURANCE; i++) {
      if(dp[i]->index <= first_index) {
        first_index = dp[i]->index;
        first = dp[i]->digit;
      }

      if(dp[i]->index >= last_index) {
        last_index = dp[i]->index;
        last = dp[i]->digit;
      }
    }

    calibration += first * 10 + last;
    // printf("number: %c%c\n", first + '0', last + '0');
    // printf("updated calibration: %d\n", calibration);

    for(int i = 0; dp[i] != NULL; i++) {
      free(dp[i]);
    }
  }

  printf("calibration: %d\n", calibration);

  return 0;
}

void digitsweep(struct Digit **dp, char *line) {
  for(int i = 0; line[i] != '\0'; i++) {
    if(isdigit(line[i])) {
      struct Digit *digit_p = (struct Digit *) malloc(sizeof(struct Digit));
      digit_p->digit = line[i] - '0';
      digit_p->index = i;
      adddigit(dp, digit_p);
    }
  }

  return;
}

void spellingsweep(struct Digit **dp, char *line) {
  for(int i = 0; i < 10; i++) {
    int *indices = (int *) malloc(MAX_OCCURANCE * sizeof(int));
    superstrstr(line, digitspellings[i], indices);

    for(int j = 0; indices[j] != -1 && j < MAX_OCCURANCE; j++) {
      struct Digit *digit_p = (struct Digit *) malloc(sizeof(struct Digit));
      digit_p->digit = i;
      digit_p->index = indices[j];
      adddigit(dp, digit_p);
    }

    free(indices);
  }
}

void adddigit(struct Digit **dp, struct Digit *digit_p) {
  int i = 0;
  while(dp[i] != NULL) i++;
  dp[i] = digit_p;
  return;
}

int superstrstr(char *haystack, char *needle, int *indices) {
  char *ptr, *temp_haystack;
  temp_haystack = haystack;
  int i;
  for(i = 0; (ptr = strstr(haystack, needle)) != NULL; i++) {
    indices[i] = ptr - temp_haystack;
    haystack = ptr + 1;
  }

  indices[i] = -1;
}

void printdigit(struct Digit *digit) {
  printf("{digit: %d, index: %d}\n", digit->digit, digit->index);
}
