#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
  int NumNonblank;
  int End;
  int Num;
  int Squeeze;
  int Tab;
  int View;
} Flags;

void print_c(int c, int *c1, int *str_num, Flags *flags, int *pr_line_print) {
  if (flags->Squeeze == 0 || *c1 != '\n' || c != '\n' || *pr_line_print == 0) {
    *pr_line_print = (*c1 == '\n' && c == '\n') ? 1 : 0;

    if ((flags->Num || (flags->NumNonblank && c != '\n')) && *c1 == '\n') {
      *str_num += 1;
      printf("%6d\t", *str_num);
    }
    if (flags->End && c == '\n') printf("$");
    if ((flags->Tab && c == '\t') ||
        (flags->View && c >= 0 && c <= 31 && c != '\n' && c != '\t')) {
      printf("^");
      c = c + 64;
    }

    putchar(c);
    *c1 = c;
  }
}

void print_file(FILE *f, Flags *flags, int *str_num, int *c1) {
  int pr_line_print = 0;
  int c = fgetc(f);
  while (c != EOF) {
    print_c(c, c1, str_num, flags, &pr_line_print);
    c = fgetc(f);
  }
}

int main(int argc, char *argv[]) {
  Flags flags = {0, 0, 0, 0, 0, 0};
  const struct option long_options[] = {
      {"number-nonblank", 0, &flags.NumNonblank, 1},
      {"number", 0, &flags.Num, 1},
      {"squeeze-blank", 0, &flags.Squeeze, 1},
      {NULL, 0, NULL, 0}};
  int opt = getopt_long(argc, argv, "bevEnstT", long_options, NULL);
  while (opt != -1) {
    if (opt == 'b') {
      flags.NumNonblank = 1;
    } else if (opt == 'e') {
      flags.View = 1;
      flags.End = 1;
    } else if (opt == 'E') {
      flags.End = 1;
    } else if (opt == 'n') {
      flags.Num = (flags.NumNonblank == 1) ? 0 : 1;
    } else if (opt == 's') {
      flags.Squeeze = 1;
    } else if (opt == 't') {
      flags.View = 1;
      flags.Tab = 1;
    } else if (opt == 'T') {
      flags.Tab = 1;
    }
    opt = getopt_long(argc, argv, "bevEnstT", long_options, NULL);
  }
  opt = optind;
  int str_num = 0, c1 = '\n';
  while (opt < argc) {
    FILE *f = fopen(argv[opt], "r");
    if (!f) {
      perror(argv[opt]);
      return EXIT_FAILURE;
    } else {
      print_file(f, &flags, &str_num, &c1);
    }
    fclose(f);
    opt++;
  }
  return 0;
}