#include <iostream>
#include <string>
#include <sstream>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
using namespace std;

int replaceExm(string &orgStr);
/*
 * problem: giving a simple string compress and decompress it
 * aaabbbccccddpqrrrrr -------> 3a3b4c2dpq5r
 *case 1: abcd ----1a1b1c1d --> not compressed --- so ignore count 1
 *case 2: if character repetation is say 600 times how u will decompress it
 *case 3: if string represent numeric value also how u will differentiate
 between actual count and numeric character
 *case 3: you might check no of compressed character before compressing it
 * */

string decompressStr(string &orgStr)
{
  stringstream ss, tmpCnt;
  int count = 0;
  cout<<orgStr.length()<<endl;
  for(int x = 0; x < orgStr.length(); x++)
  {
    switch(orgStr.at(x))
    {
      case '2':
      case '3':
      case '4':
      case '5':
      case '6':
      case '7':
      case '8':
      case '9':
      {
        int  p = ((orgStr.at(x)) - 48); 
        tmpCnt<<p;
        break;
      }
      default:
      {
        count = atoi(tmpCnt.str().c_str());
        if(count > 0)
        {
          char c = orgStr.at(x);
          for(int p = 0; p < count; p++)
          {
            ss<<c;
          }
        }
        else
        {
          ss<<orgStr.at(x);
        }
        count = 0;
        tmpCnt.clear();
        tmpCnt.str("");
      }
      break;
    }
  }

return ss.str();
}

string compressStr(string &orgStr)
{
    stringstream ss; //equivalanet to string buffer
    char last = orgStr.at(0);
    int count = 0;
    for(int x =0; x < orgStr.length(); x++)
    {
      if(last == orgStr.at(x))
      {
        count++;
      }
      else
      {
        if(count > 1)
        {
          ss<<count<<last;
        }
        else //dont append count 1 as it wont add into compression
        {
          ss<<last;
        }
        last = orgStr.at(x);
        count = 1;
      }
    }
    if(count > 1)
    {
      ss<<count<<last;
    }
    else
    {
      ss<<last;
    }
    return ss.str();
}

int InPlacecompressStr(string &orgStr)
{
    stringstream ss; //equivalanet to string buffer
    char last = orgStr.at(0);
    int replaceIndex = 0;
    int count = 0;
    for(int x =0; x < orgStr.length(); x++)
    {
      if(last == orgStr.at(x))
      {
        count++;
      }
      else
      {
        if(count > 1)
        {
          ss<<count<<last;
        }
        else //dont append count 1 as it wont add into compression
        {
          ss<<last;
        }
        last = orgStr.at(x);
        count = 1;
        cout<<"before"<<replaceIndex<<endl;
        orgStr.replace(replaceIndex, ss.str().length(), ss.str());
        cout<<orgStr<<endl;
        replaceIndex+=ss.str().length();
        ss.clear();
        ss.str("");
      }
    }
    if(count > 1)
    {
      ss<<count<<last;
    }
    else
    {
      ss<<last;
    }
     orgStr.replace(replaceIndex, ss.str().length(), ss.str());
     cout<<orgStr<<endl;
     replaceIndex+=ss.str().length();
     cout<<"compressed string length "<<replaceIndex<<endl;

    return replaceIndex;
}

#include <algorithm>
bool anagram()
{
    
    //vector<char> str1 = "ABC";
    //vector<char> str2 = "BAC";
    string x = "ABCD";
    string y = "BDAC";
    vector<char> str1(x.begin(), x.end());
    vector<char> str2(y.begin(), y.end());
    sort(str1.begin(), str1.end());
    sort(str2.begin(), str2.end());

    char c = 'A';
    if(binary_search (str1.begin(), str2.end(), c))
    {
      cout<<c<<"is present"<<endl;
    }
    for(int x = 0 ; x < str1.size(); x++)
    {
      cout<<str1[x]<<"------"<<str2[x]<<endl;
      if(str1[x] != str2[x])
      {
        cout<<"strings are not anagram"<<endl;
          return false;
      }
    }

    cout<<"strings are anagram"<<endl;
      return true;
}

int main()
{
    string orgStr = "aaabbbccccddpqrrrrr";
    //string orgStr = "55555555555aaabbbccccddpqrrrrr";
    //string orgStr = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaabbbccccddpqrrrrr";
    //string orgStr = "aaabcdd";
    //string orgStr = "abcd";
    //cout<<"orgStr ---"<<orgStr<<endl;
    //orgStr.replace(orgStr.begin(), orgStr.begin() + 2, 1, 't');
  //  orgStr.replace(0, 2, 2, 't');
   // cout<<"orgStr ---"<<orgStr<<endl;
    // replaceExm(orgStr);
   //  InPlacecompressStr(orgStr);
     anagram();
#if 0
    string newStr = compressStr(orgStr);
    cout<<"compressedStr ---"<<newStr<<endl;
    if(!orgStr.compare(decompressStr(newStr)))
      cout<<"decompression is successful"<<endl;
     else
      cout<<"decompression failed "<<endl;
#endif
    return 0;
}

int replaceExm(string &orgStr)
{
std::string base="this is a test string.";
  std::string str2="n example";
  std::string str3="sample phrase";
  std::string str4="useful.";

  // replace signatures used in the same order as described above:

  // Using positions:                 0123456789*123456789*12345
  std::string str=base;           // "this is a test string."
  str.replace(9,5,str2);          // "this is an example string." (1)
  //std::cout << base<< '\n';
  //std::cout << str << '\n';
  str.replace(19,6,str3,7,6);     // "this is an example phrase." (2)
  str.replace(8,10,"just a");     // "this is just a phrase."     (3)
  str.replace(8,6,"a shorty",7);  // "this is a short phrase."    (4)
  str = base;
  std::cout << base<< '\n';
  //str.replace(22,1,3,'!');        // "this is a short phrase!!!"  (5)
  str.replace(0,1,1,'!');        // "this is a short phrase!!!"  (5)
  std::cout << str << '\n';

  // Using iterators:                                               0123456789*123456789*
  str.replace(str.begin(),str.end()-3,str3);                    // "sample phrase!!!"      (1)
  str.replace(str.begin(),str.begin()+6,"replace");             // "replace phrase!!!"     (3)
  str.replace(str.begin()+8,str.begin()+14,"is coolness",7);    // "replace is cool!!!"    (4)
  str.replace(str.begin()+12,str.end()-4,4,'o');                // "replace is cooool!!!"  (5)
  str.replace(str.begin()+11,str.end(),str4.begin(),str4.end());// "replace is useful."    (6)
  return 0;
}
