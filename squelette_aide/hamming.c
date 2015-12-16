#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include "myerror.h"

#define BUFFSIZE 1024

int GMatrix[4][8] = {{1,1,0,1,1,0,0,0},
                     {0,1,1,1,0,1,0,0},
                     {1,0,1,1,0,0,1,0},
                     {1,1,1,0,0,0,0,1}} ;

char G[4] ;
void init_generators() {
  for (int i=0; i < 4; ++i) {
    G[i] = 0 ;
    for (int j=0; j < 8; ++j)
      if (GMatrix[i][j]) G[i] |= (1 << (7-j)) ;
  }
}

void string_of_byte(char s[], const char c) {
  for (int i = 0 ; i < 8; ++i)
    s[7-i] = (c & (1 << i)) ? '1' : '0' ;
  s[8] = '\0' ;
}

void print_generator_matrix() {
  char buff[10] ;

  for (int i=0; i < 4; ++i) {
    string_of_byte(buff, G[i]) ;
    printf("%s\n", buff) ;
  }
}

void hamming(char c, char out[2]) {
  out[0] = out[1] = 0;
  for (int i=0; i < 4; ++i) {
    if (c & (1 << i))
      out[1] ^= G[i] ;
    if (c & (1 << (i+4)))
      out[0] ^= G[i] ;
  }
}

void print_test(char in) {
  char out[2] ;
  char buff[3][10] ;

  hamming(in,out);
  string_of_byte(buff[0], in);
  string_of_byte(buff[1], out[0]);
  string_of_byte(buff[2], out[1]);
  printf("in = %s out = %s %s\n", buff[0], buff[1], buff[2]) ;
}

void hamming_stream(int fin, int fdout) {
  char buffin[BUFFSIZE] ;
  char buffout[2 * BUFFSIZE] ;
  int n ;

  while ((n = read(fin, buffin, BUFFSIZE)) > 0) {
    for (int i=0; i < n; ++i)
      hamming(buffin[i], buffout + 2*i) ;
    if (write(fdout, buffout, 2*n) < 0)
      err_sys("hamming : output failure");
  }
  if (n < 0)
    err_sys("hamming : input failure");
}

char usage[] = "hamming [-gh][-t test][-i input] [-o output]" ;
char help[]  = "Parameters :\n"
  "-g        : show generator matrix.\n"
  "-t test   : lowest byte of integer test gets encoded.\n"
  "-i input  : input.  If - then stdin. (default stdin)\n"
  "-o output : output. If - then stdout. (default stdout)\n"
  "-h        : this help." ;

int main(int argc, char* argv[]) {
  char ch, in, *pt ;
  int n;
  int fdin=STDIN_FILENO, fdout=STDOUT_FILENO ;

  init_generators() ;
  while ((ch = getopt(argc, argv, "ghi:o:t:")) != -1)
    switch(ch) {
    case 'i' :
      if (strncmp(optarg, "-",1))
        if ((fdin = open(optarg, O_RDONLY, 0)) < 0)
          err_sys("hamming : Cannot open input file %s", optarg);
      break ;
    case 'o' :
      if (strncmp(optarg, "-",1))
        if ((fdout = open(optarg, O_WRONLY|O_CREAT|O_TRUNC, 0660)) < 0)
          err_sys("hamming : Cannot open output file %s", optarg);
      break ;
    case 'h' :
      printf("usage : %s\n\n%s\n", usage, help);
      return EXIT_SUCCESS;
      break;
    case 'g' :
      print_generator_matrix() ;
      return EXIT_SUCCESS ;
      break ;
    case 't':
      n = (int)strtol(optarg, &pt, 10);
      if (pt == optarg)
        err_quit("black : %s not a integer", optarg);
      in = (char) (abs(n) % 256) ;
      print_test(in) ;
      return EXIT_SUCCESS;
      break ;
    default :
      err_quit("usage : %s", usage) ;
      break;
    }
  hamming_stream(fdin, fdout);
  close(fdin);
  close(fdout);
  return EXIT_SUCCESS ;
}
