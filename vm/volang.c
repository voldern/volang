#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>

//#include "grammar.c"
#include "vl.h"

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

static int eval_file(char *filename) {
  FILE *fp;
  struct stat stats;

  if (stat(filename, &stats) == -1) {
    perror(filename);
    return 1;
  }

  fp = fopen(filename, "rb");
  if (!fp) {
    perror(filename);
    return 1;
  }

  char *buf = VL_MALLOC_N(char, stats.st_size + 1);

  if (fread(buf, 1, stats.st_size, fp) == stats.st_size) {
    //eval(buf, filename, verbose);
    VlBlock_compile(buf);
  } else {
    perror(filename);
  }

  VL_FREE(buf);
  
  return 0;
}

int main(int argc, char **argv) {
  int opt;
  
  while ((opt = getopt(argc, argv, "e:ivdh")) != -1) {
    switch(opt) {
    case 'i':
      /* printf("> "); */
      /* while (yyparse()) { */
      /*   printf("> "); */
      /* } */
      /* printf("> "); */
      /* VlBlock_compile(NULL); */
      /* printf("\n"); */
      printf("Not implemented yet!\n");
      return 0;
    case 'e':
      VlBlock_compile(optarg);
      printf("\n");
      return 0;
    case 'v':
      printf("volang %s\n", VL_VERSION);
      return 0;
    default:
    case 'h':
      return usage();
    }
  }

  if (argc == 2 && strlen(argv[1]) > 0)
    eval_file(argv[1]);

  return 0;
}
