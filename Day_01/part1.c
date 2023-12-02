#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define TRUE 1
#define FALSE 0

void append(char *line, char newchar) {
  int len = strlen(line);
  line[len] = newchar;
  return;
}

int main(int argc, char **argv) {
  int calibration = 0;
  FILE *fp;
  fp = fopen(argv[1], "r");
  char line[256];
  while(fgets(line, sizeof(line), fp)) {
    int len = strlen(line);
    char digitarray[256] = {0};
    for(int i = 0; i < len; i++) {
      if(isdigit(line[i])) {
        append(digitarray, line[i]);
      }
    }
    int len_digitarray = strlen(digitarray);
    calibration += (digitarray[0] - '0') * 10 + (digitarray[len_digitarray - 1] - '0');
  }

  printf("calibration: %d\n", calibration);

  return 0;
}
