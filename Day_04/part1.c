#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WINNING_NUMS 10
#define MAX_PLAYER_NUMS 25
#define LINE_LEN 256

int main(int argc, char **argv) {
  if (argc != 2) {
    printf("usage: %s input_file\n", argv[0]);
    return 0;
  }

  FILE *fp = fopen(argv[1], "r");
  if (fp == NULL) {
    fprintf(stderr, "cannot open file %s\n", argv[1]);
    exit(EXIT_FAILURE);
  }

  char line[LINE_LEN];

  int pointSum = 0;

  while (fgets(line, LINE_LEN, fp) != NULL) {
    int winNums[MAX_WINNING_NUMS], playerNums[MAX_PLAYER_NUMS];
    int winNumCount = 0, playerNumCount = 0;
    int beginIndex = strstr(line, ":") - line;  // index of ':'

    // find the index of first number in the winning list
    while (!isdigit(line[beginIndex])) beginIndex++;

    // populate winning numbers array
    int number = 0;
    int i;
    int lastChar = 0;
    for (i = beginIndex; line[i] != '|'; i++) {
      if (isdigit(line[i])) {
        number = number * 10 + (line[i] - '0');
        lastChar = line[i];
      } else {
        if (isdigit(lastChar)) {
          winNums[winNumCount++] = number;
          number = 0;
          lastChar = line[i];
        } else {
          lastChar = line[i];
        }
      }
    }

    /* for (int index = 0; index < winNumCount; index++) { */
    /*   printf("%d, ", winNums[index]); */
    /* } */

    // find the index of first number in the players list
    while (!isdigit(line[i])) i++;

    // populate players numbers array
    for (i; i < strlen(line); i++) {
      if (isdigit(line[i])) {
        number = number * 10 + (line[i] - '0');
        lastChar = line[i];
      } else {
        if (isdigit(lastChar)) {
          playerNums[playerNumCount++] = number;
          number = 0;
          lastChar = line[i];
        } else {
          lastChar = line[i];
        }
      }
    }

    /* for (int index = 0; index < playerNumCount; index++) { */
    /*   printf("%d, ", playerNums[index]); */
    /* } */
    /* putchar('\n'); */

    int point = 0;
    for (int i = 0; i < playerNumCount; i++) {
      int exists = 0;
      for (int j = 0; j < winNumCount; j++) {
        if (winNums[j] == playerNums[i]) {
          exists = 1;
          break;
        }
      }

      if (exists) {
        if (point == 0) {
          point = 1;
        } else {
          point = point << 1;
        }
      }
    }

    pointSum += point;
  }

  printf("point sum: %d\n", pointSum);

}
