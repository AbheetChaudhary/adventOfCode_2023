#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINE_LEN 256
#define MAX_NUMBERS 2048
#define MAX_SYMBOLS 2048

struct Number {
  int value;
  int row;
  int column;
  int isPart;
};

struct Symbol {
  int shape;
  int row;
  int column;
};

/*
 * It takes a stream whose first character is a digit and returns the integer of
 * which that first character is part of. It also takes an int pointer pointing
 * to the current column in the stream.
 */

int get_int(FILE *fp, int *column) {
  int c;
  int value = 0;
  while (isdigit(c = fgetc(fp))) {
    value = value * 10 + (c - '0');
    (*column)++;
  }

  ungetc(c, fp);

  return value;
}

// count the number of digits in an integer. The input must be non-negative
int digit_count(int number) {
  int digit_count = 0;
  if (number == 0) {
    return 1;
  } else {
    while (number != 0) {
      digit_count++;
      number /= 10;
    }
    return digit_count;
  }
}

/*
 * this function sets 1 to the isPart field of all the numbers that are parts
 */

void symbol_proximity_scan(struct Symbol *symbol, struct Number **numbers,
                           int numberCount) {
  int digitCount;
  /*
     scan the proximity of each symbol by first narrowing down the rows to be
     just below/above or the same as the symbol. Then narrow down the possible
     columns by using the length data of the number->value field.
  */
  for (int i = 0; i < numberCount; i++) {
    if (!numbers[i]->isPart && numbers[i]->row >= symbol->row - 1 &&
        numbers[i]->row <= symbol->row + 1) {
      digitCount = digit_count(numbers[i]->value);
      if (numbers[i]->column <= symbol->column + 1 &&
          numbers[i]->column >= symbol->column - digitCount) {
        numbers[i]->isPart = 1;
      }
    }
  }
}

int main(int argc, char **argv) {
  if (argc != 2) {
    printf("usage: %s input_file\n", argv[0]);
    exit(EXIT_SUCCESS);
  }

  FILE *fp = fopen(argv[1], "r");
  if (fp == NULL) {
    fprintf(stderr, "could not open file %s\n", argv[1]);
    exit(EXIT_FAILURE);
  }

  // an array of pointer to numbers
  struct Number **numbers =
      (struct Number **)malloc(MAX_NUMBERS * sizeof(struct Number *));
  int numberCount = 0;

  // an array of pointer to symbols
  struct Symbol **symbols =
      (struct Symbol **)malloc(MAX_SYMBOLS * sizeof(struct Symbol *));
  int symbolCount = 0;

  int row = 0, column = 0; // keep track of the coordinates where a symbol or
                           // number occurs
  int c;

  while ((c = fgetc(fp)) != EOF) {
    if (c == '.') {
      column++;
    } else if (isdigit(c)) {
      ungetc(c, fp);
      struct Number *number = (struct Number *)malloc(sizeof(struct Number));
      number->isPart = 0;
      number->column = column;
      number->row = row;
      int num_value = get_int(fp, &column);
      number->value = num_value;

      if (numberCount < MAX_NUMBERS) {
        numbers[numberCount++] = number;
      } else {
        fprintf(stderr,
                "reached number limit. Cannot store more than %d numbers\n",
                MAX_NUMBERS);
        exit(EXIT_SUCCESS);
      }
    } else if (c == '\n') {
      column = 0;
      row++;
    } else {
      struct Symbol *symbol = (struct Symbol *)malloc(sizeof(struct Symbol));
      symbol->row = row;
      symbol->column = column++;
      symbol->shape = c;
      if (symbolCount < MAX_SYMBOLS) {
        symbols[symbolCount++] = symbol;
      } else {
        fprintf(stderr,
                "reached symbol limit. Cannot store more than %d symbols\n",
                MAX_SYMBOLS);
        exit(EXIT_SUCCESS);
      }
    }
  }

  /* print numbers and their location */
  // for (int i = 0; i < numberCount; i++) {
  //   printf("{value: %d, row: %d, column: %d}\n", numbers[i]->value,
  //   numbers[i]->row, numbers[i]->column);
  // }

  /* print symbol locations */
  // for (int i = 0; i < symbolCount; i++) {
  //   printf("{symbol @ row: %d, column: %d}\n", symbols[i]->row,
  //   symbols[i]->column);
  // }

  // search in each symbols proximity for part numbers
  for (int i = 0; i < symbolCount; i++) {
    symbol_proximity_scan(symbols[i], numbers, numberCount);
  }

  // find sum of parts
  int partSum = 0;
  for (int i = 0; i < numberCount; i++) {
    if (numbers[i]->isPart == 1) {
      // printf("part number: %d\n", numbers[i]->value);
      partSum += numbers[i]->value;
    } /* else {
      printf("non part: %d\n", numbers[i]->value);
    } */
  }

  printf("partSum: %d\n", partSum);

  int gearRatioSum = 0;
  int digitCount;
  for (int i = 0; i < symbolCount; i++) {
    if (symbols[i]->shape == '*') {
      /*
         create gear ratio of every shape that can be gear and keep track the
         number of adjacent parts. In the end if parts were exactly 2 then it is
         indeed a gear and so add the gear ratio to the aggregate sum
      */

      int adjacentPartCount = 0, shapeGearRatio = 1;
      for (int j = 0; j < numberCount; j++) {
        if (numbers[j]->isPart && numbers[j]->row >= symbols[i]->row - 1 &&
            numbers[j]->row <= symbols[i]->row + 1) {
          digitCount = digit_count(numbers[j]->value);
          if (numbers[j]->column <= symbols[i]->column + 1 &&
              numbers[j]->column >= symbols[i]->column - digitCount) {
            adjacentPartCount++;
            shapeGearRatio *= numbers[j]->value;
          }
        }
      }
      if (adjacentPartCount == 2) {
        gearRatioSum += shapeGearRatio;
      }
    }
  }

  printf("gear ratio sum: %d\n", gearRatioSum);

  exit(EXIT_SUCCESS);
}
