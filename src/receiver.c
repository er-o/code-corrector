#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "../squelette_aide/myerror.h"

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
char char_courant[8];
char message[1024];


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

//Fonction qui décode 8 bits d'un message de hamming en 8,4
//Retourne 1 si message intacte ou corrige, et 0 si le message a trop d'erreurs
//Pour récupérer un char, il faut décoder 16 bits
//Exemple : a -> 0110 0001
//On l'encode en hamming : 1101 1000 1100 0110
//                            * ----    * ---- => on retrouve notre a à l'envers
//Il faut décoder en 2 fois : chaque fois 8 bits
//Les différentes étapes :
//on calcule le syndrome, s'il est différent de 0 alors on a une erreur
//on vérifie alors avec le bit de parité (celui avec une * en-dessous je pense)
//si une erreur on peux la corriger, alors on change le bit indiqué par l'index
//ensuite on récupére les 4 bits voulus
int hamming(char msgEncode[8]) {
  int octetErrone = 0;
  int syndrome[3] = {0,0,0};
  int index;
  //On calcule le syndrome
  for(int i = 0; i < 3; i++){
    for(int j = 0; j < 7; j++){
      syndrome[i] ^= msgEncode[j] & GMatrixControl[j][i];
      //printf("tmp[%d]: %d, leader : %d, GMatrixControl : %d\n", i, tmp[i], leader[k][j], GMatrixControl[j][i]);
    }
  }
  index = syndrome[0] + 2*syndrome[1] + 4*syndrome[2];

  int parite = 0;
  //On regarde le bit de parité
  for (int k = 0; k <= 6; k++) {
    parite ^= msgEncode[k];
  }

  //Si pas un index nulle
  if (index == 0) {
    if(msgEncode[7] == parite) {
      //printf("Le message est intacte\n");
    } else {
      printf("L'index est a 0, alors que le bit de parité n'est pas respecté. (on le corrige)\n");
      msgEncode[7] ^= 1;
      printf("/////////////////////////////////////////////////////////////////////////");
    }
  //Sinon on répare l'erreur
  } else {
    if(msgEncode[7] != parite) {
      printf("Le message a un bit mal transmis, il est corrigé\n");

      printf("bits recu :  ");
      for (int i = 0; i < 8; i++) {
        printf("%d", bits_recu[i]);
      }
      printf("   8-synd[index]  : %d   ", 8-synd[index]);

      msgEncode[7-synd[index]] ^= 1;



    } else {
      printf("Problème : Le message contient trop d'erreurs\n");
      octetErrone = 1;
    }
  }
  return octetErrone;
}

void lire_fichier(char* optarg) {
  int fdin = STDIN_FILENO;

  char test = '\0';

  if ((fdin = open(optarg, O_RDONLY, 0)) < 0)
    err_sys("transmit : Cannot open %s for input", optarg);

  int n, tmp, bitCourant, octetErrone;
  tmp = 0;
  octetErrone = 0;
  while ((n = read(fdin, io_buff, BUFFSIZE)) > 0) {
    printf("\n%d   :  ", tmp/2 +1);

    char_recu = io_buff[0];

    string_of_byte(bits_recu, char_recu);


    printf("bits recu :  ");
    for (int i = 0; i < 8; i++) {
      printf("%d", bits_recu[i]);
    }
    printf("\n");
    octetErrone |= hamming(bits_recu);
    printf("bits recu :  ");
    for (int i = 0; i < 8; i++) {
      printf("%d", bits_recu[i]);
    }
    printf("\n");

    if (tmp % 2 == 0) {
      for(int i = 4; i < 8; i++) {
        test |= bits_recu[i]<<i;

        bitCourant++;
      }
    }
    else {
      //On gére le cas ou le mesage recu est erroné
      if (octetErrone == 1) {
        test = 95;
        octetErrone = 0;
      } else {
        for(int i = 0; i < 4; i++) {
          test |= bits_recu[i+4]<<i;
        }
      }
      //printf ("char_courant : %c\n", char_courant);

      message[(tmp-1)/2] = test;

      test ='\0';
    }
    tmp += 1;

  }
  if (n < 0)
    err_sys("transmit : input error");

  printf("Message finale : %s\n", message);
}



int main() {

  to_control();
  to_print_Matrix();
  printf("\n\n\n");
  to_print_Control();
  printf("\n\n\n");
  syndrom();
  lire_fichier("messageOut.txt");


}
