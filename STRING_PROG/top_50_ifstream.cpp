#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <map>
#include <vector>
#include <unordered_map>

using namespace std;

bool myfunction(const pair<string,int>& x, const pair<string,int>& y)
{
    return x.second > y.second;
}

bool myfunction1(const pair<int, string>& x, const pair<int, string>& y)
{
    return x.first > y.first;
}

bool myfunction2(const pair<int, string>& x, const pair<int, string>& y)
{
    return x.first > y.first;
}

struct val_lessthan : binary_function < pair<string,int>, pair<string,int>, bool >
{
    bool operator() (const pair<string,int>& x, const pair<string,int>& y) const
    {
      return x.second > y.second;
    }
}val_lt;

int main()
{
    string filename;
    int count = 0;
    unordered_map<string, int> strHash;
    vector<pair<int, string> > strHash1;
    // Get the filename.
    cout << "Enter the file you wish to have searched:\n";
    cin >> filename;
    // Open file.
    ifstream file(filename.c_str());
    // Read in all the words.
    string word;
    int heapCnt =0;
    while (file >> word)
    {
      // Remove punctuation.
      int index;
      int minFrequency = 1;
      while ((index = word.find_first_of(".,!?\\;-*+")) != string::npos)
        word.erase(index, 1);
      auto x = strHash.find(word);
      if(x != strHash.end())
      {
        strHash[word] = (strHash.at(word) + 1);
      }
      else
      {
        strHash[word] = 1;
      }
      if(strHash1.size() == 10)
      {
        /*if the requested size has reached then start comparing*/
        if(strHash1.front().first < strHash.at(word))
        {
          pop_heap (strHash1.begin(),strHash1.end(), myfunction1);
          strHash1.pop_back();
          pair<int, string> mypair(strHash.at(word), word);
          strHash1.push_back(mypair);
          push_heap (strHash1.begin(),strHash1.end(), myfunction1);
        }
      }
      else
      {
        /*Let the min heap fill till requested size of top items*/
        pair<int, string> mypair(strHash.at(word), word);
        strHash1.push_back(mypair);
        if(strHash1.size() == 10)
        {
          make_heap (strHash1.begin(),strHash1.end(), myfunction1);
        }
      }
      /**/
      count++;
    }

#if 0
    for(auto i = strHash.begin(); i != strHash.end(); i++)
    {
      //pair<string, int> mypair(i->first, i->second);
      pair<int, string> mypair(i->second, i->first);
      strHash1.push_back(mypair);
      //cout<<i->first<<"-------"<<i->second <<endl;
    }
#endif
    //sort(strHash1.begin(), strHash1.end(), val_lt);
    //sort(strHash1.begin(), strHash1.end(), myfunction2);
    //sort(strHash1.begin(), strHash1.end());

   // make_heap (strHash1.begin(),strHash1.end());

#if 1
    cout <<"total words in file ----" <<count <<"----"<<strHash1.size()<<endl;
    int p = 0;
    for(auto i = strHash1.begin() ; i != strHash1.end() ; i++)
    {
      if(p == 10)
        break;
      cout<<i->first<<"-------"<<i->second <<endl;
      p++;
      //cout<<i->second<<"-------"<<i->first<<endl;
    }
    cout<<"------********************************"<<endl;
    for(int i = 0; i < 10 ; i++)
    {
          cout<<strHash1.front().first<<"-----"<<strHash1.front().second<<endl;
       //   auto x = strHash1.front();
        //  cout<<strHash1.front()<<endl;
          pop_heap (strHash1.begin(),strHash1.end(), myfunction1);
          strHash1.pop_back();
      //cout<<i->second<<"-------"<<i->first<<endl;
    }
#endif
}


