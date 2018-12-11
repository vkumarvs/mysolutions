#include <iostream>
#include <vector>
#include <stdio.h>
using namespace std;

int main()
{
    vector<int>         _selectVector;

    // Build out the probability vector
    // Top choice gets 'n' slots. Last choice gets 1 slot
    // So if there is 1 available primary, then it will get 100%
    // If the are 3, then the top choice will get 3 slots, second 2 slots, third 1 slots
    // for a total of 6 slots
    for (int i = 0; i < 4; i++) {
        for (int j = i; j < 4; j++) {
            _selectVector.push_back(i);
        }
    }
    printf("1:vector size is %zu\n", _selectVector.size());
    for (int i = 0; i < (int)_selectVector.size(); i++) {
        printf("value: %d \n", _selectVector[i]);
    }


    int choice;
    for (int i = 0; i < 6; i++) {
        choice = _selectVector[i];
        printf("selection: %d -- choice:%d\n", i, choice);
    }
#if 0
    int x = 0;
    outIdx.push(x);
    printf("vector size is %zu\n", outIdx.size());

    hostIdx = 1;
    for (std::vector<int>::iterator it = outIdx.begin() ; it != outIdx.end(); ++it)
    {
        printf("valuf of it %d \n", *it);
        if()
    }
#endif
    return 0;
}
