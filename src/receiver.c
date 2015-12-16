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


int GMatrixControl[8][4];
char* synd[16];

void to_control(){
  for (int i=0; i < 4; ++i) {
    for (int j=0; j < 4; ++j)
      GMatrixControl[j][i] = GMatrix[i][j];
    for (int k = 4; k < 8; k++)
      GMatrixControl[k][i] = GMatrix[i][k];
  }
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
  for(int i = 0; i < 8; i++) {
    for (int j = 0; j < 4; j++) {
      printf (" %d ", GMatrixControl[i][j]);
    }
    printf("\n");
  }
}


int main() {

  to_control();
  to_print_Matrix();
  printf("\n\n\n");
  to_print_Control();



}
