#include <iostream>
#include <string>
#include <vector>
#include <sstream>

using namespace std;

void printPermutation(vector<char> &in, string &out, bool *used, int length, int level)
{
    if(level == (length))
    {
      cout<<out<<endl;;
      return;
    }
    for(int i = 0; i < length; i++)
    {
      if(used[i])
      {
        continue;
      }
      out.push_back(in[i]);
      used[i] = true;
      printPermutation(in, out, used, length, (level + 1));
      used[i] = false;
      out.erase((out.end() - 1)); //clear();
    }
}

int main()
{
    char ch[] = {'A','B','C','D'}; 
    vector<char> arr(ch, (ch + (sizeof(ch)/sizeof(char))));
    for(int i = 0; i < arr.size(); i++)
    {
      cout<<arr[i]<<"\t";
    }

    cout<<endl;

    bool *used = new bool(arr.size());
    for(int x = 0; x < arr.size(); x++)
          used[x] = false;
    //stringstream out; 
    string out; 
    printPermutation(arr, out, used, arr.size(), 0);  
    cout <<endl;
    return 0;
}
