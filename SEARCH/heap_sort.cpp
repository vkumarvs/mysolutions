////////////////////////////////////////////////////////////////////
// A Simple implementation of Heap Sort (Second Attempt)          //
// Alexander Karagulamos                                          //
// (c) 2011                                                       //
//                                                                //
// Algorithms used in this C++ implementation:                    //
//                                                                //
// ALGORITHM <-> [CREATE-HEAP] -- {Max-Heap}                      //
// 1. Given an unsorted LIST of size n                            //
// 2. For the depth of the tree represented by LIST               //
//    3. We are currently at node i                               //
//    4. We find MAX(2i, 2i+1) child nodes                        //
//    5. If the index returned by MAX gives a child node greater  //
//       than its parent node i, swap them.                       //
//    6. Repeat steps 3 - 5 for the remaining i nodes in LIST     //
// 7. Repeat the steps above until the heap is completely formed  //
//                                                                //
// ALGORITHM <-> [UPDATE-HEAP] --{Max-Heap}                       //
// 1. Given the heapified LIST formed by performing CREATE-HEAP.  //
// 2. IF SIZE(LIST) equals 2 -- i.e. LIST contains 2 nodes        //
//    3. Swap node positions.                                     //
// 4. For n number of nodes in LIST                               //
//    5. Swap the first and last node positions so that the last  //
//       n node is in its sorted position.                        //
//    6. Peform CREATE-HEAP on the remaining n-1 nodes.           //
// 7. Repeat the steps above until the LIST is completely sorted. //
//                                                                //
// ALGORITHM <-> [HEAP-SORT]                                      //
// 1. Perform CREATE-HEAP.                                        //
// 2. Perform UPDATE-HEAP.                                        //
////////////////////////////////////////////////////////////////////
// A sample run of the program yields the result below:           //
////////////////////////////////////////////////////////////////////
// Heap Order: 123 45 67 7 34 1 11 1 6                            //
//                                                                //
// Sorted List: 1 1 6 7 11 34 45 67 123                           //
////////////////////////////////////////////////////////////////////

#include <iostream>
using namespace std;

inline int max(const int [], int, int);
inline void swap(int [], int, int);
void createHeap(int [], int);
void updateHeap(int [], int);
void heapSort(int [], int);

int main(){
  int list[] = {6, 11, 1, 45, 34, 123, 67, 1, 7}; // Unsorted list

  const int size = sizeof(list) / sizeof(list[0]);

  createHeap(list, size);

  cout << endl << "Heap Order: ";
  for(int i = 0; i < size; i++)
    cout << list[i] << " ";
  cout << endl;  

  heapSort(list, size);

  cout << endl << "Sorted List: ";
  for(int n = 0; n < size; n++)
    cout << list[n] << " ";
  cout << endl;
}

// returns the index of the max of list[x] and list[y]
inline int max(const int list[], int x, int y){
  return list[x] > list[y] ? x : y;
}

// exchanges the values of list[x] and list[y]
inline void swap(int list[], int x, int y){
  int temp = list[x];
  list[x] = list[y];
  list[y] = temp;
}

// builds a max-heap struture
void createHeap(int list[], int size){
  int largest = 0;

  for(int d = size/2; d > 0; d--) // depth of tree processed so far
    for(int n = 0; n < d; n++){ // currently at node n    
      largest = max(list, 2*n+1, 2*n+2); 
      // index of max(left, right) child nodes is in largest

      if(list[n] < list[largest]) // child greater than parent node?
        swap(list, n, largest); // swap them!
    } 
}

// reforms the heap after partial sorting of the list
void updateHeap(int list[], int size){
  if(size == 2) swap(list, 0, 1);

  for(int n = size-1; n > 1; n--){        
    swap(list, 0, n);          
    createHeap(list, n-1);
  }
}

// Completely sorts the list in increasing order
void heapSort(int list[], int size){  
  createHeap(list, size);    
  updateHeap(list, size);
}
