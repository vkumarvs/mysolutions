#include <iostream>
#include <iomanip>
using namespace std;

class DeckOfCards
{
public:
    DeckOfCards()
    {
        welcome();

        resetDeck();
        resetCardCounter();
        resetSuitCounter();
        resetStats();

        deckNumber = 1; //set the first card to be dealt
                        //should be incremented whenever a card is dealt
        //setDealersHand();
        //setMyHand();


        srand( time( 0 ) );
    }

    void resetAll()
    {
        resetDeck();
        resetCardCounter();
        resetSuitCounter();
        resetStats();

        deckNumber = 1;

    }

    void shuffle()
    {
        resetDeck();

        int row;
        int column;


        for ( int card = 1; card <= 52; card++ )
        {
            do
            {
                row = rand() % 4;
                column = rand() % 13;
            } while( deck[ row ][ column ] != 0 );


            deck[ row ][ column ] = card;
        }
    }

    void deal()
    {

        static const char *suit[ 4 ] = { "Hearts", "Diamonds", "Clubs", "Spades" };

        static const char *face[ 13 ] = { "Ace", "Deuce", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine", "Ten", "Jack", "Queen", "King" };


        for ( int card = 1; card <= 52; card++ )
        {

            for ( int row = 0; row <= 3; row++ )
            {

                for ( int column = 0; column <= 12; column++ )
                {

                    if ( deck[ row ][ column ] == card )
                    {
                        cout << setw( 5 ) << right << face[ column ]
                        << " of " << setw( 8 ) << left << suit[ row ]
                        << ( card % 2 == 0 ? '\n' : '\t'  );
                    }
                }
            }
        }
    }

    void dealHand()
    {
        cout << "\nDealing you 5 cards..." << endl;

        for(int card = 1; card <= 5; card++)
        {
            for(int r = 0; r < 4; ++r)
            {
                for(int c = 0; c < 13; ++c)
                {
                    if(deck[r][c] == deckNumber)
                    {
                        hand[card - 1][0] = c;
                        hand[card - 1][1] = r;
                        deckNumber++;
                    }
                }
            }
        }
    }

    void dealDealersHand()
    {
        cout << "\nDealing dealer 5 cards..." << endl;

        for(int card = 1; card <= 5; card++)
        {
            for(int r = 0; r < 4; ++r)
            {
                for(int c = 0; c < 13; ++c)
                {
                    if(deck[r][c] == deckNumber)
                    {
                        dealersHand[card-1][0] = c;
                        dealersHand[card-1][1] = r;
                        deckNumber++;
                    }
                }
            }
        }

    }

    void showHand()
    {
        static const char *suit[ 4 ] = { "Hearts", "Diamonds", "Clubs", "Spades" };
        static const char *face[ 13 ] = { "Ace", "Deuce", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine", "Ten", "Jack", "Queen", "King" };


        for ( int card = 1; card <= 5; card++ )
        {
            cout << setw( 5 ) << right << face[ hand[card-1][0] ] << " of " << setw( 8 ) << left << suit[ hand[card-1][1] ] << endl;
        }
    }

    void showYourHand()
    {

        static const char *suit[ 4 ] = { "Hearts", "Diamonds", "Clubs", "Spades" };
        static const char *face[ 13 ] = { "Ace", "Deuce", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine", "Ten", "Jack", "Queen", "King" };

        cout << endl;
        cout << "***YOUR HAND***" << endl;
        for ( int card = 1; card <= 5; card++ )
        {
            cout << card << " " << setw( 5 ) << right << face[ hand[card-1][0] ] << " of " << setw( 8 ) << left << suit[ hand[card-1][1] ] << endl;
        }

        cout << "\n-You have ";
        printHandType();

    }

    void showDealersHand()
    {

        static const char *suit[ 4 ] = { "Hearts", "Diamonds", "Clubs", "Spades" };
        static const char *face[ 13 ] = { "Ace", "Deuce", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine", "Ten", "Jack", "Queen", "King" };

        cout << "\n";
        cout << "***DEALER'S HAND***" << endl;
        for ( int card = 1; card <= 5; card++ )
        {
            cout << setw( 5 ) << right << face[ dealersHand[card-1][0] ] << " of " << setw( 8 ) << left << suit[ dealersHand[card-1][1] ] << endl;
        }

        cout << "\n-Dealer has ";
        printDealersHandType();
        cout << endl;

    }

    void printHandArray()
    {
        for(int r = 0; r < 5; ++r)
        {
            for(int c = 0; c < 2; ++c)
            {
                cout << setw(5) << hand[r][c];
            }
            cout << endl;
        }
    }

    void printDealersHandArray()
    {
        for(int r = 0; r < 5; ++r)
        {
            for(int c = 0; c < 2; ++c)
            {
                cout << setw(5) << dealersHand[r][c];
            }
            cout << endl;
        }
    }

    void printCardCounterArray()
    {
        for(int i = 0; i < 13; ++i)
            cout << setw(4) << cardCounter[i];
    }

    bool containsPair(int array[5][2])
    {
        //static const char *suit[ 4 ] = { "Hearts", "Diamonds", "Clubs", "Spades" };
        //static const char *face[ 13 ] = { "Ace", "Deuce", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine", "Ten", "Jack", "Queen", "King" };

        resetCardCounter();

        for(int i = 0; i < 5; ++i)
            cardCounter[array[i][0]]++;

        for(int i = 0; i < 13; ++i)
        {
            if(cardCounter[i] == 2)
            {

                return true;
            }
        }


        return false;
    }

    bool contains2Pairs(int array[5][2])
    {
        //static const char *suit[ 4 ] = { "Hearts", "Diamonds", "Clubs", "Spades" };
        //static const char *face[ 13 ] = { "Ace", "Deuce", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine", "Ten", "Jack", "Queen", "King" };

        resetCardCounter();

        for(int i = 0; i < 5; ++i)
            cardCounter[array[i][0]]++;

        int pairs = 0;

        for(int i = 0; i < 13; ++i)
        {
            if(cardCounter[i] == 2)
            {
                pairs++;
            }
        }

        if(pairs == 2)
            return true;
        else
            return false;

    }

    bool containsThreeOfAKind(int array[5][2])
    {
        resetCardCounter();

        for(int i = 0; i < 5; ++i)
            cardCounter[array[i][0]]++;

        for(int i = 0; i < 13; ++i)
        {
            if(cardCounter[i] == 3)
            {
                return true;
            }
        }

        return false;
    }

    bool dealersHandContainsThreeOfAKind()
    {
        return containsThreeOfAKind(dealersHand);
    }

    bool containsFourOfAKind(int array[5][2])
    {
        resetCardCounter();

        for(int i = 0; i < 5; ++i)
            cardCounter[array[i][0]]++;

        for(int i = 0; i < 13; ++i)
        {
            if(cardCounter[i] == 4)
            {
                return true;
            }
        }

        return false;

    }

    bool containsFullHouse(int array[5][2])
    {
        if(containsThreeOfAKind(array) && containsPair(array))
            return true;
        else
            return false;
    }

    bool containsFlush(int array[5][2])
    {
        resetSuitCounter();

        for(int i = 0; i < 5; ++i)
            suitCounter[array[i][1]]++;

        for(int i = 0; i < 4; ++i)
        {
            if(suitCounter[i] == 5)
            {
                return true;
            }
        }

        return false;
    }

    bool containsStraight(int array[5][2])
    {
        resetCardCounter();
        resetSuitCounter();

        int copy[5][2] = {0};

        for(int i = 0; i < 5; ++i)
        for(int j = 0; j < 2; ++j)
            copy[i][j] = array[i][j];

        bubbleAscending(copy);
        //lowStraightCheck(array);

        if(copy[4][0] == 0
        && copy[3][0] == 12
        && copy[2][0] == 11
        && copy[1][0] == 10
        && copy[0][0] == 9)
        {
            return true;
        }

        for(int i = 0; i < 5; ++i)
            cardCounter[copy[i][0]]++;

        for(int i = 0; i < 9; ++i)
        {
            if(cardCounter[i] == 1)
            {
                for(int j = 1; j <= 4; ++j)//check next four cards
                {
                    if(cardCounter[i + j] != 1)
                        return false;
                }

                return true;
            }
        }

        return false;
    }

    bool containsStraightFlush(int array[5][2])
    {
        if(containsStraight(array) && containsFlush(array))
            return true;
        else
            return false;
    }

    bool containsRoyalFlush(int array[5][2])
    {
        if(containsFlush(array) && containsTenJQKA(array))
            return true;

        return false;
    }

    bool containsTenJQKA(int array[5][2])
    {
        resetCardCounter();

        for(int i = 0; i < 5; ++i)
            cardCounter[array[i][0]]++;

        if(cardCounter[0] == 1)
        {
            for(int i = 9; i <= 12; ++i)
                if(cardCounter[i] != 1)
                    return false;
            return true;
        }

        return false;

    }

    void setMyHand()
    {
        //face
        hand[0][0] = 0;
        hand[1][0] = 12;
        hand[2][0] = 11;
        hand[3][0] = 10;
        hand[4][0] = 9;

        //suit
        hand[0][1] = 1;
        hand[1][1] = 1;
        hand[2][1] = 1;
        hand[3][1] = 1;
        hand[4][1] = 1;

    }

    void setDealersHand()
    {
        //face
        dealersHand[0][0] = 0;
        dealersHand[1][0] = 1;
        dealersHand[2][0] = 2;
        dealersHand[3][0] = 3;
        dealersHand[4][0] = 4;

        //suit
        dealersHand[0][1] = 1;
        dealersHand[1][1] = 0;
        dealersHand[2][1] = 0;
        dealersHand[3][1] = 0;
        dealersHand[4][1] = 0;


    }

    void resetCardCounter()
    {
        for(int i = 0; i < 13; ++i)
            cardCounter[i] = 0;

        for(int i = 0; i < 4; ++i)
            for(int j = 0; j < 13; j++)
                cardCounter2[i][j] = 0;
    }

    void resetSuitCounter()
    {
       for(int i = 0; i < 4; ++i)
            suitCounter[i] = 0;
    }

    void resetDeck()
    {
        for ( int row = 0; row <= 3; row++ )
        {
            for ( int column = 0; column <= 12; column++ )
            {
                deck[ row ][ column ] = 0;
            }
        }
    }

    void increment(int a)
    {
        switch(a)
        {
            case 1:
                pairs++;
                break;
            case 2:
                twoPairs++;
                break;
            case 3:
                threes++;
                break;
            case 4:
                fours++;
                break;
            case 6:
                flushes++;
                break;
            case 7:
                straights++;
                break;
            case 5:
                fullHouses++;
                break;
            case 8:
                straightFlushes++;
                break;
            case 9:
                royalFlushes++;
                break;

        }
    }

    void printResults(int total)
    {
        cout << fixed << setprecision(6);
        cout << setw(20) << "Pairs: " << setw(7) << pairs << setw(10) << static_cast<double>(pairs) / total * 100 << endl;
        cout << setw(20) << "Two Pairs: " << setw(7) << twoPairs << setw(10) << static_cast<double>(twoPairs) / total * 100 << endl;
        cout << setw(20) << "3 of a kinds: " << setw(7) << threes << setw(10) << static_cast<double>(threes) / total * 100 << endl;
        cout << setw(20) << "Straights: " << setw(7) << straights << setw(10) << static_cast<double>(straights) / total * 100 << endl;
        cout << setw(20) << "Flushes: " << setw(7) << flushes << setw(10) << static_cast<double>(flushes) / total * 100 << endl;
        cout << setw(20) << "Full Houses: " << setw(7) << fullHouses << setw(10) << static_cast<double>(fullHouses) / total * 100 << endl;
        cout << setw(20) << "4 of a kinds: " << setw(7) << fours << setw(10) << static_cast<double>(fours) / total * 100 << endl;
        cout << setw(20) << "Straight Flushes: " << setw(7) << straightFlushes << setw(10) << static_cast<double>(straightFlushes) / total * 100 << endl;
        cout << setw(20) << "Royal Flushes: " << setw(7) << royalFlushes << setw(10) << static_cast<double>(royalFlushes) / total * 100 << endl;
    }

    void resetStats()
    {
        pairs = 0;
        twoPairs = 0;
        threes = 0;
        fours = 0;
        flushes = 0;
        straights = 0;
        fullHouses = 0;
        straightFlushes = 0;
        royalFlushes = 0;
    }

    int findHandValue(int h[5][2])
    {
        if(containsRoyalFlush(h))
            return 9;
        else if(containsStraightFlush(h))
            return 8;
        else if(containsStraight(h))
            return 7;
        else if(containsFlush(h))
            return 6;
        else if(containsFullHouse(h))
            return 5;
        else if(containsFourOfAKind(h))
            return 4;
        else if(containsThreeOfAKind(h))
            return 3;
        else if(contains2Pairs(h))
            return 2;
        else if(containsPair(h))
            return 1;
        else
            return 0;

    }

    void tally(int array[5][2])
    {
        int hands;


        hands = 10000;

        for(int i = 0; i < hands; ++i)
        {
            shuffle();
            dealHand();

            if(containsRoyalFlush(array))
                increment(9);
            else if(containsStraightFlush(array))
                increment(8);
            else if(containsStraight(array))
                increment(7);
            else if(containsFlush(array))
                increment(6);
            else if(containsFullHouse(array))
                increment(5);
            else if(containsFourOfAKind(array))
                increment(4);
            else if(containsThreeOfAKind(array))
                increment(3);
            else if(contains2Pairs(array))
                increment(2);
            else if(containsPair(array))
                increment(1);

        }

        printResults(hands);
    }

    void compareHands()
    {

        handValue = findHandValue(hand);
        dealersHandValue = findHandValue(dealersHand);

        if(handValue > dealersHandValue)
            cout << "You win!" << endl;
        else if(handValue < dealersHandValue)
            cout << "You lose!" << endl;
        else
        {
            switch(handValue)
            {
                case 0:
                    findBestHighCard();
                    break;
                case 1:
                    findBestPair();
                    break;
                case 2:
                    findBestTwoPair();
                    break;
                case 3:
                    findBestThreeOfAKind();
                    break;
                case 4:
                    findBestFourOfAKind();
                    break;
                case 5:
                    findBestFullHouse();
                    break;
                case 6:
                    findBestHighCard();
                    break;
                case 7:
                    findBestStraight();
                    break;
                case 8:
                    findBestStraightFlush();
                    break;
                case 9:
                    cout << "It's a tie!" << endl;
                    break;
            }
        }
    }

    void findBestStraightFlush()
    {
        bubbleAscending(hand);
        bubbleAscending(dealersHand);

        lowStraightCheck(hand);
        lowStraightCheck(dealersHand);

        int winner = 0;

        for(int i = 4; i >= 0; --i)
        {
            if(hand[i][0] == 0 && dealersHand[i][0] != 0) //if checking ace
            {
                    winner = 1;
            }
            else if(dealersHand[i][0] == 0 && hand[i][0] != 0)  //if checking ace
            {
                    winner = 2;
            }
            else if(hand[i][0] > dealersHand[i][0])
            {
                i = -1;
                winner = 1;
            }
            else if(hand[i][0] < dealersHand[i][0])
            {
                i = -1;
                winner = 2;
            }
        }

        if(winner == 1)
            cout << "You win!" << endl;
        else if(winner == 2)
            cout << "Dealer wins!" << endl;
        else
            cout << "Its a tie!" << endl;
    }

    void findBestStraight()
    {
        bubbleAscending(hand);
        bubbleAscending(dealersHand);

        lowStraightCheck(hand);
        lowStraightCheck(dealersHand);

        int winner = 0;

        for(int i = 4; i >= 0; --i)
        {
            if(hand[i][0] == 0 && dealersHand[i][0] != 0) //if checking ace
            {
                    winner = 1;
            }
            else if(dealersHand[i][0] == 0 && hand[i][0] != 0)  //if checking ace
            {
                    winner = 2;
            }
            else if(hand[i][0] > dealersHand[i][0])
            {
                i = -1;
                winner = 1;
            }
            else if(hand[i][0] < dealersHand[i][0])
            {
                i = -1;
                winner = 2;
            }
        }

        if(winner == 1)
            cout << "You win!" << endl;
        else if(winner == 2)
            cout << "Dealer wins!" << endl;
        else
            cout << "Its a tie!" << endl;

    }

    void lowStraightCheck(int h[5][2])
    {
        if(h[4][0] == 0 && h[3][0] == 4)
        {
            int cardr, cardc;

            cardr = h[4][1];
            cardc = h[4][0];

            //cout << "Ace to move - Row: " << cardr << " Column: " << cardc << endl;

            for(int i = 4; i > 0; i--)
            {
                h[i][0] = h[i-1][0];
                h[i][1] = h[i-1][1];
            }

            h[0][0] = cardc;
            h[0][1] = cardr;

        }
    }

    void findBestFullHouse()
    {
        int myTriple, myDouble;
        int dealerTriple, dealerDouble;

        findTripleDouble(myTriple, myDouble, hand);
        findTripleDouble(dealerTriple, dealerDouble, dealersHand);

/*
        cout << "My TD: " << myTriple << " " << myDouble << endl;
        cout << "De TD: " << dealerTriple << " " << dealerDouble << endl;
*/

        if(myTriple > dealerTriple)
            cout << "You win!" << endl;
        else if(myTriple < dealerTriple)
            cout << "You lose!" << endl;
        else if(myTriple == dealerTriple)
        {
            if(myDouble > dealerDouble)
                cout << "You win!" << endl;
            else if(myDouble < dealerDouble)
                cout << "You lose!" << endl;
            else
                cout << "What are we doing here?" << endl;
        }

    }

    void findBestFourOfAKind()
    {
        int myFour = findFour(hand);
        int dealersFour = findFour(dealersHand);

        if(myFour > dealersFour)
            cout << "You win!" << endl;
        else if(myFour < dealersFour)
            cout << "You lose!" << endl;
        else
        {
            //if the same, compare remaining 2 cards
            //cout << "Same pair." << endl;

            bubbleAscending(hand);
            bubbleAscending(dealersHand);

            int winner = 0;

            for(int i = 4; i >= 0; --i)
            {
                //if current card is part of the pair, exit loop
                if(hand[i][0] == myFour || dealersHand[i][0] == myFour)
                {
                    //skip this iteration
                }
                else if(hand[i][0] == 0 && dealersHand[i][0] != 0) //if checking ace
                {
                        winner = 1;
                }
                else if(dealersHand[i][0] == 0 && hand[i][0] != 0)  //if checking ace
                {
                        winner = 2;
                }
                else if(hand[i][0] > dealersHand[i][0])
                {
                    i = -1;
                    winner = 1;
                }
                else if(hand[i][0] < dealersHand[i][0])
                {
                    i = -1;
                    winner = 2;
                }
            }

            if(winner == 1)
                cout << "You win!" << endl;
            else if(winner == 2)
                cout << "Dealer wins!" << endl;
            else
                cout << "Its a tie!" << endl;
        }

    }

    void findBestTwoPair()
    {
        int myPair1, myPair2;
        int dealersPair1, dealersPair2;

        findTwoPairs(myPair1, myPair2, hand);
        findTwoPairs(dealersPair1, dealersPair2, dealersHand);

        int myHighPair, dealersHighPair;
        int myLowPair, dealersLowPair;

        /*
        cout << "    My pairs: " << myPair1 << " " << myPair2 << endl;
        cout << "Dealer pairs: " << dealersPair1 << " " << dealersPair2 << endl;
        */

        if(myPair1 > myPair2)
        {
            myHighPair = myPair1;
            myLowPair = myPair2;
        }
        else
        {
            myHighPair = myPair2;
            myLowPair = myPair1;
        }

        if(dealersPair1 > dealersPair2)
        {
            dealersHighPair = dealersPair1;
            dealersLowPair = dealersPair2;
        }
        else
        {
            dealersHighPair = dealersPair2;
            dealersLowPair = dealersPair1;
        }

/*
        cout << "Dealer high: " << dealersHighPair << endl;
        cout << "My high: " << myHighPair << endl;
*/
        int winner = 0;

        if(myHighPair > dealersHighPair)
            cout << "You win!" << endl;
        else if(dealersHighPair > myHighPair)
            cout << "You lose!" << endl;
        else if(dealersHighPair == myHighPair)
        {
            if(myLowPair > dealersLowPair)
                cout << "You win!" << endl;
            else if(myLowPair < dealersLowPair)
                cout << "You lose!" << endl;
            else if(myLowPair == dealersLowPair)
            {
                bubbleAscending(hand);
                bubbleAscending(dealersHand);

                for(int i = 4; i >= 0; --i)
                {
                    //if current card is part of the pair, exit loop
                    if(hand[i][0] == myHighPair || dealersHand[i][0] == myHighPair
                    || hand[i][0] == myLowPair || dealersHand[i][0] == myLowPair)
                    {
                        //skip this iteration
                    }
                    else if(hand[i][0] == 0 && dealersHand[i][0] != 0) //if checking ace
                    {
                        winner = 1;
                    }
                    else if(dealersHand[i][0] == 0 && hand[i][0] != 0)  //if checking ace
                    {
                        winner = 2;
                    }
                    else if(hand[i][0] > dealersHand[i][0])
                    {
                        i = -1;
                        winner = 1;
                    }
                    else if(hand[i][0] < dealersHand[i][0])
                    {
                        i = -1;
                        winner = 2;
                    }
                }

            if(winner == 1)
                cout << "You win!" << endl;
            else if(winner == 2)
                cout << "Dealer wins!" << endl;
            else
                cout << "Its a tie!" << endl;
            }
        }
    }

    void findBestThreeOfAKind()
    {
        int myThree = findThree(hand);
        int dealersThree = findThree(dealersHand);

        if(myThree > dealersThree)
            cout << "You win!" << endl;
        else if(myThree < dealersThree)
            cout << "You lose!" << endl;
        else
        {
            //if the same, compare remaining 2 cards
            //cout << "Same pair." << endl;

            bubbleAscending(hand);
            bubbleAscending(dealersHand);

            int winner = 0;

            for(int i = 4; i >= 0; --i)
            {
                //if current card is part of the pair, exit loop
                if(hand[i][0] == myThree || dealersHand[i][0] == myThree)
                {
                    //skip this iteration
                }
                else if(hand[i][0] == 0 && dealersHand[i][0] != 0) //if checking ace
                {
                    winner = 1;
                }
                else if(dealersHand[i][0] == 0 && hand[i][0] != 0)  //if checking ace
                {
                    winner = 2;
                }
                else if(hand[i][0] > dealersHand[i][0])
                {
                    i = -1;
                    winner = 1;
                }
                else if(hand[i][0] < dealersHand[i][0])
                {
                    i = -1;
                    winner = 2;
                }
            }

            if(winner == 1)
                cout << "You win!" << endl;
            else if(winner == 2)
                cout << "Dealer wins!" << endl;
            else
                cout << "Its a tie!" << endl;
        }

    }

    void findTripleDouble(int &a, int &b, int h[5][2])
    {
        resetCardCounter();
        fillCardCounter(h);

        for(int i = 0; i < 13; i++)
        {
            if(cardCounter[i] == 3)
                a = i;
            else if(cardCounter[i] == 2)
                b = i;
        }

    }

    void findTwoPairs(int &a, int &b, int h[5][2])
    {
        resetCardCounter();
        fillCardCounter(h);

        int index = 0;

        for(int i = 0; i < 13; i++)
            if(cardCounter[i] == 2)
            {
                switch(index)
                {
                    case 0:
                        a = i;
                        index++;
                        break;
                    case 1:
                        b = i;
                        index++;
                        break;
                }
            }

    }

    void findBestPair()
    {
        //compare the pairs
        int myPair = findPair(hand);
        int dealersPair = findPair(dealersHand);

        if(myPair > dealersPair)
            cout << "You win!" << endl;
        else if(myPair < dealersPair)
            cout << "You lose!" << endl;
        else
        {
            //if the same, compare remaining 3 cards
            //cout << "Same pair." << endl;

            bubbleAscending(hand);
            bubbleAscending(dealersHand);

            int winner = 0;

            for(int i = 4; i >= 0; --i)
            {
                //if current card is part of the pair, exit loop
                if(hand[i][0] == myPair || dealersHand[i][0] == myPair)
                {
                    //skip this iteration
                }
                else if(hand[i][0] == 0 && dealersHand[i][0] != 0) //if checking ace
                {
                    winner = 1;
                }
                else if(dealersHand[i][0] == 0 && hand[i][0] != 0)  //if checking ace
                {
                    winner = 2;
                }
                else if(hand[i][0] > dealersHand[i][0])
                {
                    i = -1;
                    winner = 1;
                }
                else if(hand[i][0] < dealersHand[i][0])
                {
                    i = -1;
                    winner = 2;
                }
            }

            if(winner == 1)
                cout << "You win!" << endl;
            else if(winner == 2)
                cout << "Dealer wins!" << endl;
            else
                cout << "Its a tie!" << endl;
        }
    }

    int findFour(int h[5][2])
    {
        resetCardCounter();
        fillCardCounter(h);

        for(int i = 0; i < 13; i++)
            if(cardCounter[i] == 4)
                return i;
        return -1;

    }

    int findThree(int h[5][2])
    {
        resetCardCounter();
        fillCardCounter(h);

        for(int i = 0; i < 13; i++)
            if(cardCounter[i] == 3)
                return i;
        return -1;

    }

    int findPair(int h[5][2])
    {
        resetCardCounter();
        fillCardCounter(h);

        for(int i = 0; i < 13; i++)
            if(cardCounter[i] == 2)
                return i;
        return -1;
    }

    void findBestHighCard()
    {
        bubbleAscending(hand);
        bubbleAscending(dealersHand);

        int winner = 0;

        for(int i = 4; i >= 0; --i)
        {
            if(hand[i][0] == 0 && dealersHand[i][0] != 0) //if checking ace
            {
                winner = 1;
            }
            else if(dealersHand[i][0] == 0 && hand[i][0] != 0)  //if checking ace
            {
                winner = 2;
            }
            else if(hand[i][0] > dealersHand[i][0])
            {
                i = -1;
                winner = 1;
            }
            else if(hand[i][0] < dealersHand[i][0])
            {
                i = -1;
                winner = 2;
            }
        }

        if(winner == 1)
            cout << "You win!" << endl;
        else if(winner == 2)
            cout << "Dealer wins!" << endl;
        else
            cout << "Its a tie!" << endl;
    }

    void moveAces(int h[5][2])
    {
        int aces = 0;

        for(int i = 0; i < 5; ++i)
            if(h[i][0] == 0)
                aces++;

        //cout << aces << " ace(s)." << endl;

        while(aces != 0)
        {
            int cardr, cardc;

            cardr = h[0][1];
            cardc = h[0][0];

            //cout << "Ace to move - Row: " << cardr << " Column: " << cardc << endl;

            for(int i = 0; i < 4; i++)
            {
                h[i][0] = h[i+1][0];
                h[i][1] = h[i+1][1];
            }

            h[4][0] = cardc;
            h[4][1] = cardr;

            aces--;
        }
    }

    void bubbleAscending(int array[5][2])
    {
        int size = 5;

        int temp; //used for swapping
        int step = 0; //counts how many passes have been made
                      //after n passes the n largest values should be in place
                      //so there is no reason to check them again
        bool sorted = false;
        while(sorted != true)
        {
            sorted = true;

            for(int i = 0; i < size - 1 - step; i++)
            {
                if(array[i][0] > array[i+1][0])
                {
                    sorted = false;

                    temp = array[i][0];
                    array[i][0] = array[i+1][0];
                    array[i+1][0] = temp;

                    temp = array[i][1];
                    array[i][1] = array[i+1][1];
                    array[i+1][1] = temp;
                }
            }
            step++; //incremented after a complete pass
        }

        moveAces(array);
    }

    void printHandType()
    {
        switch(findHandValue(hand))
        {
            case 0:
                cout << "High card." << endl;
                break;
            case 1:
                cout << "Pair!" << endl;
                break;
            case 2:
                cout << "Two Pairs!" << endl;
                break;
            case 3:
                cout << "Three of a Kind!" << endl;
                break;
            case 4:
                cout << "Four of a Kind!" << endl;
                break;
            case 5:
                cout << "Full House!" << endl;
                break;
            case 6:
                cout << "Flush!" << endl;
                break;
            case 7:
                cout << "Straight!" << endl;
                break;
            case 8:
                cout << "Straight Flush!" << endl;
                break;
            case 9:
                cout << "Royal Flush!" << endl;
                break;

        }
    }

    void printDealersHandType()
    {
        switch(findHandValue(dealersHand))
        {
            case 0:
                cout << "High card." << endl;
                break;
            case 1:
                cout << "Pair!" << endl;
                break;
            case 2:
                cout << "Two Pairs!" << endl;
                break;
            case 3:
                cout << "Three of a Kind!" << endl;
                break;
            case 4:
                cout << "Four of a Kind!" << endl;
                break;
            case 5:
                cout << "Full House!" << endl;
                break;
            case 6:
                cout << "Flush!" << endl;
                break;
            case 7:
                cout << "Straight!" << endl;
                break;
            case 8:
                cout << "Straight Flush!" << endl;
                break;
            case 9:
                cout << "Royal Flush!" << endl;
                break;

        }
    }

    void evaluateDealersHand()
    {
        //cout << "\nEvaluating the dealer's hand..." << endl;
        evaluateHand(dealersHand);
    }

    void evaluateHand(int h[5][2])
    {
        int type = findHandValue(h);

        switch(type)
        {
            case 0:
                if(goodChanceForStraight(h))
                    checkStraight(h);
                else
                    checkHighCard(h);
                break;
            case 1:
                if(goodChanceForStraight(h))
                    checkStraight(h);
                else
                    checkPair(h);
                break;
            case 2:
                improveTwoPairs(h);
                break;
            case 3:
                improveThreeOfAKind(h);
                break;
            case 4:
                improveFourOfAKind(h);
                break;
            default:
                cout << "\nThe dealer thinks he can beat you with what he's got. :)" << endl;
                break;
        }
    }

    void flushIsClose(int h[5][2])
    {
        resetCardCounter();
        resetSuitCounter();

        for(int i = 0; i < 5; ++i)
            cardCounter2[h[i][1]][h[i][0]]++;

        for(int r = 0; r < 4; ++r)
        {
            for(int c = 0; c < 13; ++c)
            {
                if(cardCounter2[r][c] == 1)
                    suitCounter[r]++;
            }
        }

        for(int i = 0; i < 4; ++i)
        {
            if(suitCounter[i] == 3)
            {
                //discard 2 cards of other suit(s)

                int goodSuit = i; //suit to keep
                int card1r, card1c;
                int card2r, card2c;

                int index = 1;

                for(int j = 0; j < 2; ++j)
                {
                    for(int r = 0; r < 4; ++r)
                    {
                        for(int c = 0; c < 13; ++c)
                        {
                            if(r != goodSuit && cardCounter2[r][c] != 0)
                            {
                                switch(index)
                                {
                                    case 1:
                                        card1c = c;
                                        card1r = r;
                                        index++;
                                        break;
                                    case 2:
                                        card2c = c;
                                        card2r = r;
                                        index++;
                                        break;
                                }
                            }
                        }
                    }
                }

                changeTwoCards(h, card1r, card1c, card2r, card2c);

            }
            else if(suitCounter[i] == 4)
            {
                //discard 1 card of other suit
                int goodSuit = i; //suit to keep
                int card1r, card1c;

                for(int r = 0; r < 4; ++r)
                {
                    for(int c = 0; c < 13; ++c)
                    {
                        if(r != goodSuit && cardCounter2[r][c] != 0)
                        {
                            card1c = c;
                            card1r = r;
                        }
                    }
                }
/*
                cout << "\nSwapping the following card:" << endl;
                cout << "Card 1 - Row: " << card1r+1 << " Column: " << card1c+1 << endl;
*/
                changeOneCard(h, card1r, card1c);

                /*
                cardCounter2[card1r][card1c] = 0;

                for(int r = 0; r < 4; r++)
                {
                    for(int c = 0; c < 13; c++)
                    {
                        if(deck[r][c] == deckNumber)
                        {
                            cardCounter2[r][c] = 1;
                            deckNumber++;
                            r = 4;
                            c = 13;
                        }
                    }
                }

                int index = 0;

                for(int r = 0; r < 4; ++r)
                {
                    for(int c = 0; c < 13; ++c)
                    {
                        if(cardCounter2[r][c] == 1)
                        {
                            h[index][0] = c;
                            h[index][1] = r;
                            index++;
                        }
                    }
                }
                */
/*
                cout << "\nAfter swapping the odd suit." << endl;
                cout << "\nDealer's hand: ";
                printDealersHandType();
                showDealersHand();
*/
            }
        }
    }

    void changeThreeLowest(int h[5][2])
    {
        //cout << "Going for something better than this..." << endl;

        for(int i = 0; i < 5; ++i)
            cardCounter2[h[i][1]][h[i][0]]++;
/*
        for(int r = 0; r < 4; ++r)
        {
            for(int c = 0; c < 13; ++c)
            {
                cout << setw(4) << cardCounter2[r][c];
            }
            cout << endl;
        }
*/
        //replace lowest 3 cards
        int card1r, card1c;
        int card2r, card2c;
        int card3r, card3c;

        int index = 1;

        for(int c = 0; c < 13; ++c)
        {
            for(int r = 0; r < 4; ++r)
            {
                if(cardCounter2[r][c] == 1)
                {
                    if(index == 1)
                    {
                        card1c = c;
                        card1r = r;
                        index++;
                    }
                    else if(index == 2)
                    {
                        card2c = c;
                        card2r = r;
                        index++;
                    }
                    else if(index == 3)
                    {
                        card3c = c;
                        card3r = r;
                        index++;
                    }
                }
            }
        }
/*
        cout << "Going to replace the following 3 cards:" << endl;
        cout << "Card 1 - Row: " << card1r+1 << " Column: " << card1c+1 << endl;
        cout << "Card 2 - Row: " << card2r+1 << " Column: " << card2c+1 << endl;
        cout << "Card 3 - Row: " << card3r+1 << " Column: " << card3c+1 << endl;

        printDealersHandArray();
*/

        changeThreeCards(h, card1r, card1c, card2r, card2c, card3r, card3c);


    }

    void checkHighCard(int h[5][2])
    {
        resetSuitCounter();
        resetCardCounter();

        for(int r = 0; r < 5; ++r)
        {
            suitCounter[h[r][1]]++;
        }

        bool doFlushCheck = false;

        for(int r = 0; r < 4; ++r)
            if(suitCounter[r] >= 3)
                doFlushCheck = true;

        if(doFlushCheck)
        {
            flushIsClose(h);
        }
        else
        {
            changeThreeLowest(h);
        }





    }

    void checkPair(int h[5][2])
    {
        //cout << "Trying to improve the pair..." << endl;

        resetSuitCounter();
        resetCardCounter();

        fillCardCounter(h);
        fillCardCounter2(h);
/*
        for(int r = 0; r < 4; ++r)
        {
            for(int c = 0; c < 13; ++c)
            {
                cout << setw(4) << cardCounter2[r][c];
            }
            cout << endl;
        }
*/
        int card1r, card1c;
        int card2r, card2c;
        int card3r, card3c;

        int pairValue;

        for(int i = 0; i < 13; ++i)
            if(cardCounter[i] == 2)
                pairValue = i;

        for(int i = 0; i < 3; ++i)
        {
            for(int r = 0; r < 4; ++r)
            {
                for(int c = 0; c < 13; ++c)
                {
                    if(c != pairValue && cardCounter2[r][c] == 1)
                    {
                        if(i == 0)
                        {
                            card1r = r;
                            card1c = c;
                            i++;
                        }
                        else if(i == 1)
                        {
                            card2c = c;
                            card2r = r;
                            i++;
                        }
                        else if(i == 2)
                        {
                            card3c = c;
                            card3r = r;
                            i++;
                        }
                    }
                }
            }
        }
/*
        cout << "Going to replace the following 3 cards:" << endl;
        cout << "Card 1 - Row: " << card1r+1 << " Column: " << card1c+1 << endl;
        cout << "Card 2 - Row: " << card2r+1 << " Column: " << card2c+1 << endl;
        cout << "Card 3 - Row: " << card3r+1 << " Column: " << card3c+1 << endl;
*/
        changeThreeCards(h, card1r, card1c, card2r, card2c, card3r, card3c);


    }

    void improveThreeOfAKind(int h[5][2])
    {
        //cout << "Trying for more than three..." << endl;

        resetSuitCounter();
        resetCardCounter();

        fillCardCounter(h);
        fillCardCounter2(h);

        int card1r, card1c;
        int card2r, card2c;

        int tripleValue;

        for(int i = 0; i < 13; ++i)
            if(cardCounter[i] == 3)
                tripleValue = i;

//        cout << "triple: " << tripleValue << endl;


        //identify cards to be removed
        for(int i = 0; i < 2; ++i)
        {
            for(int r = 0; r < 4; ++r)
            {
                for(int c = 0; c < 13; ++c)
                {
                    if(c != tripleValue && cardCounter2[r][c] == 1)
                    {
                        if(i == 0)
                        {
                            card1r = r;
                            card1c = c;
                            i++;
                        }
                        else if(i == 1)
                        {
                            card2c = c;
                            card2r = r;
                            i++;
                        }
                    }
                }
            }
        }
/*
        cout << "Going to replace the following 2 cards:" << endl;
        cout << "Card 1 - Row: " << card1r+1 << " Column: " << card1c+1 << endl;
        cout << "Card 2 - Row: " << card2r+1 << " Column: " << card2c+1 << endl;
*/
        changeTwoCards(h, card1r, card1c, card2r, card2c);
    }

    void improveFourOfAKind(int h[5][2])
    {
        //cout << "Trying for a slight improvement..." << endl;

        resetSuitCounter();
        resetCardCounter();
        fillCardCounter(h);
        fillCardCounter2(h);

        int card1r, card1c;

        int quad;

        for(int i = 0; i < 13; ++i)
        {
            if(cardCounter[i] == 4)
                quad = i;
        }
/*
        cout << "Pair 1: " << pair1 << endl;
        cout << "Pair 2: " << pair2 << endl;
*/
        for(int r = 0; r < 4; ++r)
        {
            for(int c = 0; c < 13; ++c)
            {
                if(c != quad && cardCounter2[r][c] == 1)
                {
                        card1r = r;
                        card1c = c;
                }
            }
        }

        changeOneCard(h, card1r, card1c);
    }

    void test()
    {
        checkStraight(dealersHand);
    }

    bool goodChanceForStraight(int h[5][2])
    {
        resetSuitCounter();
        resetCardCounter();

        fillCardCounter(h);
        fillCardCounter2(h);

        for(int i = 0; i < 9; ++i)
        {
            if(cardCounter[i] == 1)
            {
                if(cardCounter[i+1] == 1
                && cardCounter[i+2] == 1
                && cardCounter[i+3] == 1)
                {
                    return true;
                }
            }
        }

        return false;
    }

    void checkStraight(int h[5][2])
    {
        resetSuitCounter();
        resetCardCounter();

        fillCardCounter(h);
        fillCardCounter2(h);

        //outputCardCounter2();

        int a, b; //limits of 4 card straight

        for(int i = 0; i < 9; ++i)
        {
            if(cardCounter[i] == 1)
            {
                if(cardCounter[i+1] == 1
                && cardCounter[i+2] == 1
                && cardCounter[i+3] == 1)
                {
                    a = i;
                    b = i + 3;

                    goForStraight(h, a, b);
                }
            }
        }
    }

    void goForStraight(int h[5][2], int a, int b)
    {


        int card1r, card1c;

        for(int i = 0; i < 4; ++i)
        {
            for(int j = 0; j < a; ++j)
            {
                if(cardCounter2[i][j] == 1)
                {
                    card1r = i;
                    card1c = j;
                }
            }

            for(int j = b+1; j < 13; ++j)
            {
                if(cardCounter2[i][j] == 1)
                {
                    card1r = i;
                    card1c = j;
                }
            }

        }
/*
        cout << "Going to replace the following card:" << endl;
        cout << "Card 1 - Row: " << card1r+1 << " Column: " << card1c+1 << endl;
*/
        changeOneCard(h, card1r, card1c);

        //outputCardCounter2();

    }

    void outputCardCounter()
    {
        for(int c = 0; c < 13; ++c)
            cout << setw(4) << cardCounter[c];
    }

    void outputCardCounter2()
    {
        for(int r = 0; r < 4; ++r)
        {
            for(int c = 0; c < 13; ++c)
            {
                cout << setw(4) << cardCounter2[r][c];
            }
            cout << endl;
        }
    }

    void improveTwoPairs(int h[5][2])
    {
        resetSuitCounter();
        resetCardCounter();
        fillCardCounter(h);
        fillCardCounter2(h);

        int card1r, card1c;

        int pair1 = -1, pair2 = -1;

        for(int i = 0; i < 13; ++i)
        {
            if(cardCounter[i] == 2 && pair1 == -1)
                pair1 = i;
            else if(cardCounter[i] == 2 && pair2 == -1)
                pair2 = i;
        }
/*
        cout << "Pair 1: " << pair1 << endl;
        cout << "Pair 2: " << pair2 << endl;
*/
        for(int r = 0; r < 4; ++r)
        {
            for(int c = 0; c < 13; ++c)
            {
                if(c != pair1 && c != pair2 && cardCounter2[r][c] == 1)
                {
                        card1r = r;
                        card1c = c;
                }
            }
        }

        changeOneCard(h, card1r, card1c);
    }

    void changeOneCard(int h[5][2], int card1r, int card1c)
    {
        cout << "\nDealer changes one card..." << endl;

        cardCounter2[card1r][card1c] = 0;

        for(int r = 0; r < 4; r++)
        {
            for(int c = 0; c < 13; c++)
            {
                if(deck[r][c] == deckNumber)
                {
                    cardCounter2[r][c] = 1;
                    deckNumber++;
                    r = 4;
                    c = 13;
                }
            }
        }

        int index = 0;

        for(int r = 0; r < 4; ++r)
        {
            for(int c = 0; c < 13; ++c)
            {
                if(cardCounter2[r][c] == 1)
                {
                    h[index][0] = c;
                    h[index][1] = r;
                    index++;
                }
            }
        }
    }

    void changeTwoCards(int h[5][2], int card1r, int card1c, int card2r, int card2c)
    {
        cout << "\nDealer changes two cards..." << endl;

        cardCounter2[card1r][card1c] = 0;
        cardCounter2[card2r][card2c] = 0;

        //assigns next 3 cards in the deck
        for(int card = 0; card < 2; ++card)
        {
            for(int r = 0; r < 4; r++)
            {
                for(int c = 0; c < 13; c++)
                {
                    if(deck[r][c] == deckNumber)
                    {
                        cardCounter2[r][c] = 1;
                        deckNumber++;
                        r = 4;
                        c = 13;
                    }
                }
            }
        }

        int index = 0;

        for(int r = 0; r < 4; ++r)
        {
            for(int c = 0; c < 13; ++c)
            {
                if(cardCounter2[r][c] == 1)
                {
                    h[index][0] = c;
                    h[index][1] = r;
                    index++;
                }
            }
        }
    }

    void changeThreeCards(int h[5][2], int card1r, int card1c, int card2r, int card2c, int card3r, int card3c)
    {
        cout << "\nDealer changes three cards..." << endl;

        cardCounter2[card1r][card1c] = 0;
        cardCounter2[card2r][card2c] = 0;
        cardCounter2[card3r][card3c] = 0;

        //assigns next 3 cards in the deck
        for(int card = 0; card < 3; ++card)
        {
            for(int r = 0; r < 4; r++)
            {
                for(int c = 0; c < 13; c++)
                {
                    if(deck[r][c] == deckNumber)
                    {
                        cardCounter2[r][c] = 1;
                        deckNumber++;
                        r = 4;
                        c = 13;
                    }
                }
            }
        }

        int index = 0;

        for(int r = 0; r < 4; ++r)
        {
            for(int c = 0; c < 13; ++c)
            {
                if(cardCounter2[r][c] == 1)
                {
                    h[index][0] = c;
                    h[index][1] = r;
                    index++;
                }
            }
        }
    }

    void fillCardCounter(int h[5][2])
    {
        for(int i = 0; i < 5; ++i)
            cardCounter[h[i][0]]++;
    }

    void fillCardCounter2(int h[5][2])
    {
        for(int i = 0; i < 5; ++i)
            cardCounter2[h[i][1]][h[i][0]]++;
    }

    void swapOneCard(int h[5][2], int card)
    {
        hand[card][0] = 0;
        hand[card][1] = 0;

        for(int r = 0; r < 4; r++)
        {
            for(int c = 0; c < 13; c++)
            {
                if(deck[r][c] == deckNumber)
                {
                    hand[card][0] = c;
                    hand[card][1] = r;
                    deckNumber++;
                    r = 4;
                    c = 13;
                }
            }
        }
    }

    void swapCards()
    {
        int num;
        int choice;

        int entries[5] = {0};
        int entriesIndex = 0;

        cout << "\nEnter number of cards to swap followed by the card numbers: ";
        cin >> num;

        while(num < 0 || num > 5)
        {
            cout << "Invalid input (" << num << "). Input again: ";
            cin >> num;
        }

        if(num == 0)
        {
            cout << "You chose not to swap any cards." << endl;
        }
        else if(num == 5)
        {
            cout << "You chose to swap ALL your cards." << endl;
            for(int i = 0; i < 5; ++i)
            {
                swapOneCard(hand, i);
            }
        }
        else
        {
            cout << "You chose to swap " << num << " cards." << endl;

            for(int i = 0; i < num; ++i)
            {
                cin >> choice;

                while(choice < 1 || choice > 5)
                {
                    cout << "Invalid input (" << choice << "). Input again: ";
                    cin >> choice;
                }

                while(searchEntries(entries, choice))
                {
                    cout << "You've already chosen that card (" << choice << ").\nChoose another: ";
                    cin >> choice;
                }

                entries[entriesIndex++] = choice; //store entry

                swapOneCard(hand, choice - 1);
                cout << "Changed card in position " << choice << endl;
            }
        }
    }

    bool searchEntries(int e[], int key)
    {
        for(int i = 0; i < 5; ++i)
        {
            if(e[i] == key)
                return true;
        }

        return false;
    }

    void help()
    {
        cout << endl;
        cout << "To play the game, first look at your cards and decided which" << endl;
        cout << "cards you would like to keep, and which cards you would like" << endl;
        cout << "to swap for new cards." << endl << endl;

        cout << "Once you have decided, input your selection like the example" << endl;
        cout << "below." << endl << endl;

        helpDemo();

        cout << endl << endl;
        cout << "First enter the number of cards you want to swap. Then enter" << endl;
        cout << "the positional number located next to each card. All numbers" << endl;
        cout << "must be separated by a space." << endl << endl;

        cout << "Example: 2 1 2" << endl;
        cout << "This will swap the 2 cards in positions 1 and 2." << endl << endl;

        cout << "Example: 3 3 4 5" << endl;
        cout << "This will swap the 3 cards in positions 3, 4, and 5." << endl << endl;

        cout << "There currently isn't any error checking code written as of" << endl;
        cout << "yet, so try not to mess up the input. If that happens, just" << endl;
        cout << "restart the game. :)" << endl << endl;

        system("pause");
    }

    void helpDemo()
    {
        shuffle();
        dealHand();
        showYourHand();
    }

    void runPokerGame()
    {
        char helpme;
        cout << "\nView help? (Y/N) ";
        cin >> helpme;

        if(helpme == 'y' || helpme == 'Y')
            help();

        cout << "\nTHE GAME HAS BEGUN!!!" << endl;

        resetAll();
        shuffle();
        dealHand();
        dealDealersHand();



        for(int i = 1; i < 4; ++i)
        {
            cout << endl;
            //cout <<
            cout << "*** ROUND " << i << " OF 3 ***" << endl;
            showYourHand();
            evaluateDealersHand();

            swapCards();
        }

        cout << "\nShow 'em!" << endl;
        showYourHand();
        showDealersHand();
        compareHands();

        cout << "\nTHANKS FOR PLAYING!" << endl << endl;

        cout << "Press any key to exit." << endl;

        system("pause");
    }

    void welcome()
    {
        cout << "*************************************************" << endl;
        cout << "**********-*-*-*-* JOE'S POKER *-*-*-*-**********" << endl;
        cout << "*************************************************" << endl << endl;

        cout << "Press CTRL + BREAK at any time to exit." << endl;
        cout << "BREAK should be located above Page Up" << endl;
    }

private:
    int deckNumber;

    int deck[ 4 ][ 13 ]; // represents deck of cards
    int hand[ 5 ][ 2 ]; //represents hand
    int dealersHand[ 5 ][ 2 ];
    int temp[5][2];

    int handValue;
    int dealersHandValue;

    int cardCounter[ 13 ]; //counts how many cards of each face a hand contains
    int suitCounter[4]; //counts how many cards of each suit a hand contains
    int cardCounter2[4][13]; //includes face and suit

    int pairs;
    int twoPairs;
    int threes;
    int fours;
    int flushes;
    int straights;
    int fullHouses;
    int straightFlushes;
    int royalFlushes;

}; // end class DeckOfCards


int main()
{

    DeckOfCards deckOfCards;
    deckOfCards.runPokerGame();

    return 0;
}

