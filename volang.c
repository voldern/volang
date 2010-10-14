#include <stdio.h>
#include <unistd.h>

//#include "vm/grammar.c"

#define VL_VERSION "0.1"

static int usage() {
  printf("usage: volang [options] [file]\n"
         "options:\n"
         " -i interactive\n"
         " -e eval code\n"
         " -v print version\n"
         " -h print this\n");

  return 1;
}

int main(int argc, char **argv) {
  int opt;
  
  while ((opt = getopt(argc, argv, "e:ivdh")) != -1) {
    switch(opt) {
    case 'i':
      printf("> ");
      while (yyparse()) {
        printf("> ");
      }
      printf("\n");
      return 0;
    case 'e':
      charbuf = optarg;
      yyparse();
      printf("\n");
      return 0;
    case 'v':
      printf("volang %s\n", VL_VERSION);
      return 1;
    default:
    case 'h':
      return usage();
    }
  }

  return 0;
}
