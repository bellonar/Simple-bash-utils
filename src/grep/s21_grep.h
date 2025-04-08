#include <stdio.h>

typedef struct flags {
  int e, i, v, c, l, n, h, s, f, o, count_of_patterns, count_of_files, error;
  char **patterns;
} flags;
#define MAX_BUFFER 256

flags flag_parser(int argc, char *argv[]);
void output(flags flag, FILE *file, size_t *count_of_string);
void pattern_from_string(flags *flag, char *pt);
void pattern_from_file(flags *flag, char *filename);
void read_files(int argc, char *argv[], flags flag);
void grep_file(flags flag, char *filename, FILE *file);
int is_match(flags flag, char *line);
void invalid_file(char *file);
void grep_with_o(flags flag, size_t line_count, char *line, char *filename);
void free_all(flags *flag);
void invalid_file(char *file);
void invalid_option();
int reg_for_o(char *line, flags flag);