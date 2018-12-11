#include <iostream>
#include <stdio.h>
#include<math.h>
using namespace std;

/*idea to generate all subsets or powersets is that for n elements total subset
 * will be 2^n 
 * so for x= {A,B,C} total subset would be 2^3 = 8
 * 111 = {A,B,C} = 7
 * 110 = {A,B}   = 6
 * 101 = {A,C}   = 5
 * 100 = {A}     = 4
 * 011 = {B,C}   = 3 
 * 010 = {B}     = 2
 * 001 = {C}     = 1
 * 000 = {}      = 0
 *
 * Other solution would be a recursive one
 * Time complexity will be O(2^n), we can not do better than this
 * */

//void print_subsets(int numbers[], int num_of_numbers){
void print_subsets(char numbers[], int num_of_numbers) {
  int pow_of_num = pow(2.0, num_of_numbers);
  printf("power of num %u\n", pow_of_num);
  int i;
  for (i = 1; i < pow_of_num; ++i) {
    int j = i;
    printf("{");
    int digit = 0;
    while (j != 0) {
      if (j % 2) {
        printf("%c,", numbers[digit]);//cout << numbers[digit] << endl;
      }
      ++digit;
      j = j>>1; //= j/2; // This can also be done by bitwise operation
    }
    printf("} \n");
  }
  printf("\n");
}

void print_subsets_test_drive() {
    //int numbers[] = {1, 2, 3, 4, 5};
    //int numbers[] = {1, 2, 3};
    //int numbers[] = {8, 9, 10};
    char numbers[] = {'A', 'B', 'C'};
    print_subsets(numbers, 3);
}

int main() {
    print_subsets_test_drive();
    return 0;
}

