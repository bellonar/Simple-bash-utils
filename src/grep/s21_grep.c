#define _GNU_SOURCE
#include "s21_grep.h"

#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
  flags flag = {0};
  flag = flag_parser(argc, argv);
  if (flag.error != 1) read_files(argc, argv, flag);

  free_all(&flag);
  return 0;
}

flags flag_parser(int argc, char *argv[]) {
  flags flag = {0};
  flag.patterns = malloc(sizeof(char *) * MAX_BUFFER);
  int opt;
  while (((opt = getopt_long(argc, argv, "e:ivclnhsf:o", 0, NULL)) != -1) &&
         (flag.error != 1)) {
    switch (opt) {
      case 'e':
        flag.e = 1;
        pattern_from_string(&flag, optarg);
        break;
      case 'i':
        flag.i = REG_ICASE;
        break;
      case 'v':
        flag.v = 1;
        break;
      case 'c':
        flag.c = 1;
        break;
      case 'l':
        flag.l = 1;
        break;
      case 'n':
        flag.n = 1;
        break;
      case 'h':
        flag.h = 1;
        break;
      case 's':
        flag.s = 1;
        break;
      case 'f':
        flag.f = 1;
        pattern_from_file(&flag, optarg);
        break;
      case 'o':
        flag.o = 1;
        break;
      default:
        invalid_option();
        flag.error = 1;
    }
  }

  if ((flag.e == 0) && (flag.f == 0) && (flag.error != 1)) {
    pattern_from_string(&flag, argv[optind]);

    optind++;
  }
  flag.count_of_files = argc - optind;

  return flag;
}

void pattern_from_string(flags *flag, char *pt) {
  flag->patterns[flag->count_of_patterns] =
      malloc(sizeof(char) * strlen(pt) + sizeof(char));

  strcpy(flag->patterns[flag->count_of_patterns], pt);
  flag->count_of_patterns++;
}

void pattern_from_file(flags *flag, char *filename) {
  FILE *file = fopen(filename, "r");
  if (file == NULL) {
    invalid_file(filename);
    flag->error = 1;
  } else {
    size_t len = MAX_BUFFER;
    char *line = malloc(sizeof(char) * MAX_BUFFER);
    while (getline(&line, &len, file) != EOF) {
      if (line[strlen(line) - 1] == '\n') line[strlen(line) - 1] = '\0';
      if (line[0] != '\n') pattern_from_string(flag, line);
    }
    free(line);
    fclose(file);
  }
  
}

void read_files(int argc, char *argv[], flags flag) {
  for (int i = optind; i < argc; i++) {
    FILE *file = fopen(argv[i], "r");
    if ((file == NULL) && (flag.s != 1))

      invalid_file(argv[i]);
    else {
      grep_file(flag, argv[i], file);
      fclose(file);
    }
  }
}

void grep_file(flags flag, char *filename, FILE *file) {
  char *line = malloc(sizeof(char) * MAX_BUFFER);
  size_t line_count = 0, count_of_match = 0;
  int stop = 0;
  size_t len = MAX_BUFFER;
  while ((getline(&line, &len, file) != EOF) && (stop == 0)) {
    line_count++;

    if (flag.l == 1) {
      if (is_match(flag, line) == 0) {
        printf("%s\n", filename);
        stop = 1;
      }
    }
    if ((flag.c == 1) && (flag.l != 1)) {
      if (is_match(flag, line) == 0) count_of_match++;
    }
    if ((flag.o == 1) && (flag.l != 1) && (flag.c != 1)) {
      if (flag.v != 1) grep_with_o(flag, line_count, line, filename);
    }
    if ((is_match(flag, line) == 0) && (flag.o != 1) && (flag.l != 1) &&
        (flag.c != 1)) {
      if ((flag.h == 0) && (flag.count_of_files > 1)) printf("%s:", filename);
      if (flag.n == 1) printf("%zu:", line_count);
      printf("%s", line);
      if (line[strlen(line) - 1] != '\n') printf("\n");
    }
  }
  if ((flag.c == 1) && (flag.l == 0)) {
    if ((flag.h == 0) && (flag.count_of_files > 1)) printf("%s:", filename);
    printf("%zu\n", count_of_match);
  }

  free(line);
}

int is_match(flags flag, char *line) {
  int match = 1;
  for (int i = 0; i < flag.count_of_patterns; i++) {
    regex_t reg;
    regcomp(&reg, flag.patterns[i], flag.i);
    if (regexec(&reg, line, 0, 0, 0) == 0) match = 0;
    regfree(&reg);
  }
  if (flag.v == 1) {
    if (match == 1)
      match = 0;
    else
      match = 1;
  }
  return match;
}

void grep_with_o(flags flag, size_t line_count, char *line, char *filename) {
  char *line_ptrn = line;
  regmatch_t match = {0};
  int curent_index = 0;
  while ((curent_index = reg_for_o(line_ptrn, flag)) != -1) {
    if ((flag.h == 0) && (flag.count_of_files > 1)) printf("%s:", filename);
    if (flag.n == 1) printf("%zu:", line_count);
    regex_t rg;
    regcomp(&rg, flag.patterns[curent_index], flag.i);
    regexec(&rg, line_ptrn, 1, &match, 0);
    printf("%.*s\n", (int)(match.rm_eo - match.rm_so),
           (line_ptrn + match.rm_so));
    line_ptrn += match.rm_eo;

    regfree(&rg);
  }
}

void free_all(flags *flag) {
  for (int i = 0; i < flag->count_of_patterns; i++) free(flag->patterns[i]);
  free(flag->patterns);
}
void invalid_file(char *file) {
  printf("grep: %s: No such file or directory\n", file);
}
void invalid_option() {
  printf("Usage: grep [OPTION]... PATTERNS [FILE]...\n");
  printf("Try 'grep --help' for more information.\n");
}
int reg_for_o(char *line, flags flag) {
  int index = -1;
  regmatch_t tempmatch = {0};
  for (int i = 0; i < flag.count_of_patterns; i++) {
    regex_t reg;
    regmatch_t match = {0};
    regcomp(&reg, flag.patterns[i], flag.i);
    if (regexec(&reg, line, 1, &match, 0) == 0)
      if (((tempmatch.rm_so == 0) && (tempmatch.rm_eo == 0)) ||
          (tempmatch.rm_so > match.rm_so) ||
          ((tempmatch.rm_so == match.rm_so) &&
           (tempmatch.rm_eo < match.rm_eo))) {
        tempmatch.rm_so = match.rm_so;
        tempmatch.rm_eo = match.rm_eo;
        index = i;
      }

    regfree(&reg);
  }
  return index;
}