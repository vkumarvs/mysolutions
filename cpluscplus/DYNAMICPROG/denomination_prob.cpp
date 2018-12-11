#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <vector>
using namespace std;

uint32_t makeChange(uint32_t amount, uint32_t denom)
{
    int next_denom = 0;
#if 1
    switch(denom)
    {
      case 20:
      next_denom = 10;
      break;
      case 10:
      next_denom = 5;
      break;
      case 5:
      return 1;
    }
#else
    switch(denom)
    {
      case 20:
      next_denom = 5;
      break;
      case 5:
      return 1;
    }
#endif
    int ways = 0;
    for(int x = 0; x*denom <= amount ; x++)
    {
      if((amount-x*denom) > 0)
      printf("%u --- > (%u(%u) , %u(%u))\n", amount, x, denom, ((amount-x*denom)/next_denom), next_denom);
      else
      printf("%u --- > (%u(%u) , %u(%u))\n", amount, x, denom, 0, next_denom);
      ways+= makeChange(amount-x*denom, next_denom);
    }
    return ways;
}

int main()
{
    uint32_t amount, facevalue, noOfDifferentCoins, maxFaceVal = 0;
    vector<uint32_t> denomCoinArr;
    printf("Enter targeted amount to which change required\n");
    scanf("%u", &amount);

    printf("Enter denomination coin nubers \n");
    scanf("%u", &noOfDifferentCoins);
    for(int x = 0; x < noOfDifferentCoins; x++)
    {
      printf("x - %u -- total coins %u enter coin face value\n", x, noOfDifferentCoins);
      scanf("%u", &facevalue);
      if(facevalue > amount)
      {
        printf("wrong value of coin face value (> amount %u)\n", facevalue, amount);
        exit(0);
      }
      if(facevalue > maxFaceVal)
         maxFaceVal = facevalue;
      denomCoinArr.push_back(facevalue);
    }

    printf("no of ways to get amount(%u) \n", makeChange(amount, maxFaceVal));
}
