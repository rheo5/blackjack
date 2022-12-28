//libraries
#include <iostream>
#include <ios>
#include <iomanip>
#include <string>
#include <ctime>
using namespace std;

//structure for card information
struct Card
{
    string suit; //heart, diamond, spade, clover
    string description; //rank and suit initial
    int rank; //1 to 11
    int value; //1 to 13

    Card() { //constructor
        suit="";
        description="";
        rank=0;
        value=0;
    }
};

//structure for card array
struct CardArray
{
    Card * cards; 
    int max; //maximum number of elements of the card array
    int used_cards; //number of unused cards

    CardArray(){ //constructor
        cards=nullptr;
        max=0;
        used_cards=0;
    }
};

//function declarations
void getNewDeck(CardArray & deck);
void printDeck(const CardArray & deck);
void shuffleDeck(CardArray& deck);
int blackjack(CardArray & deck);
void deal(CardArray & deck, string player_newcard, string dealer_newcard);
int blackjack_check(CardArray& deck, CardArray& user);
int p_d_blackjack (CardArray& deck, CardArray& player, CardArray& dealer);
string p_d_result (CardArray& deck, CardArray& player, CardArray& dealer);
int playGames(CardArray& deck);
void newShuffleDeck(CardArray&deck);
void advice(CardArray& deck, CardArray& player, CardArray& dealer);
void Ace_test_p(CardArray& deck, CardArray& player);
void Ace_test_d(CardArray& deck, CardArray& dealer);
void reveal_summary(CardArray& deck, CardArray&player, CardArray& dealer);
int valuesum(CardArray & user);

//constant integers to prevent magic numbers
const int NUMBEROFCARDS= 52;
const int BLACKJACK=21;
const int NUMBERINHAND= 12;

int main()
{
    CardArray thedeck; 
    getNewDeck(thedeck); //creating a deck
    printDeck(thedeck); //printing the cards in order
    
    cout<<"\n\nshuffled: "<<endl;
    //shuffle
    shuffleDeck(thedeck); //shuffling the deck
    printDeck(thedeck); //printing the shuffled deck

    cout<<"\n\n\n";

    playGames(thedeck); //play loop of blackjack games until user says no

    delete []thedeck.cards; //deleting dynamic memory
    return 0;
}

void getNewDeck(CardArray & deck){
    deck.max=NUMBEROFCARDS;
    deck.used_cards=NUMBEROFCARDS;
    deck.cards =new Card[NUMBEROFCARDS]; 

    //array for deck informations
    string suit_name[] = {"spades", "hearts", "diamonds", "clubs"}; 
    string suit_sign[] = {"S", "H", "D", "C"};
    string description_sign[] = {"A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K"};
    int rank_arr[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13};
    int value_arr[] = {11, 2, 3, 4, 5, 6, 7, 8 ,9, 10, 10, 10, 10};

    int k=0;
    for (int i=0; i<4; i++)
    {
        for(int j=0; j<13; j++)
        {
            //assigning informations to deck cards
            deck.cards[k].description=description_sign[j] + suit_sign[i]; 
            deck.cards[k].suit=suit_name[i]; 
            deck.cards[k].value=value_arr[j];
            deck.cards[k].rank=rank_arr[j];
            k++;
        }
    }
}

//printing card deck
void printDeck(const CardArray & deck)
{
    for (int i=0; i<NUMBEROFCARDS; i++)
    {
        if (i%13==0) //for every suit, create new line
        {
            cout<<endl;
        }
        cout<< setw(4)<<deck.cards[i].description;
    }
}

//shuffling card deck
void shuffleDeck(CardArray & deck)
{
    srand (time(NULL)); 
    
    for (int i=0; i<NUMBEROFCARDS; i++){
        int random_num=rand()%NUMBEROFCARDS;
        Card filler = deck.cards[random_num]; //for the random number's order in deck of cards
        deck.cards[random_num]=deck.cards[i];
        deck.cards[i]= filler; //will fill the random card in i as i increases to 52
    }
}

//deal one card to user
void dealCards(CardArray& deck, CardArray& user) 
{
    user.cards[user.used_cards] = deck.cards[deck.used_cards-1]; //assigning the user's card to the top card of the deck
    user.cards[user.used_cards].value = deck.cards[deck.used_cards-1].value; //assigning the value of the user's card to the top card of the deck's value
    user.cards[user.used_cards].rank = deck.cards[deck.used_cards-1].value; //assigning the rank of the user's card to the top card of the deck's rank
    
    user.used_cards++; //moves to the next user's card 
    deck.used_cards--; //moves to the next top card

    if (deck.used_cards==0) //if the deck runs out of card
    {
        newShuffleDeck(deck); //will create a fresh shuffled deck
    }
}

//create fresh shuffled deck
void newShuffleDeck(CardArray&deck){ 
    getNewDeck(deck); //gets new deck of cards
    shuffleDeck(deck); //shuffles them
    deck.used_cards=NUMBEROFCARDS; //assign the top card back to 52
}

//finding sum of the user's cards values
int valuesum(CardArray & user) 
{
    int user_valuesum =0;
    for (int i=0; i<NUMBERINHAND; i++)
    {
        user_valuesum = user_valuesum + user.cards[i].value;
    }
    return user_valuesum;
}

//checks if the sum of the value for the user is above, below, or is blackjack
int blackjack_check(CardArray& deck, CardArray& user)
{
    if (valuesum(user) <BLACKJACK)
    {
        return 1; //sum is less than 21
    }
    else if (valuesum(user) ==BLACKJACK)
    {
        return 0; //sum is 21; blackjack
    }
    else if (valuesum(user) >BLACKJACK)
    {
        return -1; //sum is greater than 21; bust
    }
    return 5;
}

//identifying if theres a win or loss or draw for the user's sums
int p_d_blackjack (CardArray& deck, CardArray& player, CardArray& dealer)
{
    if (blackjack_check(deck, player)==0 && blackjack_check(deck, dealer)==1) //player hits blackjack and dealer hand is less than 21
    {
        return 1;
    }

    else if (blackjack_check(deck, player)==1 && blackjack_check(deck, dealer)==-1) //player hand is less than 21 and dealer hand is bust
    {
        return 2;
    }

    else if (blackjack_check(deck, player)==0 && blackjack_check(deck, dealer)==0) //player and dealer hits blackjack
    {
        return 0;
    }

    else if (blackjack_check(deck, player)==-1 && blackjack_check(deck, dealer)!=-1) //player hand is bust and dealer hand is not bust
    { 
        return -1;
    }

    else if (blackjack_check(deck, player)==1 && blackjack_check(deck, dealer)==0) //player hand is less than 21 and dealer hand is blackjack
    {
        return -2;
    }

    else if(blackjack_check(deck, player)==1 && blackjack_check(deck, dealer) ==1) //both player and dealer has sum less than 21
    {
        return 3;
    }
    return 5;
}

//returns the win condition of when either or both users hit blackjack or bust
string p_d_result (CardArray& deck, CardArray& player, CardArray& dealer)
{
    if (p_d_blackjack(deck, player, dealer)==1) //player hits blackjack and dealer hand is less than 21
    {
        return "\nBlackjack. You win!";
    }

    else if (p_d_blackjack(deck, player, dealer)==2) //player hand is less than 21 and dealer hand is bust
    {
        return "\nDealer busts. You win!";
    }

    else if (p_d_blackjack(deck, player, dealer)==0) //player and dealer hits blackjack
    {
        return "\nIt's a draw!";
    }

    else if (p_d_blackjack(deck, player, dealer)==-1) //player hand is bust and dealer hand is not bust
    {
        return "\nBust. You lose!";
    }

    else if (p_d_blackjack(deck, player, dealer)==-2) //player hand is less than 21 and dealer hand is blackjack
    {
        return "\nDealer has blackjack. You lose!";
    }

    else if(p_d_blackjack(deck, player, dealer)==3) //both player and dealer has sum less than 21
    {
        return "";
    }

    return "ERROR";
}

//giving advice according to the basic strategies of blackjack
void advice(CardArray& deck, CardArray& player, CardArray& dealer){

    for (int i=7; i<=11; i++) //when the dealer's card is between 7 to 11
    {   
        if (dealer.cards[0].value == i && valuesum(player) >=17)
        {
            cout<<"You should not hit because your score is "<<valuesum(player)<<" and are likely to go bust."<<endl;
        }
        else if (dealer.cards[0].value == i && valuesum(player) <17)
        {
            cout<<"You should hit because your score is "<<valuesum(player)<<" and are not likely to go bust. The dealer has a high value card, so it is more likely that the card you hit is not high"<<endl;
        }
    }
    for (int i=1; i<=6; i++){ //when the dealer's card is less than 6
        if (dealer.cards[0].value == i && valuesum(player) >=12) 
        {
            cout<<"You should not hit because your score is "<<valuesum(player)<<" and are likely to go bust. The dealer has a low value card, so it is more likely that the card you hit is very high"<<endl;
        }
        else if (dealer.cards[0].value == i && valuesum(player) <12)
        {
            cout<<"You should hit because your score is "<<valuesum(player)<<" and are not likely to go bust."<<endl;
        }
    }
}

//Changing ace value for player's conditions in the assignment
void Ace_test_p(CardArray& deck, CardArray& player)
{
    for (int i=0; i<NUMBERINHAND; i++)
    {
        if (player.cards[i].value == 11 && blackjack_check(deck, player)==-1) //when the ace value is 11 and the player goes bust
        {
            player.cards[i].value = 1; //ace value will then be 1
        }
        if (player.cards[i].value ==1) //when the ace value is 1
        {
            if (valuesum(player) + 10 < 21) //when having ace value 11 results in a non-bust situation where the number is closer to blackjack
            {
                player.cards[i].value= 11; //ace value changes to 11
            }
        }
    }
}

//Changing ace value for dealer's conditions in the assignment
void Ace_test_d(CardArray& deck, CardArray& dealer)
{
    for (int i=0; i<NUMBERINHAND; i++)
    {
        if (dealer.cards[i].value == 11 && blackjack_check(deck, dealer)==-1) //when the ace value is 11 and the dealer goes bust
        {
            dealer.cards[i].value =1; //ace value will then be 1
        }

        if (dealer.cards[i].value==1) //when the ace value is 1
        {
            if (valuesum(dealer) + 10 >= 17 && valuesum(dealer) + 10<=BLACKJACK) //when having ace value as 11 results in a number between 10 and 21
            {
                dealer.cards[i].value=11; //ace value changes to 11
            }
        }
    }
}

//reveals player's hand, dealer's hand, player sum and dealer sum
void reveal_summary(CardArray & deck, CardArray&player, CardArray& dealer)
{
    cout<<"*Player*: ";
    for (int i=0; i<NUMBERINHAND; i++)
    {
        cout<<setw(5)<<player.cards[i].description;
    }
    cout<<endl;

    cout<<"+Dealer+: ";
    for (int i=0; i<NUMBERINHAND; i++)
    {
        cout<<setw(5)<<dealer.cards[i].description;
    }
    cout<<"\n\n";

    cout<<"player sum: "<<valuesum(player)<<endl;
    cout<<"dealer sum: "<<valuesum(dealer)<<endl;
}

//blackjack game
int blackjack(CardArray & deck)
{
    //creating player and dealer's hands
    CardArray player; 
    CardArray dealer; 
    player.used_cards=0;
    dealer.used_cards=0;

    player.cards= new Card[NUMBERINHAND]; 
    dealer.cards = new Card[NUMBERINHAND];

    //deal one card to player and dealer
    dealCards(deck, player);
    dealCards(deck, dealer);

    //prints player and dealer hand
    cout<<"Deal first card:"<<endl;
    cout<<"---------------------------"<<endl;
    cout<<"*Player*: "<<setw(5)<<player.cards[0].description<<endl;
    cout<<"+Dealer+: "<<setw(5)<<dealer.cards[0].description<<endl;
    cout<<"\n\n";

    //deals second card to player and dealer
    dealCards(deck, player);
    dealCards(deck, dealer);

    //prints player and dealer hand
    cout<<"Deal second card:"<<endl;
    cout<<"---------------------------"<<endl;
    cout<<"*Player*: "<<setw(5)<<player.cards[0].description<<setw(5)<<player.cards[1].description<<endl;
    cout<<"+Dealer+: "<<setw(5)<<dealer.cards[0].description<<setw(5)<<"??"<<endl;
    cout<<"\n";

    //tests for ace card value determination
    Ace_test_p(deck, player);
    Ace_test_d(deck, dealer);

    //if the winner or loser is determined through blackjack or bust, prints the win conditions and ends game
    if (p_d_blackjack(deck, player, dealer) < 0) //player loses
    { 
        reveal_summary(deck, player, dealer);
        cout<<p_d_result(deck, player, dealer)<<endl;
        return -1;
    }
    else if (p_d_blackjack(deck, player, dealer) == 0) //draw
    { 
        reveal_summary(deck, player, dealer);
        cout<<p_d_result(deck, player, dealer)<<endl;
        return 0;
    }
    else if (p_d_blackjack(deck, player, dealer) ==1 || p_d_blackjack(deck, player, dealer) ==2) //player wins
    {
        reveal_summary(deck, player, dealer);
        cout<<p_d_result(deck, player, dealer)<<endl;
        return 1;
    }

    //giving advice before the player chooses to hit or stand
    advice(deck, player, dealer);
    cout<<endl;

    //asking player to hit or stand
    char choice;
    cout<<"Dealing to player"<<endl;
    cout<<"---------------------------"<<endl;
    cout<<"Enter h to hit or s to stand:  ";
    cin>>choice;
    
    //handling input error
    while(cin.fail())
    { 
        cin.clear(); 
        cin.ignore(100000, '\n');
        cout << "Please enter a h or s: ";
        cin>>choice;
    }
    while(choice!='h' && choice!='s')
    {
        cout << "Please enter a h or s: ";
        cin>>choice;
    }

    //when player hits
    while (choice =='h')
    {   
        //deals one more card to player
        dealCards(deck, player);

        //prints player's hand
        cout<<"\n*Player*: ";
        for (int i=0; i<NUMBERINHAND; i++)
        {
            cout<<setw(5)<<player.cards[i].description;
        }
        cout<<"\n\n";

        //tests for ace card value determination
        Ace_test_p(deck, player);

        //if the winner or loser is determined through blackjack or bust
        if (p_d_blackjack(deck, player, dealer) < 0) //player loses
        { 
            reveal_summary(deck, player, dealer);
            cout<<p_d_result(deck, player, dealer)<<endl;
            return -1;
        }

        else if (p_d_blackjack(deck, player, dealer) == 0) //draw
        { 
            reveal_summary(deck, player, dealer);
            cout<<p_d_result(deck, player, dealer)<<endl;
            return 0;
        }

        else if (p_d_blackjack(deck, player, dealer) ==1 || p_d_blackjack(deck, player, dealer) ==2) //player wins
        {
            reveal_summary(deck, player, dealer);
            cout<<p_d_result(deck, player, dealer)<<endl;
            return 1;
        }

        //giving advice before the player chooses to hit or stand
        advice(deck, player, dealer);
        cout<<endl; 

        //asks player to hit or stand again
        cout<<"Enter h to hit or s to stand:  ";
        cin>>choice;
        //handling input error
        while(cin.fail())
        { 
            cin.clear(); 
            cin.ignore(100000, '\n');
            cout << "Please enter a h or s: ";
            cin>>choice;
        }
        while(choice!='h' && choice!='s')
        {
            cout << "Please enter a h or s: ";
            cin>>choice;
        }
    }

    //when player chooses to stand
    if (choice =='s')
    {
        //dealing to dealer

        //ace value determination
        Ace_test_d(deck, dealer);

        cout<<"\n\nDealing to dealer"<<endl;
        cout<<"---------------------------"<<endl;

        //when dealer's hand is less than 17
        while(valuesum(dealer)<17) 
        {
            cout<<"+Dealer+: ";
            for (int i=0; i<NUMBERINHAND; i++)
            {
                cout<<setw(5)<<dealer.cards[i].description;
            }
            cout<<endl;
            
            //deal one more card to dealer
            dealCards(deck, dealer);
            Ace_test_d(deck, dealer);
        }
        //if dealer's hand is greater than 17
        if (valuesum(dealer) >=17)
        {
            //reveals dealer's hand
            cout<<"+Dealer+: ";
            for (int i=0; i<NUMBERINHAND; i++)
            {
                cout<<setw(5)<<dealer.cards[i].description;
            }
            cout<<"\n\n";
        }

        //after dealing to player and dealer, determine who is the winner
        //if the winner or loser is determined through blackjack or bust
        Ace_test_p(deck, player);
        Ace_test_d(deck, dealer);

        if (p_d_blackjack(deck, player, dealer) < 0) //player loses
        { 
            reveal_summary(deck, player, dealer);
            cout<<p_d_result(deck, player, dealer)<<endl;
            return -1;
        }
        else if (p_d_blackjack(deck, player, dealer) == 0) //draw
        { 
            reveal_summary(deck, player, dealer);
            cout<<p_d_result(deck, player, dealer)<<endl;
            return 0;
        }
        else if (p_d_blackjack(deck, player, dealer) ==1 || p_d_blackjack(deck, player, dealer) ==2) //player wins
        {
            reveal_summary(deck, player, dealer);
            cout<<p_d_result(deck, player, dealer)<<endl;
            return 1;
        }

        //if winner is undetermined as noone hits blackjack or bust, determining who's hand is greater
        else if (valuesum(player)> valuesum(dealer)) //player hand is greater. player wins
        {
            reveal_summary(deck, player, dealer);
            cout<<"\nYou win!"<<endl;
            return 1;
        }
        else if (valuesum(player)== valuesum(dealer)) //both hands have same value. draw
        {
            reveal_summary(deck, player, dealer);
            cout<<"\nIt's a draw!"<<endl;
            return 0;
        }
        else if (valuesum(player) <valuesum(dealer)) //dealer hand is greater. player loses
        {
            reveal_summary(deck, player, dealer);
            cout<<"\nDealer wins!"<<endl;
            return -1;
        }
    }

    //deletes dynamic memory of player and dealer's card
    delete []player.cards;
    delete []dealer.cards;

    return 0;
}

//play multiple games
int playGames(CardArray& deck) 
{
    deck.used_cards=NUMBEROFCARDS;
    char play;
    play='y';
    int return_value =0;
    int games =0;
    int win =0;
    int loss =0;
    int draw =0;

    while (play=='y')
    {
        return_value = blackjack(deck); //plays blackjack
        
        //counting the number of games, wins, losses, draws
        games++; 

        if (return_value==-1) //counts number of losses
        {
            ++loss;
        }
        else if (return_value==0) //counts number of draws
        {
            ++draw;
        }
        else if (return_value==1) //counts number of wins
        {
            ++win;
        }

        //asks player if they want to play another game
        cout<<"\nDo you want to play another game? (y/n) "; 
        cin>>play;
        //handling input error
        while(cin.fail())
        { 
            cin.clear(); 
            cin.ignore(100000, '\n');
            cout << "Please enter a y or n: ";
            cin>>play;
        }
        while(play!='y' && play!='n')
        {
            cout << "Please enter a y or n: ";
            cin>>play;
        }
    }
    
    //if player decides to stop playing
    if (play=='n')
    {
        //prints game stats
        cout<<"\nThanks for playing, you played "<<games<<" games and your record was:"<<endl;
        cout<<setw(5)<<"wins: "<<win<<endl;
        cout<<setw(5)<<"draws: "<<draw<<endl;
        cout<<setw(5)<<"losses: "<<loss<<endl;
    }

    return 0;
}