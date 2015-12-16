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

void to_control(){
    for (int i=0; i < 4; ++i) {
      G[i] = 0 ;
      for (int j=0; j < 8; ++j)
        if (GMatrix[i][j]) G[i] |= (1 << (7-j)) ;
    }
}





int main(int argc, char*  argv[]) {









}
