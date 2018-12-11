#if 0
#include<stdio.h>
void print(int arr[4][4], int s1, int s2) {
    int i, j;
    printf("\n");
    for(i = 0; i<s1; i++) {
        for(j = 0; j<s2; j++) {
 //           printf("%d, ", *((arr+i)+j));
        }
    }
    printf("\n");
}

int main() {
    int a[4][4] = {{0}};
    print(a,4,4);
}
#endif

#include <iostream>

void print(int a[][4]){
    for (int i = 0; i <4; i++){
        for (int j = 0; j < 4; j++){
            //accessing as 4x4 array
            std::cout << a[i][j] <<std::endl;        

            //accessing corresponding to the physical layout in memory
            std::cout <<  *(*(a)+ i*4 + j) << std::endl;  

        }
    }
}

int main(){
    int a[4][4];

    //populating the array with the corresponding indices from 0 to 15
    int m = 0;
    for (int i = 0; i<4; i++){
        for (int j= 0; j < 4; j++){
            a[i][j] =  m;
            m++;
        }
    }
    print(a);
}
