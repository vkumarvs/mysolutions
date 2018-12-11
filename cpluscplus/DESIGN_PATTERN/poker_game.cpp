/*
 * http://www.fredosaurus.com/notes-cpp/misc/random-shuffle.html
http://www.dreamincode.net/forums/topic/139183-poker-game-using-oop/ --- very good link to give design idea
http://www.dreamincode.net/forums/topic/116864-how-to-make-a-poker-game-in-java/

The four French playing card suits used primarily in the English speaking world: spades (♠), hearts (♥), diamonds (♦) and clubs (♣).

There are 13 different values of cards: Ace, 2, 3, 4, 5, 6, 7, 8, 9, 10,
Jack, Queen, and King. Each of these appears in a deck 4 times: once each in
Hearts, Spades, Clubs, and Diamonds. Jacks, Queens, and Kings have pictures of
people in them, and thus are called face cards.

Poker Game basic:
Ante and blinds are both forced bets.
Ante is a bet that everyone in the game must make in order to stay at the table. It forces there to always be at least some payout in the pot, increasing the relative value of playing a hand to folding.
Blinds are bets that only one or two players are forced to make, usually a small blind and a big blind, the small blind at half the big blind. They are bet by the two players after the dealer, and thus rotate around as the dealer rotates. The blinds start betting off; bidding starts with the player after the big blind, and players must call or raise the blind bet to stay in the game

Poker Hand: The 5 cards held in a game of poker.
***********
Every player gets 2 face down cards in starting.
Four betting round:

a)PreFlop : player after big blind start , three possible options every player has in this round - Call/raise or fold
b)Flop round :: Three cards are displayed on poker table and every player can use face up card to make his 5 card hand. Now in this round small blind player will act first and will have options(Check/bet/fold) other player will have options like CALL/raise/fold
c)Turn round: One more card will be face up at centre of the table so player have option to make poker hand out of 6 card available
d)River round: One more card will be face up centre of the table so player have option to make poker hand out of 7 card. After betting any remaining player
has to face up their face down card and whoever has best poker hand will win the money pot.

SUIT: a suit is the collection of 13 cards of any type (spades/hear/club/diamond)
Poker hand rank (weak to strong ranking order):

1)High Card
2)One Pair
3)Two pair
4)Three of the kind -- Three card are the same rank in 5 card
5)Straight --  five consecutive card in hand .. ACE can be count as 1
6)Flush -- five card of the same suit 
7)FULL HOUSE : three of same kind and one pair
8)Four of same kind 
9)sraight and flush both
10)Royal flush

1)Most popular form is 6 player format:
2)Small blind
3)Big blind
4)Dealer

Betting player options and terminology
*************************************
5)CALL
6)CHECK : not to bet and it can be done by small blind
7)RAISE: raise the bet in multiple of minimum bet
8)FOLD: can choose to quit the game
9)BET:Can put money for betting

Elements:
1)Cards --- 4 type each 13 in number
2)Players---- multiple and minimum of 4
3)Deck -- collection of cards
4)Ranking system

1)Shuffle cards -- need some kind of random behavior for card positions in a deck

*/


