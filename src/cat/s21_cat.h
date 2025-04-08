#include <stdio.h>

typedef struct flags {
  int b, e, n, s, t, v;
  int invalid_option;
} flags;

flags flag_parser(int argc, char *argv[]);
void cat_file(flags flag, FILE *file, size_t *count_of_string,
              int *empty_string, int *ch_prev);
void read_files(int argc, char *argv[], flags flag);
void show_nonprinting(int *ch);
void invalid_option();

void invalid_file(char *file);
