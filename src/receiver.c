#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>


int GMatrix[4][8] = {{1,1,0,1,1,0,0,0},
{0,1,1,1,0,1,0,0},
{1,0,1,1,0,0,1,0},
{1,1,1,0,0,0,0,1}} ;


int GMatrixControl[7][3];
int synd[8];



#define BUFFSIZE 1
char io_buff[BUFFSIZE] ;
char char_recu;
char bits_recu[8];
char* message[1024];

int pow (int a, int b) {
  int mult = 1;
  for (int i = 1; i <= b; i++) {
    mult *= a;
  }

  return mult;
}


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
  for (int i = 0 ; i < 8; ++i) {
    s[7-i] = (c & (1 << i)) ? '1' : '0' ;
    s[7-i] -= 48;
  }
  s[8] = '\0' ;
}


void syndrom(){
  int leader[7][7] = {
  {0,0,0,0,0,0,1},
  {0,0,0,0,0,1,0},
  {0,0,0,0,1,0,0},
  {0,0,0,1,0,0,0},
  {0,0,1,0,0,0,0},
  {0,1,0,0,0,0,0},
  {1,0,0,0,0,0,0}};

  int tmp[3] = {0,0,0};
  int index;
  for(int k = 0; k < 7; k++){
  //  printf("===============\n");
    for(int i = 0; i < 3; i++){
      for(int j = 0; j < 7; j++){
        tmp[i] ^= leader[k][j] & GMatrixControl[j][i];
        //printf("tmp[%d]: %d, leader : %d, GMatrixControl : %d\n", i, tmp[i], leader[k][j], GMatrixControl[j][i]);
      }
    }
      //printf("Syndrome n°%d : [%d, %d, %d]\n", k, tmp[0], tmp[1], tmp[2]);
    index = tmp[0] + 2*tmp[1] + 4*tmp[2];
    for (int i = 0; i < 3; i++) {
      tmp[i] = 0;
    }
    synd[index] = k+1;
  }

  printf("index syndromes\n");
  for(int i = 0; i < 8; i++) {
    printf ("index : %d    bit erreur : %d\n", i, synd[i]);
  }
  printf("\n");
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


void lire_fichier(char* optarg) {
  int fdin = STDIN_FILENO;
  if ((fdin = open(optarg, O_RDONLY, 0)) < 0)
    err_sys("transmit : Cannot open %s for input", optarg);

  int n, tmp;
  while ((n = read(fdin, io_buff, BUFFSIZE)) > 0) {
    tmp = 0;
    char_recu = io_buff[0];
    string_of_byte(bits_recu, char_recu);
    hamming(bits_recu);
    for(int i = 7; i >= 0; i--) {
      tmp += bits_recu[i] * pow(2,  7-i);
    }
    char_recu = tmp;
    printf("char_corrige : %d\n", tmp);

  }
  if (n < 0)
    err_sys("transmit : input error");


  for(int i = 0; i < 8; i++) {
    printf("bit %d : %d\n", i, bits_recu[i]);
  }
  printf("char_corrige : %c\n", char_recu);
}

void hamming(char* msgEncode) {
  int syndrome[3] = {0,0,0};
  int index;
  for(int i = 0; i < 3; i++){
    for(int j = 0; j < 7; j++){
      syndrome[i] ^= msgEncode[j] & GMatrixControl[j][i];
      //printf("tmp[%d]: %d, leader : %d, GMatrixControl : %d\n", i, tmp[i], leader[k][j], GMatrixControl[j][i]);
    }
  }
  index = syndrome[0] + 2*syndrome[1] + 4*syndrome[2];
  if (index != 0) {
    msgEncode[8-synd[index]] ^= 1;
  }
  printf ("index : %d\n", index);
  printf ("synd[index] : %d\n", synd[index]);
  printf("synd[0] : %d synd[1] : %d synd[2] : %d\n", syndrome[0], syndrome[1], syndrome[2]);

}

int main() {

  to_control();
  to_print_Matrix();
  printf("\n\n\n");
  to_print_Control();
  printf("\n\n\n");
  syndrom();
  lire_fichier("message.txt");


}
