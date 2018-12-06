#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <ctime>

/*
    Update - Not enough random numbers!

Although the purpose of this article isn't to show how to write a casino-quality
shuffle algorithm, there is a really interesting problem lurking in all shuffles
based on using a random number generator in the way described.  Ricky Seltzer
emailed us with a simple message - 2^64 < 52!  If you consider using Knuth
Fisher-Yates to shuffle a deck of 52 cards then in principle every arrangement
of the deck, i.e. 52! sequences, should occur equally often - but wait! The
random number generator is usually only based on 64-bit arithmetic which means
it repeats after "only" 2^64, which means that it can't generate more than this
number of unique sequences and as 2^64 << 52! this approach is doomed to fail.  That is, the Knuth Fisher-Yates shuffle will miss out a lot of arrangements of the deck and will not produce a casino quality shuffle because of the limitations of the random number generator in use.  One possible approach is to reseed the generator at each shuffle, but this isn't easy if you have to keep track of the seed.  A much better solution would be to use a cryptographic-quality random number generator with a much larger period.  
This all goes to emphasize the fact that computer-generated random numbers are really pseudo random numbers and have additional properties that you need to keep in mind. Once again randomness is a tough and subtle subject.
2)
*/

using namespace std;


void Shuffle(vector<int> &cards)
{
    srand((unsigned) time(NULL));
    for (int i = 0; i < 51; i++) {
      //int j = rand() % (52 - i);
      int j = rand() % (52);
      //cout<<"value of j \n" <<j <<"\t";
      if ( i != j )
      {
        int temp = cards[i];
        cards[i] = cards[j];
        cards[j] = temp;
      }
      else
      {
        cout<<"generated duplicate value ---"<<i <<"-----"<<j<<endl;
      }
    }
    cout<<endl;
}

int main()
{
    unordered_map<string, string> cardDeck;
    vector<int> cards;
    for(int i = 1; i <= 52; i++)
    {
      cards.push_back(i);
    }
    Shuffle(cards);

    /*Initialize card suits value*/
    vector<string> cardSuit;
    cardSuit.push_back("Hearts");
    cardSuit.push_back("Diamonds");
    cardSuit.push_back("Clubs");
    cardSuit.push_back("Spades");

    /*Initialize card suits face value*/
    vector<string> cardFace;
    cardFace.push_back("Ace");
    cardFace.push_back("Two");
    cardFace.push_back("Three");
    cardFace.push_back("Four");
    cardFace.push_back("Five");
    cardFace.push_back("Six");
    cardFace.push_back("Seven");
    cardFace.push_back("Eight");
    cardFace.push_back("Nine");
    cardFace.push_back("Ten");
    cardFace.push_back("Jack");
    cardFace.push_back("Queen");
    cardFace.push_back("King");

    /*Make card dictionary based on suits*/
    for(int   i = 0; i<4; i++)
    {
      for(int j = 0; j < 13; j++)
      {
       // cout<<cardSuit[i]<<"-----"<<cardFace[j]<<endl;
        cardDeck[cardSuit[i]] = cardFace[j];
      }
    }

    unsigned n = cardDeck.bucket_count();

    cout << "cardDeck has " << n << " buckets.\n";

    for (unsigned i=0; i<n; ++i) {
      cout << "bucket #" << i << " contains: ";
      for (auto it = cardDeck.begin(i); it!=cardDeck.end(i); ++it)
        cout << "[" << it->first << ":" << it->second << "] ";
      cout << "\n";
    }

    for(auto x = cardDeck.begin(); x!= cardDeck.end(); x++)
    {
      cout<<x->first <<"-------- "<<x->second<<endl;
    }

    return 0;
}
