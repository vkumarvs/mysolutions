/*
 *Given array pick any n elements to maximize the sum with a condition that
 neighbors shouldn't be picked
 *
 * Given an array of positive integers, what's the most efficient algorithm to
 * find non-consecutive elements from this array which, when added together,
 * produce the maximum sum?
  http://stackoverflow.com/questions/4487438/maximum-sum-of-non-consecutive-elements
 * */
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
using namespace std;
int sum(int *arr,int size) {
        int *sum = (int *)malloc(sizeof(int) * size);
        int i;

        for(i=0;i<size;i++) {
                if(i==0) {
                        sum[0] = arr[0];
                }else if(i==1) {
                        sum[1] = max(sum[0],arr[1]);
                }else{
                        sum[i] = max(sum[i-2]+arr[i],sum[i-1]);
                }
        }    
        return sum[size-1];
}

/*
Algo description:
For every step we can form non-consecutive set by choosing either of two adjacent element
We will keep element which form big total
for example array [8,6,3,10,5,10]
for first step we start making our non-consecutive set by taking 8 or 6
so we start with 8 then 8,3 or 8,10 -- bigger one is 8,10
6,10 or 6,5
next step 6,10,10 or 8,10,10 or 8,3,5 -- bigger one is 8,10,10
*/
int sumOptimize(int *a,int size)
{
    int dp[2] = {a[0],a[1]};

    for(int i=2;i<size;i++)
    {
      int temp = dp[1];
      dp[1] = dp[0]+a[i];
      dp[0] = max(dp[0],temp);
    }

    return max(dp[0],dp[1]);
    
}

int main()
{
    int arr4[] = {8,16,3,10,15,10};
    //cout<<"max sum is"<<sum(arr,6)<<endl;
    cout<<"max sum is"<<sumOptimize(arr4,6)<<endl;
    int arr2[] = {8,11,6,4,12,16,5};
    cout<<"optimize max sum is --- "<<sumOptimize(arr2, 7)<<endl;
    int arr1[] = {1,2,3,4,5};
    cout<<"optimize max sum is----"<<sumOptimize(arr1, 5)<<endl;
    
    int arr[] = {1,2,4,1,2,3,5,3,1,2,3,4,5,2};
    cout<<"max sum is----"<<sum(arr, 14)<<endl;
    cout<<"optimize max sum is----"<<sumOptimize(arr, 14)<<endl;
    return 0;
}


