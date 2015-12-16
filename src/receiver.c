#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>


#define BUFFSIZE 1024

int GMatrix[4][8] = {{1,1,0,1,1,0,0,0},
                     {0,1,1,1,0,1,0,0},
                     {1,0,1,1,0,0,1,0},
                     {1,1,1,0,0,0,0,1}} ;


int GMatrixControl[7][3];
char* synd[8];

int bin_to_int(char bin[]){
  int somme = 0;
  int len = strlen(bin);
  for (int i = len-1; i >= 0; i--) {
    if(bin[i] == 49) {
      somme |= (1<<(len -i-1));
    }
  }
  return somme;
}


void to_control(){
  for (int i=0; i < 3; ++i) {
    for (int j=0; j < 4; ++j)
      GMatrixControl[j][i] = GMatrix[i][j];
    for (int k = 4; k < 7; k++)
      GMatrixControl[k][i] = GMatrix[i][k];
  }
}
/*
void hamming(char c, char out[2]) {
  out[0] = out[1] = 0;
  for (int i=0; i < 4; ++i) {
    if (c & (1 << i))
      out[1] ^= GMatrix[i] ;
    if (c & (1 << (i+4)))
      out[0] ^= GMatrix[i] ;
  }
}*/

void string_of_byte(char s[], const char c) {
  for (int i = 0 ; i < 8; ++i)
    s[7-i] = (c & (1 << i)) ? '1' : '0' ;
  s[8] = '\0' ;
}


void syndrom(){
  int leader[8][8] = {{1,0,0,0,0,0,0,0},
  {0,1,0,0,0,0,0,0},
  {0,0,1,0,0,0,0,0},
  {0,0,0,1,0,0,0,0},
  {0,0,0,0,1,0,0,0},
  {0,0,0,0,0,1,0,0},
  {0,0,0,0,0,0,1,0},
  {0,0,0,0,0,0,0,1}};

  int tmp[3] = {0,0,0};

for(int k = 0; k < 8; k++)
  for(int i = 0; i < 3; i++){
    for(int j = 0; j < 7; j++){
      tmp[i] ^= leader[k][j] ^ GMatrixControl[j][i];
    }
    printf("%d\n", tmp[i]);
  }
/*
  char out[2] ;
  char buff[3][10] ;

  for(int i = 0; i < 16;i++){
    hamming(in,out);
    string_of_byte(buff[0], in);
    string_of_byte(buff[1], out[0]);
    string_of_byte(buff[2], out[1]);

    synd[i] = buff[1] + buff[2]
    printf("in = %s out = %s\n", buff[0], synd[i]) ;

  }*/
}



void to_print_Matrix() {
  printf("matrice génératrice\n");
  for(int i = 0; i < 4; i++) {
    for (int j = 0; j < 8; j++) {
      printf (" %d ", GMatrix[i][j]);
    }
    printf("\n");
  }
}


void to_print_Control() {
  printf("matrice de controle\n");
  for(int i = 0; i < 7; i++) {
    for (int j = 0; j < 3; j++) {
      printf (" %d ", GMatrixControl[i][j]);
    }
    printf("\n");
  }
}


int main() {
/*
  to_control();
  to_print_Matrix();
  printf("\n\n\n");
  to_print_Control();
  printf("\n\n\n");*/
  syndrom();




}
