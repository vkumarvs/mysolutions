#include <iostream>
#include <stdio.h>
#include <stdlib.h>
using namespace std;

#if 1
void normalPrint(int arr[][4], int row, int column)
{
    cout<<endl<<"normal print"<<endl;
    for(int x = 0; x < row; x++)
    {
      for(int j = 0; j < column; j++)
      {
        cout <<arr[x][j]<<"\t";
      }
      cout<<endl;
    }
}

void findMinInColmnMaxInRowElm(int arr[][4], int row, int column)
{
    bool isElemPresent = false;
    for(int x = 0; x < row; x++)
    {
      int max = arr[x][0];
      int max_column_index = 0;
      for(int i = 0; i < column; i++)
      {
        if(max < arr[x][i])
        {
          max = arr[x][i];
          max_column_index = i;
        }
      }
      int min = arr[x][max_column_index];
      /*check if element is minimum in column
       * if yes then print that element*/
      for(int k = 0; k < row; k++)
      {
        if(min > arr[k][max_column_index])
        {
          isElemPresent = false;
          break;
        }
        else
        {
          isElemPresent = true;
        }
      }
      if(isElemPresent)
      {
        cout<<"max element is -"<<max <<"   max row index -"<<x <<"  column index - " <<max_column_index <<endl;
      }
    }
}
void spiralPrint(int arr[][4], int row, int column)
{
   int numSubMatrix = 0;
    if(row == column) //it is a square matrix
    {
      numSubMatrix = row/2;
    }
    else
    {
      numSubMatrix = row/2 + 1;
    }
    cout<<endl<<"spiral print -- numSubMatrix "<<numSubMatrix<<endl;
    int startRow = 0, endRow = row;
    int startClm = 0, endColmn = column;
    for(int i = 0; i < numSubMatrix; i++)
    {
      startRow = startRow + i;
      endRow = endRow -i;

      startClm = startClm + i;
      endColmn = endColmn - i;
      /*print down direction*/
      for(int j = (startRow -0); j < endRow; j++)
      {
        if(j == (startRow -0))
        {
          for(int k = startClm; k < endColmn; k++)
            cout<<arr[j][k]<<"\t";
        }
        else
        {
          cout<<arr[j][endColmn - 1]<<"\t";
        }
      }

      /*print up direction*/
      for(int j = (endRow -1); j > (startRow); j--)
      {
        if(j == (endRow-1))
        {
          for(int k = (endColmn - 2); k >= startClm; k--)
            cout<<arr[j][k]<<"\t";
        }
        else
        {
          cout<<arr[j][startClm]<<"\t";
        }
      }
    }
}

#endif
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
int main()
{
    int arr[4][4];
    char ch;
    int count = 1;
    int row = 4, column = 4;

#if 0 //below code wont work
    int arr[row][column];
    print(arr);
#endif

    int min_max_elem_arr[][4] = {{6,5,36,8},
                                {7,4,30,9},
                                {32,33,34,30},
                                {40,38,37,10}
                               };

    printf("Operation options :c- create \n a - hardcoded inputs \n e- exit \n s - spiral print\n l- location of max row element and min row element \n p- print array elements \n");

    while(1)
    {
      printf("Enter option\n");
      scanf("%c", &ch);
      switch(ch)
      {
        case 'c':
        {
          printf("\n enter elements for 4*4 array\n");
          int elem = 0;
          for(int x = 0; x < row; x++)
          {
            for(int j = 0; j < column; j++)
            {
              scanf("elem", &elem);
              arr[x][j] = count++ ;
            }
          }
        }
        break;
        case 'a':
        break;
        case 'e':
        exit(0);
        break;
        case 's':
        {
          spiralPrint(min_max_elem_arr, 4, 4);
        }
        break;
        case 'l':
        {
          findMinInColmnMaxInRowElm(min_max_elem_arr,4,4);
        }
        break;
        case 'p':
        {
          normalPrint(min_max_elem_arr, 4, 4);
        }
        break;
        default:
        cout<<"invalid option"<<endl;
        break;
      }
    }
    return 0;
}



