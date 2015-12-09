#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include "myerror.h"

#define min(x,y) ((x) < (y) ? (x) : (y))

#define BUFFSIZE 1024
char buffer[BUFFSIZE] ;

void fill_black(const int black, const size_t n, int fd) {
  int l, m = n ;
  char c = black ? 0 : 255 ;
  for (int i = 0 ; i < BUFFSIZE; ++i) buffer[i] = c;
  while (m > 0) {
    if ((l = write(fd, buffer, min(BUFFSIZE, m))) < 0)
      err_sys("black : write error") ;
    m -= l;
  }
}

char usage[] = "black [-o outfile] [-h] [-w]" ;
char help[]  = "Parameters :\n"
  "-o outfile : output file. Filename - means stdout. Default stdout.\n"
  "-s size    : size in bytes of black bytes.\n"
  "-w         : fill with white.\n"
  "-h         : this help message."  ;

int main(int argc, char* argv[]) {
  int fd=STDOUT_FILENO , size=0, black=1;
  char ch, *pt ;

  while((ch = getopt(argc, argv, "hwo:s:")) != -1)
    switch(ch) {
    case 's' :
      size = (int)strtol(optarg, &pt, 10);
      if (pt == optarg)
        err_quit("black : %s not a integer", optarg);
      size = abs(size) ;
      break;
    case 'h' :
      printf("%s\n\n%s\n", usage, help);
      return EXIT_SUCCESS;
      break ;
    case 'w' :
      black = 0 ;
      break ;
    case 'o' :
      if (strncmp(optarg, "-", 1))
        if ((fd = open(optarg, O_WRONLY|O_CREAT|O_TRUNC, 0660)) < 0)
          err_sys("black : cannot open file %s", optarg);
      break;
    default :
      err_quit("usage : %s\n", usage);
      break;
    }
  fill_black(black, size, fd);
  close(fd);
  return EXIT_SUCCESS;
}
  
