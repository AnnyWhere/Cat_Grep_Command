#include "s21_grep.h"

#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_lines(FILE *f, Flags *flags, regex_t regex, int many_f,
                 char *filename) {
  char line[500];
  int pr_line_print = 1;
  int is_match = 0;
  int count = 0;
  regmatch_t match;
  while (fgets(line, sizeof(line), f) != NULL) {
    if ((regexec(&regex, line, 1, &match, 0) == 0 && flags->V_invert == 0) ||
        (regexec(&regex, line, 1, &match, 0) && flags->V_invert)) {
      if (flags->C_count) {
        count += 1;
      } else if (flags->L_file_names) {
        is_match = 1;
      } else if (flags->N_num_lines) {
        (many_f) ? printf("%s:%d:%s", filename, pr_line_print, line)
                 : printf("%d:%s", pr_line_print, line);
        // printf("%.*s ", match.rm_eo-match.rm_so, line);
      } else {
        (many_f) ? printf("%s:%s", filename, line) : printf("%s", line);
      }
      if (line[strlen(line) - 1] != '\n' && !flags->L_file_names &&
          !flags->C_count) {
        printf("\n");
      }
    }
    pr_line_print++;
  }
  if (is_match && flags->L_file_names) printf("%s\n", filename);
  if (flags->C_count)
    (many_f) ? printf("%s:%d\n", filename, count) : printf("%d\n", count);
}

int read_file(int opt, int argc, char **argv, Flags flags, char *pattern) {
  regex_t regex;
  if (opt < argc) {
    if (flags.E_pattern) {
      (flags.I_ignore) ? regcomp(&regex, pattern, REG_EXTENDED | REG_ICASE)
                       : regcomp(&regex, pattern, REG_EXTENDED);
    } else {
      (flags.I_ignore) ? regcomp(&regex, pattern, REG_ICASE)
                       : regcomp(&regex, pattern, 0);
    }
  }
  int many_f = argc - (opt + 1) - 1;
  while (++opt < argc) {
    FILE *f = fopen(argv[opt], "r");
    if (!f) {
      perror(argv[opt]);
      regfree(&regex);
      return EXIT_FAILURE;
    } else {
      print_lines(f, &flags, regex, many_f, argv[opt]);
    }
    fclose(f);
  }
  regfree(&regex);
  return 0;
}

int main(int argc, char *argv[]) {
  Flags flags = {0, 0, 0, 0, 0, 0};
  char *pattern = NULL;
  int opt = getopt(argc, argv, "e:ivcln");
  while (opt != -1 && flags.E_pattern != 1) {
    if (opt == 'e') {
      flags.E_pattern = 1;
      pattern = optarg;
    } else if (opt == 'i') {
      flags.I_ignore = 1;
    } else if (opt == 'v') {
      flags.V_invert = 1;
    } else if (opt == 'c' && flags.L_file_names == 0) {
      flags.C_count = 1;
    } else if (opt == 'l') {
      flags.L_file_names = 1;
      flags.C_count = 0;
    } else if (opt == 'n') {
      flags.N_num_lines = 1;
      // } else if (opt == 'h') {
      // flags.H_without_names = 1;
    } else {
      printf("invalid option %c\n", opt);
    }
    opt = getopt(argc, argv, "e:ivcln");
  }
  if (flags.E_pattern != 1) {
    opt = optind;
    pattern = argv[opt];
  } else {
    opt = optind - 1;
  }
  if (pattern) {
    read_file(opt, argc, argv, flags, pattern);
  }
  return 0;
}