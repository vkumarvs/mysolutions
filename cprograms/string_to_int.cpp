#include <iostream>
#include <string>
#include <math.h>
using namespace std;

int stringToInt(string &str)
{
    int num = 0;
    int j = 0;
    cout<<"str lenghth is --"<<str.length()<<endl;
#if 0
    for(int i = (str.length() -1); i >= 0; i--)
    {
      num = num+(str.at(i)-'0')*pow(10, j);
      cout<<"number is ---"<<num<<endl;
      j++;
    }
#endif
    int i = 0;
    bool isNegative = false;
    if(str.at(i) == '-')
    {
      isNegative = true;
      i = 1;
    }
    while(i < str.length())
    {
      num = num *10;
      num+=(str.at(i)-'0');
      i++;
    }
    if(isNegative)
    {
      return num * (-1);
    }
    return num;
}

string integerToString(int num)
{
    string str;
    int n = 9;
    char c = (n%10 + '0');
    cout<<"char is --"<<c<<endl;
    return str;
}

int main()
{
    string str1 = "45678";
    string str2 = "-9876";
    cout<<"converted positive string number is ----"<<stringToInt(str1)<<endl;
    cout<<"converted negative string number is ***** "<<stringToInt(str2)<<endl;
    cout<<endl<<endl;
    cout<<"converted number to string is ***** "<<integerToString(stringToInt(str1))<<endl;
    return 0;
}
