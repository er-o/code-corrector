#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "myerror.h"

#define PGN  "trans"
#define BUFFSIZE 1024

#ifndef RAND_MAX
long max = (1 << 31) - 1;
#else
  long max = RAND_MAX ;  
#endif

long _np ;

long init_bernoulli(const double p) {
  _np = (long) ((1-p) * (double)max) ;
  return _np;
}

#define bernoulli() (random() > _np ? 1 : 0)

void bruit_bytes(char* s, const size_t n) {
  for (int i = 0; i < n; ++i) {
    char c = s[i] ;
    for (int j = 0; j < 8; ++j)
      if (random() > _np) c ^= (1 << j);
    s[i] = c ;
  }
}

char io_buff[BUFFSIZE] ;
void transmit(int fdin, int fdout) {
  int n;
  
  while ((n = read(fdin, io_buff, BUFFSIZE)) > 0) {
    bruit_bytes(io_buff, n) ;
    if (write(fdout, io_buff, n) < 0)
      err_sys("transmit : output error") ;
  }
  if (n < 0)
    err_sys("transmit : input error");
}

char usage[] =  "transmit -p <prob> [-h] [-i infile] [-o outfile]" ;
char help[]  =  "transmit -p <prob> [-i infile] [-o outfile]\n\n"
  "-p  proba   : parameter of bernoulli law (default 0.5)\n"
  "-i  infile  : input file name. Infile - is stdin (default stdin)\n"
  "-o  outfile : output file name.  Outfile - is stdout (default stdout)\n"
  "-h          : this help message" ;



int main(int argc, char*  argv[]) {
  int fdin  = STDIN_FILENO;
  int fdout = STDOUT_FILENO ;
  double p  = 0.5 ;
  char* pt, ch ;
  
  while ((ch = getopt(argc, argv, "hi:o:p:")) != -1) {
    switch (ch) {
    case 'h' :
      printf("%s\n", help) ;
      return EXIT_SUCCESS;
      break;
    case 'p' :
      p = strtod(optarg, &pt) ;
      if (optarg == pt)
        err_quit("transmit : %s not a number", optarg) ;
      if ( p > 1. || p < 0)
        err_quit("transmit: %f not a valid probability", p);
      break ;
    case 'i' :
      if (strncmp(optarg, "-",1)) {
        if ((fdin = open(optarg, O_RDONLY, 0)) < 0)
          err_sys("transmit : Cannot open %s for input", optarg);
      }
      break ;
    case 'o':
      if (strncmp(optarg, "-",1)) {
        if ((fdout = open(optarg, O_WRONLY|O_CREAT|O_TRUNC, 0660)) < 0)
          err_sys("transmit : Cannot open %s for output", optarg);
      }
      break ;
    default:
      err_quit("usage : %s", usage) ;
      break ;
    }
  }
  init_bernoulli(p);
  transmit(fdin, fdout);
  close(fdin);
  close(fdout);
  return EXIT_SUCCESS;
}
