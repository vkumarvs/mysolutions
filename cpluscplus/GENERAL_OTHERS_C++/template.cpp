/*
 *template <typename T> // A
void foo(T);

template <typename T> // B
void foo(T *);

template <>
void foo<int>(int *); // C  //specialized version of B

int _tmain(int argc, _TCHAR* argv[])
{
    int n=0;
    int *p = &n;
    foo(p);

    return 0;
}
 */

#include <iostream>
using namespace std;

// class template:
template <class T>
class mycontainer {
    T element;
  public:
    mycontainer (T arg) {element=arg;}
    T increase () {return ++element;}
};

// class template specialization:
template <>
class mycontainer <char> {
    char element;
  public:
    mycontainer (char arg) {element=arg;}
    char uppercase();
   /* {
      if ((element>='a')&&(element<='z'))
      element+='A'-'a';
      return element;
   }*/
};
//template <> 
char mycontainer <char>::uppercase() 
{
    {
      if ((element>='a')&&(element<='z'))
      element+='A'-'a';
      return element;
   }
}
int main () {
  mycontainer<int> myint (7);
  mycontainer<char> mychar ('j');
  cout << myint.increase() << endl;
  cout << mychar.uppercase() << endl;
  return 0;
}
