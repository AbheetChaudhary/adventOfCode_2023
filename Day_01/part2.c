#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include "lib/libaoc.h"

#define LINE_LENGTH 256
#define MAX_OCCURANCE LINE_LENGTH

struct Digit {
  int digit;
  int index;
};

char *digitspellings[10] = {"zero", "one", "two", "three", "four", "five", "six",
  "seven", "eight", "nine"};

void digit_sweep(struct Digit **, char *);
void spelling_sweep(struct Digit **, char *);
void add_digit(struct Digit **, struct Digit *);  // no error handling, it must always succeed
void print_digit(struct Digit *);

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
    digit_sweep(dp, line);
    spelling_sweep(dp, line);

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

void digit_sweep(struct Digit **dp, char *line) {
  for(int i = 0; line[i] != '\0'; i++) {
    if(isdigit(line[i])) {
      struct Digit *digit_p = (struct Digit *) malloc(sizeof(struct Digit));
      digit_p->digit = line[i] - '0';
      digit_p->index = i;
      add_digit(dp, digit_p);
    }
  }

  return;
}

void spelling_sweep(struct Digit **dp, char *line) {
  for(int i = 0; i < 10; i++) {
    int *indices = (int *) malloc(MAX_OCCURANCE * sizeof(int));
    super_strstr(line, digitspellings[i], indices);

    for(int j = 0; indices[j] != -1 && j < MAX_OCCURANCE; j++) {
      struct Digit *digit_p = (struct Digit *) malloc(sizeof(struct Digit));
      digit_p->digit = i;
      digit_p->index = indices[j];
      add_digit(dp, digit_p);
    }

    free(indices);
  }
}

void add_digit(struct Digit **dp, struct Digit *digit_p) {
  int i = 0;
  while(dp[i] != NULL) i++;
  dp[i] = digit_p;
  return;
}

void print_digit(struct Digit *digit) {
  printf("{digit: %d, index: %d}\n", digit->digit, digit->index);
}
