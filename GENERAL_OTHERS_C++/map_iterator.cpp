// map_end.cpp
// compile with: /EHsc
#include <map>
#include <iostream>

int main( )
{
   using namespace std;
   map <int, int> m1;

   map <int, int> :: iterator m1_Iter;
   map <int, int> :: const_iterator m1_cIter;
   typedef pair <int, int> Int_Pair;

 //  m1.insert ( Int_Pair ( 1, 10 ) );
  // m1.insert ( Int_Pair ( 2, 20 ) );
   //m1.insert ( Int_Pair ( 3, 30 ) );

   m1_cIter = m1.end( );
   m1_cIter--;
#if 0
   cout << "The value of the last element of m1 is:\n" 
        << m1_cIter -> second << endl;
   
   m1_Iter = m1.end( );
   m1_Iter--;
   m1.erase ( m1_Iter );

   // The following 2 lines would err because the iterator is const
   // m1_cIter = m1.end ( );
   // m1_cIter--;
   // m1.erase ( m1_cIter );

   m1_cIter = m1.end( );
   m1_cIter--;
   cout << "The value of the last element of m1 is now:\n"
        << m1_cIter -> second << endl;
#endif
}

