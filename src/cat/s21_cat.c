#include "s21_cat.h"

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  flags flag = {0};
  flag = flag_parser(argc, argv);
  if (flag.invalid_option == 0)
    read_files(argc, argv, flag);
  else
    invalid_option();
  return 0;
}

flags flag_parser(int argc, char *argv[]) {
  flags flag = {0};
  struct option l_flag[] = {{"number", 0, 0, 'n'},
                            {"number-nonblank", 0, 0, 'b'},
                            {"squeeze-blank", 0, 0, 's'},
                            {0, 0, 0, 0}};
  int opt;
  while (((opt = getopt_long(argc, argv, "beEnstT", l_flag, NULL)) != -1)) {
    switch (opt) {
      case 'b':
        flag.b = 1;
        break;
      case 'e':
        flag.e = 1;
        flag.v = 1;
        break;
      case 'E':
        flag.e = 1;
        break;
      case 'n':
        flag.n = 1;
        break;
      case 's':
        flag.s = 1;
        break;
      case 't':
        flag.t = 1;
        flag.v = 1;
        break;
      case 'T':
        flag.t = 1;
        break;
      default:
        flag.invalid_option = 1;
        break;
    }
  }
  return flag;
}

void read_files(int argc, char *argv[], flags flag) {
  size_t count_of_string = 1;
  int empty_string = 0;
  int ch_prev = '\n';
  for (int i = optind; i < argc; i++) {
    FILE *file = fopen(argv[i], "r");
    if (file == NULL)

      invalid_file(argv[i]);
    else {
      cat_file(flag, file, &count_of_string, &empty_string, &ch_prev);
      fclose(file);
    }
  }
}

void cat_file(flags flag, FILE *file, size_t *count_of_string,
              int *empty_string, int *ch_prev) {
  int ch;

  while ((ch = fgetc(file)) != EOF) {
    if ((flag.e == 1) && (*empty_string == 0) && ((ch == '\n'))) printf("$");
    if ((flag.s == 1) && (*empty_string == 0))

      if (*ch_prev == '\n' && ch == '\n') {
        *empty_string = 1;
        printf("%c", ch);
      }
    if (ch != '\n') {
      *empty_string = 0;
    }

    if ((((flag.n == 1) && (flag.b != 1)) || ((flag.b == 1) && (ch != '\n'))) &&
        ((*ch_prev == '\n'))) {
      printf("%6zu\t", *count_of_string);
      *count_of_string = *count_of_string + 1;
    }

    if ((flag.t == 1) && ((ch == '\t'))) {
      printf("^");
      ch += 64;
    }
    if ((flag.v == 1) && (ch != '\n') && (ch != '\t')) show_nonprinting(&ch);

    if (*empty_string == 0) printf("%c", ch);

    *ch_prev = ch;
  }
}
void show_nonprinting(int *ch) {
  if (*ch > 127) {
    printf("M-");
    *ch -= 128;
  }
  if (*ch == 127) {
    printf("^");
    *ch = 63;
  }
  if ((*ch >= 0) && (*ch < 32)) {
    printf("^");
    *ch += 64;
  }
}

void invalid_option() {
  printf("cat: invalid option\n");
  printf("Try 'cat --help' for more information.\n");
}

void invalid_file(char *file) {
  printf("cat: %s: No such file or directory\n", file);
}
