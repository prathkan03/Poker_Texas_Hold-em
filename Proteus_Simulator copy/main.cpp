#include "FEHLCD.h"
#include "FEHUtility.h"
#include "FEHImages.h"
#include "fstream"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

//card class
class card{
    private:
    int number;
    int suite;
    char cardSuite[9];
    public:
    //card initilization without parameters
    card(){
        this->number = -1;
        this->suite = -1;
        strncpy(this->cardSuite,"None",5);
    }

    //card initilization with parameters
    card(int cardValue, int suitein){
    this->number = cardValue;
    this->suite = suitein;
    switch(suitein){
        case 1:
            this->cardSuite[0]= 'S';
            this->cardSuite[1]= 'p';
            this->cardSuite[2]= 'a';
            this->cardSuite[3]= 'd';
            this->cardSuite[4]= 'e';
            this->cardSuite[5]= static_cast<char>(0);
            this->cardSuite[6]= static_cast<char>(0);
            this->cardSuite[7]= static_cast<char>(0);
        break;
        case 2:
            this->cardSuite[0]= 'H';
            this->cardSuite[1]= 'e';
            this->cardSuite[2]= 'a';
            this->cardSuite[3]= 'r';
            this->cardSuite[4]= 't';
            this->cardSuite[5]= static_cast<char>(0);
            this->cardSuite[6]= static_cast<char>(0);
            this->cardSuite[7]= static_cast<char>(0);
        break;
        case 3:
            this->cardSuite[0]= 'D';
            this->cardSuite[1]= 'i';
            this->cardSuite[2]= 'a';
            this->cardSuite[3]= 'm';
            this->cardSuite[4]= 'o';
            this->cardSuite[5]= 'n';
            this->cardSuite[6]= 'd';
            this->cardSuite[7]= static_cast<char>(0);
        break;
        default:
            this->cardSuite[0]= 'C';
            this->cardSuite[1]= 'l';
            this->cardSuite[2]= 'u';
            this->cardSuite[3]= 'b';
            this->cardSuite[4]= static_cast<char>(0);
            this->cardSuite[5]= static_cast<char>(0);
            this->cardSuite[6]= static_cast<char>(0);
            this->cardSuite[7]= static_cast<char>(0);
        break;
    }
    }
    int equals(card in){
        return (in.number==this->number) * (in.suite==this->suite);
    } 

    //returns the number or card name (e.g. ace) as a string
    void cardNameAsString(char *output);

    //copys one card to another without alising the card.
    void copyFrom(card in);

    //Returns the string representation of the card in line with what is needed to get the file of the card
    void toString(char *out);

    //returns the suite
    int getSuite(){
        return this->suite;
    }

    //returns the number
    int getNumber(){
        return this->number;
    }
};

//creates the 52 card deck.
class deck{
    private:
    card cardDeck[52];
    int atCard = 0;
    public:
    //construtor
    deck(){
        int count =0;
        for(int i = 1; i<5; i++){
            for(int j = 1; j<14; j++){
                card temp(j,i);
                this->cardDeck[count].copyFrom(temp);
                count++;
            }
        }
        /*card temp1(10,2);
        card temp2(13,2);
        card temp3(11,2);
        card temp4(8,2);
        /*card temp5(1,2);
        this->cardDeck[4].copyFrom(temp1);
        this->cardDeck[5].copyFrom(temp2);
        this->cardDeck[6].copyFrom(temp3);
        this->cardDeck[7].copyFrom(temp4);
        this->cardDeck[8].copyFrom(temp5);*/
    }
    //returns the top card, and increments to the next card. The card is not removed, but the index for the next to get is incremented
    card removeTopCard();

    //Semirandomly shuffles the deck
    void shuffleCards();

    //prints the deck to terminal
    void print();

    //gets the card at the specified index
    card getCardAt(int i){
        if(i>-1&&i<52){
            return this->cardDeck[i];
        }
        else{
            card null;
            return null;
        }
    }
};

//player selection menu prototype
int playerSelection(int *AIflag, int *TwoPFlag, int *initPlayerSelect);

//difficulty selection menu prototype
int difficultySelection(int *initDifficultySelect);

//game prototype
int gameIsRunning(int *botdec, int *oppbet,int *bet,int *lastbetp1,int *lastbetp2,int *winner,int *AIflag,int *TwoPFlag, int *p1wins,int *p2wins,int *PPlayer, int *POpponent, int *stge, int *cof, int *xpos, int *ypos, int *pturn, int *ccpx, int *ccpy, char *scorep, char *scoreo, int *round, int *betting, int *checking, int *tracking, int*total, FEHImage *card1,FEHImage *card2,FEHImage *card3,FEHImage *card4,FEHImage *card5,FEHImage *card6,FEHImage *card7,FEHImage *card8,FEHImage *card9, deck *gamedeck);

//ask user if they are sure to fold or not
int fold(int *round, int *stage, int *game, int *xpos, int *ypos, int *nextround);

//prompt bet amounts
int nextBet(int *bet,int *lastbetp1,int *lastbetp2,int *betting, int *game, int *xpos, int *ypos, int *menu, int *stage, int *PPlayer, int *POpponent, int *total, int *p1turn, int *p2turn, char *scorep, char *scoreo);

//asks user to pursue new round or quit game
int nextRound(int *nextround, int *stage, int *game, int *xpos, int *ypos, int *round);

//makes the players check based on the situation of the game
int check(int *check, int *stage, int *game, int *xpos, int *ypos, int *p1turn, int *p2turn, int *tracking);

//checks for if the win is a high card
int playerhigh(int player, deck *gamedeck);

//checks if the win is if all 5 cards are same symbol
int fiveOfTheSameSuite(int player, deck *gamedeck);

//checks if win is a royal flush
int isRoyalFlush(int *cardNumber,card a,card b,card c,card d,card e);

//checks if the win is a straight flush
int straightflush(int *cardNumber,card a,card b,card c,card d,card e);

//checks if the win is a four of a kind
int fourOfAKind(int *cardNumber,card a,card b,card c,card d,card e);

//checks if the win is a full house
int fullHouse(int *cardNumber1,int *cardNumber2,card a,card b,card c,card d,card e);

//same as flush
int allSameSuite(int *cardNumber,card a,card b,card c,card d,card e);

//checks if the win is a straight
int straight(int *cardNumber,card a,card b,card c,card d,card e);

//checks if the win is trips
int threeOfAKind(int *cardNumber,card a,card b,card c,card d,card e);

//checks if the win is a two-pair
int twoPair(int *cardNumber1,int *cardNumber2,card a,card b,card c,card d,card e);

//checks if win is a one-pair
int onePair(int *cardNumber,card a,card b,card c,card d,card e);

//checks to see which player won which round
int currentHandWinner(deck *gamedeck);

//stats of the players
int playerBestStats(int *cardNumber1,int *cardNumber2, card pc1, card pc2, card a,card b,card c,card d,card e);

//decision is a 1x2 vector. The first column is what action, and the 2nd is how much to raise.
//for column 1: 1 = raise, 2 = check, 3 = fold
//for column 2 will be a number of points to raise if the first column is a 1.
void aggresiveBot(int cardsOnField, deck *gamedeck, int *decision);

void conservitiveBot(int cardsOnField, deck *gamedeck, int *decision, int *reraised, int oppraised);
//returns the most common suite on the field
int suitesOnField(card a, card b, card c, card d, card e);

int main(){
    //Math Ints
    int a = 0;
    //Flags
    int qflag = 1, menuflag = 1, cflag = 0, iflag= 0, pflag = 0, sflag= 0, infscreen = 1, startscreen = 0, ptcalc = 1;
    //Bounding box
    int qbsx = 250, qbsy = 200, cbsx = 220, cbsy = 170, pbsx = 220, pbsy = 80, ibsx = 220, ibsy = 140, sbsx = 220, sbsy = 110, prevbx=60, prevby=200, nextbx=145, nextby=200;
    //Position variables
    int xpos=0,ypos=0;
    int timenow = time(NULL);
    float totalplaytime = 0;
    char p[52]={0};
    char ptnumber[32] = {0};
    std::ifstream infile;
    infile.open("time.txt");
    infile >> totalplaytime;
    infile.close();
    
    
    LCD.Clear(GREEN);

    FEHImage mainMenuCards;
    mainMenuCards.Open("cards/cardsFEH.pic");
    //checks if the menu should be displayed
    while(qflag){
    LCD.Clear(GREEN);
    if(menuflag){
        LCD.SetFontColor(BLACK);
        LCD.WriteAt("Poker",145,20);
        mainMenuCards.Draw(20,80);
        LCD.SetFontColor(BLACK);
        LCD.DrawRectangle(pbsx-5,pbsy-5,100,25);
        LCD.WriteAt("Play", pbsx,pbsy);
        LCD.DrawRectangle(sbsx-5,sbsy-5,100,25);
        LCD.WriteAt("Stats", sbsx,sbsy);
        LCD.DrawRectangle(ibsx-5,ibsy-5,100,25);
        LCD.WriteAt("Learn", ibsx,ibsy);
        LCD.DrawRectangle(cbsx-5,cbsy-5,100,25);
        LCD.WriteAt("Credits", cbsx,cbsy);
        LCD.DrawRectangle(qbsx-5,qbsy-5,60,25);
        LCD.WriteAt("Quit", qbsx,qbsy);
               
            while(LCD.Touch(&xpos, &ypos)){
                
                //Play button and opens up a new window for playing
                if(!(xpos<pbsx || ypos<pbsy || xpos>(pbsx+90) || ypos>(pbsy+15))){
                    LCD.SetFontColor(WHITE); 
                    LCD.WriteAt("Play", pbsx,pbsy);
                    LCD.DrawRectangle(pbsx-5,pbsy-5,100,25);
                    pflag = 1;
                    LCD.SetFontColor(BLACK);
                    LCD.WriteAt("Quit", qbsx,qbsy);
                    LCD.DrawRectangle(qbsx-5,qbsy-5,60,25);
                    LCD.WriteAt("Credits", cbsx,cbsy);
                    LCD.DrawRectangle(cbsx-5,cbsy-5,100,25);
                    LCD.WriteAt("Learn", ibsx,ibsy);
                    LCD.DrawRectangle(ibsx-5,ibsy-5,100,25);
                    LCD.WriteAt("Stats", sbsx,sbsy);
                    LCD.DrawRectangle(sbsx-5,sbsy-5,100,25);
                    sflag = 0;
                    iflag = 0;
                    cflag = 0;
                    qflag = 1;
                    menuflag=0;
                }
                //stats button and opens up a new window for the statistics
                else if(!(xpos<sbsx || ypos<sbsy || xpos>(sbsx+90) || ypos>(sbsy+15))){
                    LCD.SetFontColor(WHITE); 
                    LCD.WriteAt("Stats", sbsx,sbsy);
                    LCD.DrawRectangle(sbsx-5,sbsy-5,100,25);
                    sflag = 1;
                    LCD.SetFontColor(BLACK);
                    LCD.WriteAt("Quit", qbsx,qbsy);
                    LCD.DrawRectangle(qbsx-5,qbsy-5,60,25);
                    LCD.WriteAt("Credits", cbsx,cbsy);
                    LCD.DrawRectangle(cbsx-5,cbsy-5,100,25);
                    LCD.WriteAt("Learn", ibsx,ibsy);
                    LCD.DrawRectangle(ibsx-5,ibsy-5,100,25);
                    LCD.WriteAt("Play", pbsx,pbsy);
                    LCD.DrawRectangle(pbsx-5,pbsy-5,100,25);
                    pflag = 0;
                    iflag = 0;
                    cflag = 0;
                    qflag = 1;
                    menuflag = 0;
                }
                //instructions button and opens up a window that goes through the instructions
                else if(!(xpos<ibsx || ypos<ibsy || xpos>(ibsx+90) || ypos>(ibsy+15))){
                    LCD.SetFontColor(WHITE); 
                    LCD.WriteAt("Learn", ibsx,ibsy);
                    LCD.DrawRectangle(ibsx-5,ibsy-5,100,25);
                    iflag = 1;
                    LCD.SetFontColor(BLACK);
                    LCD.WriteAt("Quit", qbsx,qbsy);
                    LCD.DrawRectangle(qbsx-5,qbsy-5,60,25);
                    LCD.WriteAt("Credits", cbsx,cbsy);
                    LCD.DrawRectangle(cbsx-5,cbsy-5,100,25);
                    LCD.WriteAt("Stats", sbsx,sbsy);
                    LCD.DrawRectangle(sbsx-5,sbsy-5,100,25);
                    LCD.WriteAt("Play", pbsx,pbsy);
                    LCD.DrawRectangle(pbsx-5,pbsy-5,100,25);
                    pflag = 0;
                    sflag = 0;
                    cflag = 0;
                    qflag = 1;
                    menuflag = 0;
                }
                //credits button and opens screen with both names
                else if(!(xpos<cbsx || ypos<cbsy || xpos>(cbsx+90) || ypos>(cbsy+15))){
                    LCD.SetFontColor(WHITE); 
                    LCD.WriteAt("Credits", cbsx,cbsy);
                    LCD.DrawRectangle(cbsx-5,cbsy-5,100,25);
                    cflag = 1;
                    LCD.SetFontColor(BLACK);
                    LCD.WriteAt("Quit", qbsx,qbsy);
                    LCD.DrawRectangle(qbsx-5,qbsy-5,60,25);
                    LCD.WriteAt("Learn", ibsx,ibsy);
                    LCD.DrawRectangle(ibsx-5,ibsy-5,100,25);
                    LCD.WriteAt("Stats", sbsx,sbsy);
                    LCD.DrawRectangle(sbsx-5,sbsy-5,100,25);
                    LCD.WriteAt("Play", pbsx,pbsy);
                    LCD.DrawRectangle(pbsx-5,pbsy-5,100,25);
                    pflag = 0;
                    sflag = 0;
                    iflag = 0;
                    qflag = 1;
                    menuflag = 0;
                }
                //quit button and exits out of the whole software
                else if(!(xpos<qbsx || ypos<qbsy || xpos>(qbsx+45) || ypos>(qbsy+15))){
                    LCD.SetFontColor(WHITE); 
                    LCD.WriteAt("Quit", qbsx,qbsy);
                    LCD.DrawRectangle(qbsx-5,qbsy-5,60,25);
                    qflag = 0;
                    LCD.SetFontColor(BLACK);
                    LCD.WriteAt("Credits", cbsx,cbsy);
                    LCD.DrawRectangle(cbsx-5,cbsy-5,100,25);
                    LCD.WriteAt("Learn", ibsx,ibsy);
                    LCD.DrawRectangle(ibsx-5,ibsy-5,100,25);
                    LCD.WriteAt("Stats", sbsx,sbsy);
                    LCD.DrawRectangle(sbsx-5,sbsy-5,100,25);
                    LCD.WriteAt("Play", pbsx,pbsy);
                    LCD.DrawRectangle(pbsx-5,pbsy-5,100,25);
                    pflag = 0;
                    sflag = 0;
                    iflag = 0;
                    cflag = 0;
                    menuflag = 0;
                }
                else{
                    menuflag=1;
                }
        }
    }

//Play
if(pflag && !menuflag){
    menuflag=1;
    //flags for the player selection
    int AIflag = 0, TwoPFlag = 0, initPlayerSelect = 1, flag1 = 0;
    //flags for the difficulty selection
    int AIdflag = 0, initDifficultySelect = 0, round = 0, betting = 0, nextround = 0, checking = 0;
    //creates the 9 cards and the deck image
    deck playingdeck;
    FEHImage cardInHand1;
    FEHImage cardInHand2;
    FEHImage oppCardInHand1;
    FEHImage oppCardInHand2;
    FEHImage cardOnField1;
    FEHImage cardOnField2;
    FEHImage cardOnField3;
    FEHImage cardOnField4;
    FEHImage cardOnField5;
    FEHImage backOfCard;
    //Points
    int PPlayer = 100, POpponent = 100;
    int Player1Wins, Player2Wins;
    char scorep[4] = {0};
    char scoreo[4] = {0};
    char stats[4] = {0};
    //game flags
    int game = 1, stage = 0, pturn = 0, menu = 0, cof = 0, total = 0, p1turn=1, p2turn = 0, tracking = 0, winner = 0, oppbet = 0,  lastbetp1 = 0, lastbetp2 = 0,sumbetp1 = 0,sumbetp2=0, botDec[2] ={0}, bet=0, bothPlayed = 0, havePointsChanged = 0;
    //click positions
    int posx, posy, currentCardPositionx = 259, currentCardPositiony = 149;

    //assigns flag 1 to whether user chose bot or 2 players
    while(!flag1){
        flag1 = playerSelection(&AIflag, &TwoPFlag, &initPlayerSelect);
    }

    LCD.Clear(GREEN);
    AIdflag = TwoPFlag;
    //if bot was selected, asks user for easy or hard difficulty
    while(!AIdflag){
        AIdflag = difficultySelection(&initDifficultySelect);
    }
    if(TwoPFlag){
        AIdflag = 0;
    }

    LCD.Clear(GREEN);
    while(game){
        //if stage is not -1, then the game page will keep running
        if(stage!=-1){
            game = gameIsRunning(botDec, &oppbet,  &bet, &lastbetp1,&lastbetp2,&winner, &AIflag, &TwoPFlag,&Player1Wins, &Player2Wins, &PPlayer, &POpponent, &stage, &cof, &posx, &posy, &pturn, &currentCardPositionx, &currentCardPositiony, scorep, scoreo, &round, &betting, &checking, &tracking, &total, &cardInHand1,&cardInHand2, &oppCardInHand1,&oppCardInHand2,&cardOnField1,&cardOnField2,&cardOnField3,&cardOnField4,&cardOnField5,&playingdeck);
        }

        //updates the money that each player has as well as how much money is in the pot
        if(betting){
            LCD.Clear(GREEN);
            betting = nextBet(&bet, &lastbetp1,&lastbetp2, &betting, &game, &posx, &posy, &menu, &stage, &PPlayer, &POpponent,&total, &p1turn, &p2turn, scorep, scoreo);
        }
        if(bet){
            bothPlayed++;
            if(TwoPFlag){
            if(pturn){
                pturn = 0;
                p1turn = 1;
                p2turn = 0;
            }
            else{
                pturn = 1;
                p1turn = 0;
                p2turn = 1;
            }
            }
            if(bet==1){
                sumbetp1 = sumbetp1+lastbetp1+lastbetp2;
                PPlayer-=lastbetp2;
                total+=lastbetp2;
                lastbetp2 = 0;
                if(AIflag==1){
                    int flag = 0, cardq = 0;
                    if(oppbet){flag=1;}
                    if(cof==1){cardq=3;}
                    if(cof==2){cardq=4;}
                    if(cof==3){cardq=5;}
                    aggresiveBot(cardq, &playingdeck, botDec);
                    if(botDec[0]==1){
                        lastbetp2=botDec[1];
                    }
                    if(botDec[0]==2){
                        lastbetp2 = 0;
                    }
                    sumbetp2 = sumbetp2+lastbetp1+lastbetp2; 
                    POpponent-=lastbetp1;
                    total+=lastbetp1;
                    lastbetp1 = 0;
                    if(botDec[0]==3){
                        POpponent=0;
                    }
                }
                else if(AIflag==2){
                    int flag = 0, cardq = 0;
                    if(oppbet){flag=1;}
                    if(cof==1){cardq=3;}
                    if(cof==2){cardq=4;}
                    if(cof==3){cardq=5;}
                    conservitiveBot(cardq, &playingdeck, botDec, &flag, lastbetp1);
                    if(botDec[0]==1){
                        lastbetp2=botDec[1];
                    }
                    if(botDec[0]==2){
                        lastbetp2 = 0;
                    }
                    sumbetp2 = sumbetp2+lastbetp1+lastbetp2; 
                    POpponent-=lastbetp1;
                    total+=lastbetp1;
                    lastbetp1 = 0;
                    if(botDec[0]==3){
                        POpponent=0;
                    }
                }
            }
            else{
                sumbetp2 = sumbetp2+lastbetp1+lastbetp2; 
                POpponent-=lastbetp1;
                total+=lastbetp1;
                lastbetp1 = 0;
            }
            if(sumbetp1==sumbetp2&&bothPlayed>1){
                cof++;
                sumbetp1=0;
                sumbetp2=0;
                lastbetp2=0;
                lastbetp1=0;
                bothPlayed=0;
                pturn = 0;
                p1turn = 1;
                p2turn = 0;
            }
            bet=0;
            if(cof>3){
            if(!havePointsChanged){
                winner = currentHandWinner(&playingdeck);
                if(winner == 2){
                    POpponent = POpponent+total; 
                    havePointsChanged = 1;  
                }
                else if(winner==1){
                    PPlayer = PPlayer+total;
                    havePointsChanged = 1;  
                }
                else{
                    havePointsChanged = 1;
                    }
            }
            stage=4;
            }
            //updates the score of each players
            int temp = PPlayer;
            scorep[2] = static_cast<char>(48+temp%10);
            temp = temp/10;
            scorep[1] = static_cast<char>(48+temp%10);
            temp = temp/10;
            scorep[0] = static_cast<char>(48+temp%10);
            temp = POpponent;
            scoreo[2] = static_cast<char>(48+temp%10);
            temp = temp/10;
            scoreo[1] = static_cast<char>(48+temp%10);
            temp = temp/10;
            scoreo[0] = static_cast<char>(48+temp%10);
        }
        if(PPlayer==0){
            winner = 2;
            }
            if(POpponent==0){
            winner = 1;
            }
    }
    pflag = game;
    cardInHand1.Close();
    cardInHand2.Close();
    oppCardInHand1.Close();
    oppCardInHand2.Close();
    cardOnField1.Close();
    cardOnField2.Close();
    cardOnField3.Close();
    cardOnField4.Close();
    cardOnField5.Close();
    backOfCard.Close();
    }

    //Stats
    if(sflag && !menuflag){
        LCD.WriteAt("Stats",1,1);
        LCD.WriteAt("Playtime(minutes):",1,20);
        if(ptcalc){
            int temp = static_cast<int>(totalplaytime/60.0);
            int lengthofnumber = 0;
        while(temp>0){
            temp = temp/10;
            lengthofnumber++;
        }
            temp = static_cast<int>(totalplaytime);
        for(int i = 0; i<lengthofnumber; i++){
            int arrayposition = lengthofnumber-i-1;
            ptnumber[arrayposition] = static_cast<char>(48+(temp%10));
            temp = temp/10;
        }
            ptcalc = 0;
    }
    LCD.SetFontColor(BLACK);
    LCD.WriteAt(ptnumber,220,20);
    //quit button in stats
    LCD.SetFontColor(BLACK);
    LCD.DrawRectangle(qbsx-5,qbsy-5,60,25);
    LCD.WriteAt("Menu", qbsx,qbsy);
    if(LCD.Touch(&xpos, &ypos)){
            while(LCD.Touch(&xpos, &ypos)){
                if(xpos>qbsx && ypos>qbsy && xpos<(qbsx+45) && ypos<(qbsy+15)){
                    LCD.SetFontColor(WHITE); 
                    LCD.WriteAt("Menu", qbsx,qbsy);
                    LCD.DrawRectangle(qbsx-5,qbsy-5,60,25);
                    menuflag = 1;
                    sflag = 0;
                }
                else{
                    LCD.SetFontColor(BLACK);
                    LCD.WriteAt("Menu", qbsx,qbsy);
                    LCD.DrawRectangle(qbsx-5,qbsy-5,60,25);
                    menuflag = 0;
                    sflag = 1;
                }
            }
    }
    }

    //information written for the instructions screen
    if(iflag && !menuflag){
    if(infscreen == 1){
    startscreen = infscreen;
    LCD.Clear(GREEN);
    LCD.WriteAt("How to play:",1,1);
    LCD.WriteAt("The goal of the game is to",1,20);
    LCD.WriteAt("have a hand that is valued ",1,40);
    LCD.WriteAt("better than your opponents,", 1, 60);
    LCD.WriteAt("and these valuations can be ",1,80);
    LCD.WriteAt("seen on page 3. To start, ",1,100);
    LCD.WriteAt("each player is given 2",1,120);
    LCD.WriteAt("cards. A initial buy in is", 1, 140);
    LCD.WriteAt("Next", nextbx,nextby);
    LCD.DrawRectangle(nextbx-5,nextby-5,60,25);
    }
    //next page
    else if(infscreen == 2){
    startscreen = infscreen;
    LCD.Clear(GREEN);
    LCD.WriteAt("How to play:",1,1);
    LCD.WriteAt("taken, costing 100 points.", 1, 20);
    LCD.WriteAt("From there, the each player", 1, 40);
    LCD.WriteAt("has the choice to either",1,60);
    LCD.WriteAt("raise the pot or pass. If", 1, 80);
    LCD.WriteAt("the pot is raised, the", 1, 100);
    LCD.WriteAt("player must either pass,",1, 120);
    LCD.WriteAt("in which they don't play",1, 140);
    LCD.WriteAt("for the rest of the round,",1, 160);
    LCD.WriteAt("or can match and put in the",1, 180);
    LCD.WriteAt("Next", nextbx,nextby);
    LCD.DrawRectangle(nextbx-5,nextby-5,60,25);
    LCD.WriteAt("Prev", prevbx,prevby);
    LCD.DrawRectangle(prevbx-5,prevby-5,60,25);
    }
    //next page
    else if(infscreen == 3){
        startscreen = infscreen;
    LCD.Clear(GREEN);
    LCD.WriteAt("How to play:",1,1);
    LCD.WriteAt("same amounts of tokens", 1, 20);
    LCD.WriteAt("that were raised by the", 1, 40);
    LCD.WriteAt("other player. After this,",1,60);
    LCD.WriteAt("the next card is flipped", 1, 80);
    LCD.WriteAt("and the process is", 1, 100);
    LCD.WriteAt("repeated until 5 cards",1, 120);
    LCD.WriteAt("are layed out. Once the ",1, 140);
    LCD.WriteAt("final round of bidding is",1, 160);
    LCD.WriteAt("complete, players reveal",1, 180);
    LCD.WriteAt("Next", nextbx,nextby);
    LCD.DrawRectangle(nextbx-5,nextby-5,60,25);
    LCD.WriteAt("Prev", prevbx,prevby);
    LCD.DrawRectangle(prevbx-5,prevby-5,60,25);
    }
    //next page
    else if(infscreen == 4){
        startscreen = infscreen;
    LCD.Clear(GREEN);
    LCD.WriteAt("How to play:",1,1);
    LCD.WriteAt("their hands, and the", 1, 20);
    LCD.WriteAt("player that had the ", 1, 40);
    LCD.WriteAt("higher rank hand wins.",1,60);
    LCD.WriteAt("The winning player then", 1, 80);
    LCD.WriteAt("recives the points in", 1, 100);
    LCD.WriteAt("the pot. Games are played",1, 120);
    LCD.WriteAt("until 1 player runs out",1, 140);
    LCD.WriteAt("of points or both players",1, 160);
    LCD.WriteAt("choose to stop. ",1, 180);
    LCD.WriteAt("Next", nextbx,nextby);
    LCD.DrawRectangle(nextbx-5,nextby-5,60,25);
    LCD.WriteAt("Prev", prevbx,prevby);
    LCD.DrawRectangle(prevbx-5,prevby-5,60,25);
    }
    //next page
    else if(infscreen == 5){
        startscreen = infscreen;
        FEHImage AOH;
        FEHImage KOH;
        FEHImage QOH;
        FEHImage JOH;
        FEHImage TOH;
        AOH.Open("cards/AceOfHeartsFEH.pic");
        KOH.Open("cards/KingOfHeartsFEH.pic");
        QOH.Open("cards/QueenOfHeartsFEH.pic");
        JOH.Open("cards/JackOfHeartsFEH.pic");
        TOH.Open("cards/10OfHeartsFEH.pic");
        LCD.Clear(GREEN);
        LCD.SetFontColor(BLACK);
        LCD.WriteAt("Ranks of hands (1-10):",1,1);
        LCD.WriteAt("1: Royal Flush",1,20);
        AOH.Draw(0,40);
        KOH.Draw(65,40);
        QOH.Draw(130,40);
        JOH.Draw(195,40);
        TOH.Draw(260,40);
        LCD.SetFontColor(BLACK);
        LCD.WriteAt("10 Jack Queen King Ace",1,145);
        LCD.WriteAt("of the same suite",1,170);
        LCD.WriteAt("Next", nextbx,nextby);
        LCD.DrawRectangle(nextbx-5,nextby-5,60,25);
        LCD.WriteAt("Prev", prevbx,prevby);
        LCD.DrawRectangle(prevbx-5,prevby-5,60,25);
        AOH.Close();
        KOH.Close();
        QOH.Close();
        JOH.Close();
        TOH.Close();
        }
        //next page
    else if(infscreen == 6){
        startscreen = infscreen;
        FEHImage OOS;
        FEHImage TWOS;
        FEHImage THOS;
        FEHImage FOOS;
        FEHImage FIOS;
        OOS.Open("cards/AceOfSpadesFEH.pic");
        TWOS.Open("cards/2OfSpadesFEH.pic");
        THOS.Open("cards/3OfSpadesFEH.pic");
        FOOS.Open("cards/4OfSpadesFEH.pic");
        FIOS.Open("cards/5OfSpadesFEH.pic");
        LCD.Clear(GREEN);
        LCD.SetFontColor(BLACK);
        LCD.WriteAt("Ranks of hands:",1,1);
        LCD.WriteAt("2: Straight Flush",1,20);
        OOS.Draw(0,40);
        TWOS.Draw(65,40);
        THOS.Draw(130,40);
        FOOS.Draw(195,40);
        FIOS.Draw(260,40);
        LCD.SetFontColor(BLACK);
        LCD.WriteAt("5 cards of the same suite",1,145);
        LCD.WriteAt("in a row",1,170);
        LCD.WriteAt("Next", nextbx,nextby);
        LCD.DrawRectangle(nextbx-5,nextby-5,60,25);
        LCD.WriteAt("Prev", prevbx,prevby);
        LCD.DrawRectangle(prevbx-5,prevby-5,60,25);
        OOS.Close();
        TWOS.Close();
        THOS.Close();
        FOOS.Close();
        FIOS.Close();
    }
    //next page
    else if(infscreen == 7){
        startscreen = infscreen;
        FEHImage AOH,KOH,QOH,JOH,TOH;
        AOH.Open("cards/7OfHeartsFEH.pic");
        KOH.Open("cards/7OfClubsFEH.pic");
        QOH.Open("cards/7OfDiamondsFEH.pic");
        JOH.Open("cards/7OfSpadesFEH.pic");
        TOH.Open("cards/10OfHeartsFEH.pic");
        LCD.Clear(GREEN);
        LCD.SetFontColor(BLACK);
        LCD.WriteAt("Ranks of hands (1-10):",1,1);
        LCD.WriteAt("3: Four-Of-A-Kind",1,20);
        AOH.Draw(0,40);
        KOH.Draw(65,40);
        QOH.Draw(130,40);
        JOH.Draw(195,40);
        LCD.SetFontColor(BLACK);
        LCD.WriteAt("4 cards of the same value",1,145);
        LCD.WriteAt("Next", nextbx,nextby);
        LCD.DrawRectangle(nextbx-5,nextby-5,60,25);
        LCD.WriteAt("Prev", prevbx,prevby);
        LCD.DrawRectangle(prevbx-5,prevby-5,60,25);
        AOH.Close();
        KOH.Close();
        QOH.Close();
        JOH.Close();
        TOH.Close();
        }
        //next page
        else if(infscreen == 8){
        startscreen = infscreen;
        FEHImage AOH,KOH,QOH,JOH,TOH;
        AOH.Open("cards/2OfHeartsFEH.pic");
        KOH.Open("cards/2OfClubsFEH.pic");
        QOH.Open("cards/2OfDiamondsFEH.pic");
        JOH.Open("cards/5OfSpadesFEH.pic");
        TOH.Open("cards/5OfHeartsFEH.pic");
        LCD.Clear(GREEN);
        LCD.SetFontColor(BLACK);
        LCD.WriteAt("Ranks of hands (1-10):",1,1);
        LCD.WriteAt("4: Full House",1,20);
        AOH.Draw(0,40);
        KOH.Draw(65,40);
        QOH.Draw(130,40);
        JOH.Draw(195,40);
        TOH.Draw(260,40);
        LCD.SetFontColor(BLACK);
        LCD.WriteAt("3 cards of one type",1,145);
        LCD.WriteAt("and two of another",1,170);
        LCD.WriteAt("Next", nextbx,nextby);
        LCD.DrawRectangle(nextbx-5,nextby-5,60,25);
        LCD.WriteAt("Prev", prevbx,prevby);
        LCD.DrawRectangle(prevbx-5,prevby-5,60,25);
        AOH.Close();
        KOH.Close();
        QOH.Close();
        JOH.Close();
        TOH.Close();
        }
        //next page
        else if(infscreen == 9){
        startscreen = infscreen;
        FEHImage AOH;
        FEHImage KOH;
        FEHImage QOH;
        FEHImage JOH;
        FEHImage TOH;
        AOH.Open("cards/AceOfHeartsFEH.pic");
        KOH.Open("cards/2OfHeartsFEH.pic");
        QOH.Open("cards/5OfHeartsFEH.pic");
        JOH.Open("cards/8OfHeartsFEH.pic");
        TOH.Open("cards/QueenOfHeartsFEH.pic");
        LCD.Clear(GREEN);
        LCD.SetFontColor(BLACK);
        LCD.WriteAt("Ranks of hands (1-10):",1,1);
        LCD.WriteAt("5: Flush",1,20);
        AOH.Draw(0,40);
        KOH.Draw(65,40);
        QOH.Draw(130,40);
        JOH.Draw(195,40);
        TOH.Draw(260,40);
        LCD.SetFontColor(BLACK);
        LCD.WriteAt("5 cards of the same suite",1,145);
        LCD.WriteAt("Next", nextbx,nextby);
        LCD.DrawRectangle(nextbx-5,nextby-5,60,25);
        LCD.WriteAt("Prev", prevbx,prevby);
        LCD.DrawRectangle(prevbx-5,prevby-5,60,25);
        AOH.Close();
        KOH.Close();
        QOH.Close();
        JOH.Close();
        TOH.Close();
        }
        //next page
        else if(infscreen == 10){
        startscreen = infscreen;
        FEHImage AOH;
        FEHImage KOH;
        FEHImage QOH;
        FEHImage JOH;
        FEHImage TOH;
        AOH.Open("cards/QueenOfSpadesFEH.pic");
        KOH.Open("cards/JackOfHeartsFEH.pic");
        QOH.Open("cards/10OfDiamondsFEH.pic");
        JOH.Open("cards/9OfHeartsFEH.pic");
        TOH.Open("cards/8OfClubsFEH.pic");
        LCD.Clear(GREEN);
        LCD.SetFontColor(BLACK);
        LCD.WriteAt("Ranks of hands (1-10):",1,1);
        LCD.WriteAt("6: Straight",1,20);
        AOH.Draw(0,40);
        KOH.Draw(65,40);
        QOH.Draw(130,40);
        JOH.Draw(195,40);
        TOH.Draw(260,40);
        LCD.SetFontColor(BLACK);
        LCD.WriteAt("5 cards in a row",1,145);
        LCD.WriteAt("of the any suite",1,170);
        LCD.WriteAt("Next", nextbx,nextby);
        LCD.DrawRectangle(nextbx-5,nextby-5,60,25);
        LCD.WriteAt("Prev", prevbx,prevby);
        LCD.DrawRectangle(prevbx-5,prevby-5,60,25);
        AOH.Close();
        KOH.Close();
        QOH.Close();
        JOH.Close();
        TOH.Close();
        }
        //next page
        else if(infscreen == 11){
        startscreen = infscreen;
        FEHImage AOH;
        FEHImage KOH;
        FEHImage QOH;
        FEHImage JOH;
        FEHImage TOH;
        AOH.Open("cards/8OfSpadesFEH.pic");
        KOH.Open("cards/8OfHeartsFEH.pic");
        QOH.Open("cards/8OfDiamondsFEH.pic");
        JOH.Open("cards/9OfHeartsFEH.pic");
        TOH.Open("cards/8OfClubsFEH.pic");
        LCD.Clear(GREEN);
        LCD.SetFontColor(BLACK);
        LCD.WriteAt("Ranks of hands (1-10):",1,1);
        LCD.WriteAt("7: Three of a kind",1,20);
        AOH.Draw(0,40);
        KOH.Draw(65,40);
        QOH.Draw(130,40);
        LCD.SetFontColor(BLACK);
        LCD.WriteAt("3 cards of the same value",1,145);
        LCD.WriteAt("Next", nextbx,nextby);
        LCD.DrawRectangle(nextbx-5,nextby-5,60,25);
        LCD.WriteAt("Prev", prevbx,prevby);
        LCD.DrawRectangle(prevbx-5,prevby-5,60,25);
        AOH.Close();
        KOH.Close();
        QOH.Close();
        JOH.Close();
        TOH.Close();
        }
        else if(infscreen == 12){
        startscreen = infscreen;
        FEHImage AOH;
        FEHImage KOH;
        FEHImage QOH;
        FEHImage JOH;
        FEHImage TOH;
        AOH.Open("cards/JackOfClubsFEH.pic");
        KOH.Open("cards/JackOfHeartsFEH.pic");
        QOH.Open("cards/5OfDiamondsFEH.pic");
        JOH.Open("cards/5OfHeartsFEH.pic");
        TOH.Open("cards/8OfClubsFEH.pic");
        LCD.Clear(GREEN);
        LCD.SetFontColor(BLACK);
        LCD.WriteAt("Ranks of hands (1-10):",1,1);
        LCD.WriteAt("8: Two-Pair",1,20);
        AOH.Draw(0,40);
        KOH.Draw(65,40);
        QOH.Draw(130,40);
        JOH.Draw(195,40);
        LCD.SetFontColor(BLACK);
        LCD.WriteAt("2 pairs of 2 cards",1,145);
        LCD.WriteAt("of the same value",1,170);
        LCD.WriteAt("Next", nextbx,nextby);
        LCD.DrawRectangle(nextbx-5,nextby-5,60,25);
        LCD.WriteAt("Prev", prevbx,prevby);
        LCD.DrawRectangle(prevbx-5,prevby-5,60,25);
        AOH.Close();
        KOH.Close();
        QOH.Close();
        JOH.Close();
        TOH.Close();
        }
        else if(infscreen == 13){
        startscreen = infscreen;
        FEHImage AOH;
        FEHImage KOH;
        FEHImage QOH;
        FEHImage JOH;
        FEHImage TOH;
        AOH.Open("cards/3OfSpadesFEH.pic");
        KOH.Open("cards/3OfHeartsFEH.pic");
        QOH.Open("cards/10OfDiamondsFEH.pic");
        JOH.Open("cards/9OfHeartsFEH.pic");
        TOH.Open("cards/8OfClubsFEH.pic");
        LCD.Clear(GREEN);
        LCD.SetFontColor(BLACK);
        LCD.WriteAt("Ranks of hands (1-10):",1,1);
        LCD.WriteAt("9: Pair",1,20);
        AOH.Draw(0,40);
        KOH.Draw(65,40);
        LCD.SetFontColor(BLACK);
        LCD.WriteAt("2 pairs of 2 cards",1,145);
        LCD.WriteAt("of the same value",1,170);
        LCD.WriteAt("Next", nextbx,nextby);
        LCD.DrawRectangle(nextbx-5,nextby-5,60,25);
        LCD.WriteAt("Prev", prevbx,prevby);
        LCD.DrawRectangle(prevbx-5,prevby-5,60,25);
        AOH.Close();
        KOH.Close();
        QOH.Close();
        JOH.Close();
        TOH.Close();
        }
        else if(infscreen == 14){
        startscreen = infscreen;
        FEHImage AOH;
        FEHImage KOH;
        FEHImage QOH;
        FEHImage JOH;
        FEHImage TOH;
        AOH.Open("cards/AceOfHeartsFEH.pic");
        KOH.Open("cards/JackOfHeartsFEH.pic");
        QOH.Open("cards/10OfDiamondsFEH.pic");
        JOH.Open("cards/9OfHeartsFEH.pic");
        TOH.Open("cards/8OfClubsFEH.pic");
        LCD.Clear(GREEN);
        LCD.SetFontColor(BLACK);
        LCD.WriteAt("Ranks of hands (1-10):",1,1);
        LCD.WriteAt("10: High",1,20);
        AOH.Draw(0,40);
        LCD.SetFontColor(BLACK);
        LCD.WriteAt("Having the highest value",1,135);
        LCD.WriteAt("card. Ace wins, then",1,155);
        LCD.WriteAt("decending order",1,175);
        LCD.WriteAt("Prev", prevbx,prevby);
        LCD.DrawRectangle(prevbx-5,prevby-5,60,25);
        AOH.Close();
        KOH.Close();
        QOH.Close();
        JOH.Close();
        TOH.Close();
        }
    //quit button in instructions
    LCD.SetFontColor(BLACK);
    LCD.DrawRectangle(qbsx-5,qbsy-5,60,25);
    LCD.WriteAt("Menu", qbsx,qbsy);
    if(LCD.Touch(&xpos, &ypos)){
            while(LCD.Touch(&xpos, &ypos)){
                if(xpos>qbsx && ypos>qbsy && xpos<(qbsx+45) && ypos<(qbsy+15)){
                    LCD.SetFontColor(WHITE); 
                    LCD.WriteAt("Menu", qbsx,qbsy);
                    LCD.DrawRectangle(qbsx-5,qbsy-5,60,25);
                    menuflag = 1;
                    iflag = 0;
                }
                else{
                    LCD.SetFontColor(BLACK);
                    LCD.WriteAt("Menu", qbsx,qbsy);
                    LCD.DrawRectangle(qbsx-5,qbsy-5,60,25);
                    menuflag = 0;
                    iflag = 1;
                }
                //next page
                if(startscreen!=14 && xpos>nextbx && ypos>nextby && xpos<(nextbx+45) && ypos<(nextby+15)){
                    LCD.SetFontColor(WHITE); 
                    LCD.WriteAt("Next", nextbx,nextby);
                    LCD.DrawRectangle(nextbx-5,nextby-5,60,25);
                    infscreen = startscreen + 1;
                }
                //prev button
                else if(startscreen!=1 &&xpos>prevbx && ypos>prevby && xpos<(prevbx+45) && ypos<(prevby+15)){
                    LCD.SetFontColor(WHITE); 
                    LCD.WriteAt("Prev", prevbx,prevby);
                    LCD.DrawRectangle(prevbx-5,prevby-5,60,25);
                    infscreen = startscreen - 1;
                }
                else{
                    if(startscreen!=1){
                    LCD.SetFontColor(BLACK);
                    LCD.WriteAt("Prev", prevbx,prevby);
                    LCD.DrawRectangle(prevbx-5,prevby-5,60,25);
                    }
                    if(startscreen!=14){
                    LCD.SetFontColor(BLACK);
                    LCD.WriteAt("Next", nextbx,nextby);
                    LCD.DrawRectangle(nextbx-5, nextby-5,60,25);
                    }
                    infscreen = startscreen;
                }   
                }

                LCD.Update();
            }
    }

    //Credits
    if(cflag && !menuflag){
    LCD.WriteAt("Programmed by:",1,1);
    LCD.WriteAt("Pratham Kancherla",1,20);
    LCD.WriteAt("Keegan Freyhof",1,40);
    //quit button in credits
    LCD.SetFontColor(BLACK);
    LCD.DrawRectangle(qbsx-5,qbsy-5,60,25);
    LCD.WriteAt("Menu", qbsx,qbsy);
    if(LCD.Touch(&xpos, &ypos)){
            while(LCD.Touch(&xpos, &ypos)){
                if(xpos>qbsx && ypos>qbsy && xpos<(qbsx+45) && ypos<(qbsy+15)){
                    LCD.SetFontColor(WHITE); 
                    LCD.WriteAt("Menu", qbsx,qbsy);
                    LCD.DrawRectangle(qbsx-5,qbsy-5,60,25);
                    menuflag = 1;
                    cflag = 0;
                }
                else{
                    LCD.SetFontColor(BLACK);
                    LCD.WriteAt("Menu", qbsx,qbsy);
                    LCD.DrawRectangle(qbsx-5,qbsy-5,60,25);
                    menuflag = 0;
                    cflag = 1;
                }
            }
    }
    }
    
    //ends the while loop
    }
    mainMenuCards.Close();
    int timeend = time(NULL);
    float accplaytime = timeend-timenow;
    accplaytime = accplaytime + totalplaytime;
    FILE *fout;
    fout = fopen("time.txt","w");
    fprintf(fout, "%f", accplaytime);
    fclose(fout);
    return 1;
}

//implementation of function 
int playerSelection(int *AIflag, int *TwoPFlag, int *initPlayerSelect){
    //Position varibles for cursor and buttons
    int xpos = 0, ypos = 0, AIbx = 70, AIby = 112,TwoPBx = 180, Twopby = 112;
    //Flag to dertmine if the selection menu should continue to be shown. 
    int flag = 0;
    
    //draws the buttons the first time though
    if(&initPlayerSelect){
    LCD.Clear(GREEN);
    LCD.SetFontColor(BLACK);
    LCD.WriteAt("Bot", AIbx,AIby);
    LCD.DrawRectangle(AIbx-5,AIby-5,50,25);
    LCD.SetFontColor(BLACK);
    LCD.WriteAt("2 Players", TwoPBx,Twopby);
    LCD.DrawRectangle(TwoPBx-5,Twopby-5,120,25);
    *initPlayerSelect = 0;
    }

    //causes the buttons to be able to be intercated with
    while(LCD.Touch(&xpos, &ypos)){
            //checks if button is pressed on bot
            if(xpos>(AIbx-5) && ypos>(AIby-5) && xpos<(AIbx+45) && ypos<(AIby+15)){
                    LCD.SetFontColor(WHITE); 
                    LCD.WriteAt("Bot", AIbx,AIby);
                    LCD.DrawRectangle(AIbx-5,AIby-5,50,25);
                    LCD.SetFontColor(BLACK);
                    LCD.WriteAt("2 Players", TwoPBx,Twopby);
                    LCD.DrawRectangle(TwoPBx-5,Twopby-5,120,25);
                    *AIflag = 1;
                    *TwoPFlag = 0;
                    flag = 1;
            } 
            //checks if button being pressed is 2 players
            else if(xpos>(TwoPBx-5) && ypos>(Twopby-5) && xpos<(TwoPBx+115) && ypos<(Twopby+15)){
                    LCD.SetFontColor(WHITE); 
                    LCD.WriteAt("2 Players", TwoPBx,Twopby);
                    LCD.DrawRectangle(TwoPBx-5,Twopby-5,120,25);
                    LCD.SetFontColor(BLACK);
                    LCD.WriteAt("Bot", AIbx,AIby);
                    LCD.DrawRectangle(AIbx-5,AIby-5,50,25);
                    *AIflag = 0;
                    *TwoPFlag = 1;
                    flag = 1;
            }
            else{
                    LCD.SetFontColor(BLACK);
                    LCD.WriteAt("2 Players", TwoPBx,Twopby);
                    LCD.DrawRectangle(TwoPBx-5,Twopby-5,120,25);
                    LCD.WriteAt("Bot", AIbx,AIby);
                    LCD.DrawRectangle(AIbx-5,AIby-5,50,25);
                    *AIflag = 0;
                    *TwoPFlag = 0;
                    flag=0;
            }
            //updates the screen
            LCD.Update();  
        }
        return flag;
}

//implementation of function
int difficultySelection(int *initDifficultySelect){
    //position of elements 
    int difficultyButtonEasyX = 70, difficultyButtonEasyY = 112, difficultyButtonHardX = 200, difficultyButtonHardY = 112, xpos =0, ypos = 0;
    //flag 
    int AIdflag = 0;
    if(!(0)){
    LCD.SetFontColor(BLACK);
    LCD.WriteAt("Easy", difficultyButtonEasyX,difficultyButtonEasyY);
    LCD.DrawRectangle(difficultyButtonEasyX-5,difficultyButtonEasyY-5,65,25);
    LCD.WriteAt("Hard", difficultyButtonHardX,difficultyButtonHardY);
    LCD.DrawRectangle(difficultyButtonHardX-5,difficultyButtonHardY-5,65,25);
    *initDifficultySelect = 1;
    }
    //runs as there is touch by the user
    while(LCD.Touch(&xpos, &ypos)){
        //checks if the easy button is being pressed and makes necessary updates
        if(xpos>(difficultyButtonEasyX-5) && ypos>(difficultyButtonEasyY-5) && xpos<(difficultyButtonEasyX+55) && ypos<(difficultyButtonEasyY+15)){
            LCD.SetFontColor(WHITE); 
            LCD.WriteAt("Easy", difficultyButtonEasyX,difficultyButtonEasyY);
            LCD.DrawRectangle(difficultyButtonEasyX-5,difficultyButtonEasyY-5,65,25);
            LCD.SetFontColor(BLACK);
            LCD.WriteAt("Hard", difficultyButtonHardX,difficultyButtonHardY);
            LCD.DrawRectangle(difficultyButtonHardX-5,difficultyButtonHardY-5,65,25);
            AIdflag = 1;
        } 
        //checks if the hard button is being pressed and makes necessary updates
        else if(xpos>(difficultyButtonHardX-5) && ypos>(difficultyButtonHardY-5) && xpos<(difficultyButtonHardX+65) && ypos<(difficultyButtonHardY+15)){
            LCD.SetFontColor(WHITE); 
            LCD.WriteAt("Hard", difficultyButtonHardX,difficultyButtonHardY);
            LCD.DrawRectangle(difficultyButtonHardX-5,difficultyButtonHardY-5,65,25);
            LCD.SetFontColor(BLACK);
            LCD.WriteAt("Easy", difficultyButtonEasyX,difficultyButtonEasyY);
            LCD.DrawRectangle(difficultyButtonEasyX-5,difficultyButtonEasyY-5,65,25);
            AIdflag = 2;
        }
        else{
            LCD.SetFontColor(BLACK);
            LCD.WriteAt("Hard", difficultyButtonHardX,difficultyButtonHardY);
            LCD.DrawRectangle(difficultyButtonHardX-5,difficultyButtonHardY-5,65,25);
            LCD.WriteAt("Easy", difficultyButtonEasyX,difficultyButtonEasyY);
            LCD.DrawRectangle(difficultyButtonEasyX-5,difficultyButtonEasyY-5,65,25);
            AIdflag = 0;
        }
        LCD.Update();  
    }
    return AIdflag;
}

//implemnetation go the game running class
int gameIsRunning(int *botdec,int *oppbet,int *bet,int *lastbetp1,int *lastbetp2, int *winner, int *AIflag, int *TwoPFlag,int *p1wins,int *p2wins, int *PPlayer, int *POpponent, int *stge, int *cof, int *xpos, int *ypos, int *pturn, int *ccpx, int *ccpy, char *scorep, char *scoreo, int *round, int *betting, int *checking, int *tracking, int *total, FEHImage *card1,FEHImage *card2,FEHImage *card3,FEHImage *card4,FEHImage *card5,FEHImage *card6,FEHImage *card7,FEHImage *card8,FEHImage *card9, deck *gamedeck){
    int temp = 0;
    //position values
    int cardx = 0, cardy = 0, mbsx = 264, mbsy = 5, csx = 1, csy=200, bsx = 70, bsy = 200, fsx = 125, fsy = 200, scardx = *ccpx, scardy = *ccpy, incrementTurnX = 264, incrementTurnY = 30;
    int cardsonfield = *cof, cardsOnFieldX = 10, cardsOnFieldY = 60;
    //flags
    int quitflag = 0, game = 1, stage = *stge;
    //Positions for quit menu
    int returnButtonX = 70, returnButtonY = 112, quitButtonX = 200, quitButtonY = 112;
    //initilization
        if(stage == 0){
            //writes the scores of both players to the screen
            temp = *PPlayer;
            scorep[2] = static_cast<char>(48+temp%10);
            temp = temp/10;
            scorep[1] = static_cast<char>(48+temp%10);
            temp = temp/10;
            scorep[0] = static_cast<char>(48+temp%10);
            temp = *POpponent;
            scoreo[2] = static_cast<char>(48+temp%10);
            temp = temp/10;
            scoreo[1] = static_cast<char>(48+temp%10);
            temp = temp/10;
            scoreo[0] = static_cast<char>(48+temp%10);
            stage++;
        }

        //setting up the score strings
        if(stage ==1){
            //card loop
            (*gamedeck).shuffleCards();
            for(int i =1; i<10;i++){
            card temp;
            temp.copyFrom((*gamedeck).removeTopCard());
            char fname[32] = {0};
            char cardout[32] = {0};
            fname[0] = 'c';
            fname[1] = 'a';
            fname[2] = 'r';
            fname[3] = 'd';
            fname[4] = 's';
            fname[5] = '/';
            temp.toString(cardout);
            for(int i = 0; i<strlen(cardout); i++){
                fname[i+6] = cardout[i]; 
            }
            int len = strlen(fname);
            fname[len] = 's';
            fname[len+1] = 'F';
            fname[len+2] = 'E';
            fname[len+3] = 'H';
            fname[len+4] = '.';
            fname[len+5] = 'p';
            fname[len+6] = 'i';
            fname[len+7] = 'c';
            if(i == 1){
            (*card1).Open(fname);
            }
            if(i == 2){
            (*card2).Open(fname);
            }
            if(i == 3){
            (*card3).Open(fname);
            }
            if(i == 4){
            (*card4).Open(fname);
            }
            if(i == 5){
            (*card5).Open(fname);
            }
            if(i == 6){
            (*card6).Open(fname);
            }
            if(i == 7){
            (*card7).Open(fname);
            }
            if(i == 8){
            (*card8).Open(fname);
            }
            if(i == 9){
            (*card9).Open(fname);
            }
        }
            stage++;
        }
        //the game interfacing phase
        if(stage == 2){
            LCD.Clear(GREEN);
            LCD.SetFontColor(BLACK);
            LCD.DrawRectangle(mbsx-5,mbsy-5,60,25);
            LCD.WriteAt("Menu", mbsx,mbsy);
            LCD.DrawRectangle(csx, csy-5, 63, 25);
            LCD.WriteAt("Check", csx, csy);
            LCD.DrawRectangle(bsx, bsy-5, 50, 25);
            LCD.WriteAt("Bet", bsx+5, bsy);
            LCD.DrawRectangle(fsx, fsy-5, 60, 25);
            LCD.WriteAt("Fold", fsx+5, fsy);
            LCD.WriteAt("P1: ", 1, 1);
            LCD.WriteAt(scorep, 36,1);
            LCD.WriteAt("P2: ", 100,1);
            LCD.WriteAt(scoreo, 135,1);
            if(*pturn == 1){
                LCD.WriteAt("P2 Turn", 175, 1);
            }
            else{
                LCD.WriteAt("P1 Turn", 175, 1);
            }
            LCD.WriteAt("Pot: ", 1, 25);
            LCD.WriteAt(*total, 85, 25);
            //draws the cards to the screen
            if(*pturn==0){
                (*card1).Draw(scardx-60,scardy);
                (*card2).Draw(scardx, scardy);
            }
            else{
                (*card3).Draw(scardx-60,scardy);
                (*card4).Draw(scardx, scardy);
            }
            if(cardsonfield>0){
                (*card5).Draw(cardsOnFieldX+240, cardsOnFieldY);
                (*card6).Draw(cardsOnFieldX+180, cardsOnFieldY);
                (*card7).Draw(cardsOnFieldX+120, cardsOnFieldY);
            }
            if(cardsonfield>1){
                (*card8).Draw(cardsOnFieldX+60, cardsOnFieldY);
            }
            if(cardsonfield>2){
                (*card9).Draw(cardsOnFieldX, cardsOnFieldY);
            }
            int playerturn = *pturn;
            int cofi = cardsonfield;
            
            while(LCD.Touch(xpos, ypos)){
                LCD.Clear(GREEN);
                if(*pturn == 1){
                LCD.WriteAt("P2 Turn", 175, 1);
                }
                else{
                LCD.WriteAt("P1 Turn", 175, 1);
                }
                LCD.SetFontColor(BLACK);
                LCD.WriteAt("P1: ", 1, 1);
                LCD.WriteAt(scorep, 36,1);
                LCD.WriteAt("P2: ", 100,1);
                LCD.WriteAt(scoreo, 135,1);
                if(*pturn==0){
                    (*card1).Draw(scardx-60,scardy);
                    (*card2).Draw(scardx, scardy);
                }
                else{
                    (*card3).Draw(scardx-60,scardy);
                    (*card4).Draw(scardx, scardy);
                }
                //quit button in game
                if(*xpos>mbsx && *ypos>mbsy && *xpos<(mbsx+45) && *ypos<(mbsy+15)){
                    LCD.SetFontColor(WHITE); 
                    LCD.WriteAt("Menu", mbsx,mbsy);
                    LCD.DrawRectangle(mbsx-5,mbsy-5,60,25);
                    LCD.SetFontColor(BLACK);
                    LCD.DrawRectangle(csx, csy-5, 63, 25);
                    LCD.WriteAt("Check", csx, csy);
                    LCD.DrawRectangle(bsx, bsy-5, 50, 25);
                    LCD.WriteAt("Bet", bsx+5, bsy);
                    LCD.DrawRectangle(fsx, fsy-5, 60, 25);
                    LCD.WriteAt("Fold", fsx+5, fsy);
                    game = 1;
                    stage = 3;
                    *round = 0;
                    *betting = 0;
                    *checking = 0;
                }
                //if the check button is being pressed
                else if(*xpos>csx && *ypos>csy && *xpos<(csx+45) && *ypos<(csy+15)){
                    LCD.SetFontColor(WHITE);
                    LCD.WriteAt("Check", csx, csy);
                    LCD.DrawRectangle(csx, csy-5, 63, 25);
                    LCD.SetFontColor(BLACK);
                    LCD.WriteAt("Menu", mbsx,mbsy);
                    LCD.DrawRectangle(mbsx-5,mbsy-5,60,25);
                    LCD.DrawRectangle(bsx, bsy-5, 50, 25);
                    LCD.WriteAt("Bet", bsx+5, bsy);
                    LCD.DrawRectangle(fsx, fsy-5, 60, 25);
                    LCD.WriteAt("Fold", fsx+5, fsy);
                    *checking = 1;
                    stage = 2;
                    game = 1;
                    *round = 0;
                    *betting = 0;
                }
                //if the bet button is being pressed
                else if(*xpos>bsx && *ypos>bsy && *xpos<(bsx+45) && *ypos<(bsy+15)){
                    LCD.SetFontColor(WHITE);
                    LCD.DrawRectangle(bsx, bsy-5, 50, 25);
                    LCD.WriteAt("Bet", bsx+5, bsy);
                    LCD.SetFontColor(BLACK);
                    LCD.WriteAt("Menu", mbsx,mbsy);
                    LCD.DrawRectangle(mbsx-5,mbsy-5,60,25);
                    LCD.DrawRectangle(csx, csy-5, 63, 25);
                    LCD.WriteAt("Check", csx, csy);
                    LCD.DrawRectangle(fsx, fsy-5, 60, 25);
                    LCD.WriteAt("Fold", fsx+5, fsy);
                    *checking = 0;
                    if(*pturn && *POpponent>=10){
                        *betting = 1;
                    }else if(!(*pturn)&&*PPlayer>=10){
                        *betting = 1;
                    }else{
                        *betting = 0;
                    }
                    *round = 0;
                    game = 1;
                    stage = -1;
                }
                //if the fold button is being pressed
                else if(*xpos>fsx && *ypos>fsy && *xpos<(fsx+45) && *ypos<(fsy+15)){
                    LCD.SetFontColor(WHITE);
                    LCD.DrawRectangle(fsx, fsy-5, 60, 25);
                    LCD.WriteAt("Fold", fsx+5, fsy);
                    LCD.SetFontColor(BLACK);
                    LCD.WriteAt("Menu", mbsx,mbsy);
                    LCD.DrawRectangle(mbsx-5,mbsy-5,60,25);
                    LCD.DrawRectangle(csx, csy-5, 63, 25);
                    LCD.WriteAt("Check", csx, csy);
                    LCD.DrawRectangle(bsx, bsy-5, 50, 25);
                    LCD.WriteAt("Bet", bsx+5, bsy);
                    *checking = 0;
                    *betting = 0;
                    *round = 1;
                    game = 1;
                    (*winner)=!*pturn;
                    stage = 5;
                }
                else{
                    LCD.SetFontColor(BLACK);
                    LCD.WriteAt("Menu", mbsx,mbsy);
                    LCD.DrawRectangle(mbsx-5,mbsy-5,60,25);
                    LCD.DrawRectangle(csx, csy-5, 63, 25);
                    LCD.WriteAt("Check", csx, csy);
                    LCD.DrawRectangle(bsx, bsy-5, 50, 25);
                    LCD.WriteAt("Bet", bsx+5, bsy);
                    LCD.DrawRectangle(fsx, fsy-5, 60, 25);
                    LCD.WriteAt("Fold", fsx+5, fsy);
                    stage = 2;
                    game = 1;
                    *round = 0;
                    *betting = 0;
                    *checking = 0;
                }
                //All of the cards are 60 by 90
                //These 2 lines allow the card to move by press
                //scardx = (*xpos-30)*((*xpos-30)>0)*((*xpos+30)<319)+259*!((*xpos+30)<319);
                //scardy = (*ypos-45)*((*ypos-45)>0)*((*ypos+45)<239)+149*!((*ypos+45)<239);
                *ccpx = scardx;
                *ccpy = scardy;
            }
            // *bet,int *lastbetp1,int *lastbetp2
            *pturn=playerturn;
            *stge = stage;
            *cof = cardsonfield;
        }
            //displays the cards if the check button is being pressed after both players decide to check in some sequence
            if(*checking){
                int temp1 = *POpponent, temp2 = *PPlayer;
                if(*pturn){
                    *POpponent = temp1 - *lastbetp1;
                    *total += *lastbetp1;
                    *lastbetp2 = *lastbetp1;
                    *lastbetp1 = 0;
                    *bet=2;
                    int po = *POpponent;
                    scoreo[2] = static_cast<char>(48+po%10);
                    po = po/10;
                    scoreo[1] = static_cast<char>(48+po%10);
                    po = po/10;
                    scoreo[0] = static_cast<char>(48+po%10);
                }
                else{
                    *PPlayer = temp2 - *lastbetp2;
                    *total += *lastbetp2;
                    *lastbetp1 = *lastbetp2;
                    *lastbetp2 = 0;
                    *bet=1;
                    int pp = *PPlayer;
                    scorep[2] = static_cast<char>(48+pp%10);
                    pp = pp/10;
                    scorep[1] = static_cast<char>(48+pp%10);
                    pp = pp/10;
                    scorep[0] = static_cast<char>(48+pp%10);
                }
                *checking=0;
            }
        if(stage == 3){
            //displays the menu for the screen
        LCD.Clear(GREEN);
            LCD.SetFontColor(BLACK);
        LCD.WriteAt("Back", returnButtonX,returnButtonY);
        LCD.DrawRectangle(returnButtonX-5,returnButtonY-5,65,25);
        LCD.WriteAt("Quit", quitButtonX,quitButtonY);
        LCD.DrawRectangle(quitButtonX-5,quitButtonY-5,65,25);
        while(LCD.Touch(xpos, ypos)){
            //goes back to the game screen if the back button is being pressed
            if(*xpos>(returnButtonX-5) && *ypos>(returnButtonY-5) && *xpos<(returnButtonX+55) && *ypos<(returnButtonY+15)){
                LCD.SetFontColor(WHITE); 
                LCD.WriteAt("Back", returnButtonX,returnButtonY);
                LCD.DrawRectangle(returnButtonX-5,returnButtonY-5,65,25);
                LCD.SetFontColor(BLACK);
                LCD.WriteAt("Quit", quitButtonX,quitButtonY);
                LCD.DrawRectangle(quitButtonX-5,quitButtonY-5,65,25);
                stage = 2;
                game = 1;
            } 
            //goes back to the menu of the game if the quit button is pressed
            else if(*xpos>(quitButtonX-5) && *ypos>(returnButtonY-5) && *xpos<(quitButtonX+65) && *ypos<(returnButtonY+15)){
                LCD.SetFontColor(WHITE); 
                LCD.WriteAt("Quit", quitButtonX,quitButtonY);
                LCD.DrawRectangle(quitButtonX-5,quitButtonY-5,65,25);
                LCD.SetFontColor(BLACK);
                LCD.WriteAt("Back", returnButtonX,returnButtonY);
                LCD.DrawRectangle(returnButtonX-5,returnButtonY-5,65,25);
                stage = 3;
                game = 0;
            }
            else{
                LCD.SetFontColor(BLACK);
                LCD.WriteAt("Quit", quitButtonX,quitButtonY);
                LCD.DrawRectangle(quitButtonX-5,quitButtonY-5,65,25);
                LCD.WriteAt("Back", returnButtonX,returnButtonY);
                LCD.DrawRectangle(returnButtonX-5,returnButtonY-5,65,25);
                stage = 3;
                game = 1;
            }
            LCD.Update(); 
            *stge = stage; 
            }
        }
        if(stage == 4){
            //after the winner has been determiend
            LCD.Clear(GREEN);
            LCD.SetFontColor(BLACK);
            if(*winner == 1){
                LCD.WriteAt("Player 1 won the hand", 1,1);
            }
            else if(*winner == 2){
                LCD.WriteAt("Player 2 won the hand", 1,1);
            }
            else{
                LCD.WriteAt("Players Tied", 1,1);
            }
            //asks user to play another hand 
            LCD.WriteAt("Play Another Hand", returnButtonX-50,returnButtonY);
            LCD.DrawRectangle(returnButtonX-55,returnButtonY-5,215,25);
            LCD.WriteAt("Quit", quitButtonX+40,quitButtonY);
            LCD.DrawRectangle(quitButtonX+35,quitButtonY-5,65,25);
            while(LCD.Touch(xpos, ypos)){
                //checks for if the user is pressing the play another hand button
            if(*xpos>(returnButtonX-55) && *ypos>(returnButtonY-5) && *xpos<(returnButtonX+160) && *ypos<(returnButtonY+15)){
                LCD.SetFontColor(WHITE); 
                LCD.WriteAt("Play Another Hand", returnButtonX-50,returnButtonY);
                LCD.DrawRectangle(returnButtonX-55,returnButtonY-5,215,25);
                LCD.SetFontColor(BLACK);
                LCD.WriteAt("Quit", quitButtonX+40,quitButtonY);
                LCD.DrawRectangle(quitButtonX+35,quitButtonY-5,65,25);
                stage = 0;
                *cof = 0;
                game = 1;
            } 
            //checks if the quit button is being pressed, which takes the user back to the menu screen
            else if(*xpos>(quitButtonX+35) && *ypos>(returnButtonY-5) && *xpos<(quitButtonX+85) && *ypos<(returnButtonY+15)){
                LCD.SetFontColor(WHITE); 
                LCD.WriteAt("Quit", quitButtonX+40,quitButtonY);
                LCD.DrawRectangle(quitButtonX+35,quitButtonY-5,65,25);
                LCD.SetFontColor(BLACK);
                LCD.WriteAt("Play Another Hand", returnButtonX-50,returnButtonY);
                LCD.DrawRectangle(returnButtonX-55,returnButtonY-5,215,25);
                stage = 4;
                game = 0;
                *cof = 5;
            }
            else{
                LCD.SetFontColor(BLACK);
                LCD.WriteAt("Quit", quitButtonX+40,quitButtonY);
                LCD.DrawRectangle(quitButtonX+35,quitButtonY-5,65,25);
                LCD.WriteAt("Play Another Hand", returnButtonX-50,returnButtonY);
                LCD.DrawRectangle(returnButtonX-55,returnButtonY-5,215,25);
                stage = 4;
                game = 1;
                *cof = 5;
            }
            LCD.Update(); 
            *stge = stage; 
            }
        }
    if(stage==5){
        if(p1wins){
            LCD.Clear(GREEN);
            LCD.SetFontColor(BLACK);
            LCD.WriteAt("Player 1 Wins", 120,10);
            LCD.WriteAt("Quit", quitButtonX,quitButtonY);
            LCD.DrawRectangle(quitButtonX-5,quitButtonY-5,65,25);
        }
        else{
            if(*TwoPFlag){
                LCD.Clear(GREEN);
                LCD.SetFontColor(BLACK);
                LCD.WriteAt("Player 2 wins", 120,10);
                LCD.WriteAt("Quit", quitButtonX,quitButtonY);
                LCD.DrawRectangle(quitButtonX-5,quitButtonY-5,65,25);
            }
            else{
              LCD.Clear(GREEN);
                LCD.SetFontColor(BLACK);
                LCD.WriteAt("Bot Wins", 120,10);
                LCD.WriteAt("Quit", quitButtonX,quitButtonY);
                LCD.DrawRectangle(quitButtonX-5,quitButtonY-5,65,25);  
            }
        }
        while(LCD.Touch(xpos, ypos)){
             if(*xpos>(quitButtonX-5) && *ypos>(returnButtonY-5) && *xpos<(quitButtonX+65) && *ypos<(returnButtonY+15)){
                LCD.SetFontColor(WHITE); 
                LCD.WriteAt("Quit", quitButtonX,quitButtonY);
                LCD.DrawRectangle(quitButtonX-5,quitButtonY-5,65,25);
                stage = 0;
                game = 0;
            }
            else{
                LCD.SetFontColor(BLACK);
                LCD.WriteAt("Quit", quitButtonX,quitButtonY);
                LCD.DrawRectangle(quitButtonX-5,quitButtonY-5,65,25);
                stage = 5;
                game = 1;
            }
        }
    }
    
    return game;
}

//fold function
int fold(int *round, int *stage, int *game, int *xpos, int *ypos, int *nextround){
    int x1 = 50, y1 = 100, x2 = 175, y2 = 100;
    LCD.Clear(GREEN);
    LCD.SetFontColor(BLACK);
    LCD.WriteAt("Are you sure you want to", 15, 25);
    LCD.WriteAt("fold?", 125, 50);
    LCD.DrawRectangle(x1, y1, 60, 25);
    LCD.WriteAt("Yes", x1+7, y1+5);
    LCD.DrawRectangle(x2, y2, 60, 25);
    LCD.WriteAt("No", x2+15, y2+5);
    while(LCD.Touch(xpos, ypos)){
        //checks if the user is pressing the yes button
        if(*xpos>x1 && *ypos>y1 && *xpos<(x1+60) && *ypos<(y1+30)){
            LCD.SetFontColor(WHITE);
            LCD.DrawRectangle(x1, y1, 60, 25);
            LCD.WriteAt("Yes", x1+7, y1+5); 
            *round = 0;
            *game = 1;
            *nextround = 1;
        }
        //checks if the user is pressing the no button
        if(*xpos>x2 && *ypos>y2 && *xpos<(x2+60) && *ypos<(y2+30)){
            LCD.SetFontColor(WHITE);
            LCD.DrawRectangle(x2, y2, 60, 25);
            LCD.WriteAt("No", x2+15, y2+5);
            *stage = 2;
            *game = 1;
            *round = 0;
        }
        LCD.Update();
        }
    
        return *round;
}
//nextRound button for if the user wants to fold on the round
int nextRound(int *nextround, int *stage, int *game, int *xpos, int *ypos, int *round){
    int x1= 50, y1=100, x2=125, y2=100;
    LCD.Clear(GREEN);
    LCD.SetFontColor(BLACK);
    LCD.WriteAt("Next Round or Quit?", 50, 20);
    LCD.DrawRectangle(x1, y1, 65, 25);
    LCD.WriteAt("Next", x1+10,y1+5);
    LCD.DrawRectangle(x2, y2, 65, 25);
    LCD.WriteAt("Quit", x2+10, y2+5);
    while(LCD.Touch(xpos, ypos)){
        //checks if the user wants to proceed to the next round
        if(*xpos>x1 && *ypos>y1 && *xpos<(x1+65) && *ypos<(y2+30)){
            LCD.SetFontColor(WHITE);
            LCD.DrawRectangle(x1, y1, 65, 25);
            LCD.WriteAt("Next", x1+10,y1+5);
            *nextround = 0;
            *stage = 2;
            *round = 0;
            *game = 1;
        }
        //checks if the user wants to quit the game and return to the menu screen
        if(*xpos>x2 && *ypos>y2 && *xpos<(x2+65) && *ypos<(y2+30)){
            LCD.SetFontColor(WHITE);
            LCD.DrawRectangle(x2, y2, 65, 25);
            LCD.WriteAt("Quit", x2+10, y2+5);
            *nextround = 0;
            *round = 0;
            *game= 0;
        }
        LCD.Update();
    }
    return *nextround;
}
//check function
int check(int *check, int *stage, int *game, int *xpos, int *ypos, int *p1turn, int *p2turn, int *tracking){
    int x1 = 100, y1= 100, p1 = *p1turn, p2 = *p2turn;
    LCD.Clear(GREEN);
    LCD.SetFontColor(BLACK);
    LCD.DrawRectangle(x1, y1, 70, 25);
    LCD.WriteAt("Check", x1, y1+5);
    while(LCD.Touch(xpos, ypos)){
        //checks if the user is pressing the check function
        if(*xpos>x1 && *ypos>y1 && *xpos<(x1+70) && *ypos<(y1+30)){
            LCD.SetFontColor(WHITE);
            LCD.DrawRectangle(x1, y1, 70, 25);
            LCD.WriteAt("Check", x1, y1+5);
            //if player 1 is pressing it, then p2 gets the chance
            if(p1 == 1){
                LCD.WriteAt("Player 1: Check?", x1-50, y1-50);
                *p1turn = 0;
                *p2turn = 1;
            }
            //is player 2 is pressing the button, then p1 gets the chance to make a decision
            if(p2==1){
                LCD.WriteAt("Player 2: Check?", x1-50, y1-50);
                *p1turn = 1;
                *p2turn = 0;
            }
            *check = 0;
            *game = 1;
            *stage = 2;
        }
        LCD.Update();
    }
    //if both players have made a decision
    *tracking = *tracking + 1;
    if(*tracking == 2){
        *tracking = 0;
    }
    return *check;
}
//bet function
int nextBet(int *bet,int *lastbetp1,int *lastbetp2, int *betting, int *game, int *xpos, int *ypos, int *menu, int *stage, int *PPlayer, int *POpponent, int *total, int *p1turn, int *p2turn, char *scorep, char *scoreo){
    int x1=50, y1=100, x2=110, y2=100, x3 =170, y3=100, x4=230, y4=100, mbsx = 264, mbsy = 5, temp = 0, bet10 = 10, p1 = *p1turn, p2 = *p2turn;
    LCD.Clear(GREEN);
    LCD.SetFontColor(BLACK);
    LCD.WriteAt("How much would you like", 25, 50);
    LCD.WriteAt("to bet?", 115, 70);
    LCD.DrawRectangle(x1, y1, 50, 25);
    LCD.WriteAt("10", x1+7, y1+5);
    LCD.DrawRectangle(x2, y2, 50, 25);
    LCD.WriteAt("20", x2+10, y2+5);
    LCD.DrawRectangle(x3, y3, 50, 25);
    LCD.WriteAt("50", x3+10, y3+5);
    LCD.DrawRectangle(x4, y4, 50, 25);
    LCD.WriteAt("100", x4+5, y4+5);
    LCD.DrawRectangle(100, 200, 60, 25);
    LCD.WriteAt("Back", 105, 205);
    int varp = *PPlayer, varo = *POpponent, var2 = *total;
    while(LCD.Touch(xpos, ypos)){
        //chekcs if the 10 button is being pressed and updates the pot and player balance
        if(*xpos>x1 && *ypos>y1 && *xpos<(x1+60) && *ypos<(y1+30)){
            LCD.SetFontColor(WHITE);
            LCD.DrawRectangle(x1, y1, 50, 25);
            LCD.WriteAt("10", x1+7, y1+5);
            if(p1 ==1 && *PPlayer>=10){
                *lastbetp1 =10;
                *bet=1;
                *PPlayer= varp -10;
                temp = *PPlayer;
                scorep[2] = static_cast<char>(48+temp%10);
                temp = temp/10;
                scorep[1] = static_cast<char>(48+temp%10);
                temp = temp/10;
                scorep[0] = static_cast<char>(48+temp%10);
                *total = var2 + 10;
                *stage = 2;
                *betting = 0;
                *menu =0;
                *p1turn = 0;
                *p2turn = 1;
            }
            if(p2==1&& *POpponent>=10){
                *lastbetp2 =10;
                *bet=2;
                *POpponent = varo - 10;
                temp = *POpponent;
                scoreo[2] = static_cast<char>(48+temp%10);
                temp = temp/10;
                scoreo[1] = static_cast<char>(48+temp%10);
                temp = temp/10;
                scoreo[0] = static_cast<char>(48+temp%10);
                *total = var2 + 10;
                *stage = 2;
                *betting = 0;
                *menu =0;
                *p2turn = 0;
                *p1turn = 1;
            }
            }
        //checks if the user is selecting to bet 20 and updating pot and player balance    
        else if(*xpos>x2 && *ypos>y2 && *xpos<(x2+50) && *ypos<(y2+25)){
            LCD.SetFontColor(WHITE);
            LCD.DrawRectangle(x2, y2, 50, 25);
            LCD.WriteAt("20", x2+10, y2+5);
            if(p1 ==1 && *PPlayer>=20){
                *lastbetp1 =20;
                *bet=1;
                *PPlayer= varp -20;
                temp = *PPlayer;
                scorep[2] = static_cast<char>(48+temp%10);
                temp = temp/10;
                scorep[1] = static_cast<char>(48+temp%10);
                temp = temp/10;
                scorep[0] = static_cast<char>(48+temp%10);
                *total = var2 + 20;
                *stage = 2;
                *betting = 0;
                *menu =0;
                *p1turn = 0;
                *p2turn = 1;
            }
            if(p2==1 && *POpponent>=20){
                *lastbetp2 =20;
                *bet=2;
                *POpponent = varo - 20;
                temp = *POpponent;
                scoreo[2] = static_cast<char>(48+temp%10);
                temp = temp/10;
                scoreo[1] = static_cast<char>(48+temp%10);
                temp = temp/10;
                scoreo[0] = static_cast<char>(48+temp%10);
                *total = var2 + 20;
                *stage = 2;
                *betting = 0;
                *menu =0;
                *p2turn = 0;
                *p1turn = 1;
            }
        }
        //checks if the user is selecting to bet 50 and updating pot and player balance 
        else if(*xpos>x3 && *ypos>y3 && *xpos<(x3+50) && *ypos<(y3+25)){
            LCD.SetFontColor(WHITE);
            LCD.DrawRectangle(x3, y3, 50, 25);
            LCD.WriteAt("50", x3+10, y3+5);
            if(p1 ==1&& *PPlayer>=50){
                *lastbetp1 =50;
                *bet=1;
                *PPlayer= varp -50;
                temp = *PPlayer;
                scorep[2] = static_cast<char>(48+temp%10);
                temp = temp/10;
                scorep[1] = static_cast<char>(48+temp%10);
                temp = temp/10;
                scorep[0] = static_cast<char>(48+temp%10);
                *total = var2 + 50;
                *stage = 2;
                *betting = 0;
                *menu =0;
                *p1turn = 0;
                *p2turn = 1;
            }
            if(p2==1&& *POpponent>=50){
                *lastbetp2 =50;
                *bet=2;
                *POpponent = varo - 50;
                temp = *POpponent;
                scoreo[2] = static_cast<char>(48+temp%10);
                temp = temp/10;
                scoreo[1] = static_cast<char>(48+temp%10);
                temp = temp/10;
                scoreo[0] = static_cast<char>(48+temp%10);
                *total = var2 + 50;
                *stage = 2;
                *betting = 0;
                *menu =0;
                *p2turn = 0;
                *p1turn = 1;
            }
        }
        //checks if the user is selecting to bet 100 and updating pot and player balance 
        else if(*xpos>x4 && *ypos>y4 && *xpos<(x4+50) && *ypos<(y4+25)){
            LCD.SetFontColor(WHITE);
            LCD.DrawRectangle(x4, y4, 50, 25);
            LCD.WriteAt("100", x4+5, y4+5);
            if(p1 ==1&& *PPlayer>=100){
                *lastbetp1 =100;
                *bet=1;
                *PPlayer= varp -100;
                temp = *PPlayer;
                scorep[2] = static_cast<char>(48+temp%10);
                temp = temp/10;
                scorep[1] = static_cast<char>(48+temp%10);
                temp = temp/10;
                scorep[0] = static_cast<char>(48+temp%10);
                *total = var2 + 100;
                *stage = 2;
                *betting = 0;
                *menu =0;
                *p1turn = 0;
                *p2turn = 1;
            }
            if(p2==1&& *POpponent>=100){
                *lastbetp2 =100;
                *bet=2;
                *POpponent = varo - 100;
                temp = *POpponent;
                scoreo[2] = static_cast<char>(48+temp%10);
                temp = temp/10;
                scoreo[1] = static_cast<char>(48+temp%10);
                temp = temp/10;
                scoreo[0] = static_cast<char>(48+temp%10);
                *total = var2 + 100;
                *stage = 2;
                *betting = 0;
                *menu =0;
                *p2turn = 0;
                *p1turn = 1;
            }
        }
        //checks for if the user wants to select the menu button in the bet screen
        else if(*xpos>mbsx && *ypos>mbsy && *xpos<(mbsx+45) && *ypos<(mbsy+15)){
            LCD.SetFontColor(WHITE);
            LCD.WriteAt("Menu", mbsx,mbsy);
            LCD.DrawRectangle(mbsx-7, mbsy-5, 63, 25);
            *stage = 2;
            *betting = 0;
            *menu = 1;
        }
        else if(*xpos>100 && *ypos>200 && *xpos<(160) && *ypos<(225)){
            LCD.SetFontColor(WHITE);
            LCD.DrawRectangle(100, 200, 60, 25);
            LCD.WriteAt("Back", 105, 205);
            *stage = 2;
            *betting = 0;
            *menu=0;
        }
        else{
            LCD.SetFontColor(BLACK);
            LCD.DrawRectangle(x1, y1, 50, 25);
            LCD.WriteAt("10", x1+5, y1+5);
            LCD.DrawRectangle(x2, y2, 50, 25);
            LCD.WriteAt("20", x2+5, y2+5); 
            LCD.DrawRectangle(x3, y3, 50, 25);
            LCD.WriteAt("50", x3+5, y3+5);
            LCD.DrawRectangle(x4, y4, 50, 25);
            LCD.WriteAt("100", x4+5, y4+5);
            LCD.DrawRectangle(100, 200, 60, 25);
            LCD.WriteAt("Back", 105, 205);
            *stage = -1;
            *betting = 1;
            *menu = 0;
            *bet=0;
        }
        LCD.Update();
    }
        
    return *betting;
}

//implementation of prototype in card class
void card::cardNameAsString(char *output){
        if(this->number>1&&this->number<10){
            *output = static_cast<char>(48+this->number);
        }
        else if(this->number==10){
            *output = static_cast<char>(49);
            *(output+1) = static_cast<char>(48);
        }
        else{
        switch(this->number){
            case 11:
            *(output) = 'J';
            *(output+1) = 'a';
            *(output+2) = 'c';
            *(output+3) = 'k';
            break;
            case 12:
            *(output) = 'Q';
            *(output+1) = 'u';
            *(output+2) = 'e';
            *(output+3) = 'e';
            *(output+4) = 'n';
            break;
            case 13:
            *(output) = 'K';
            *(output+1) = 'i';
            *(output+2) = 'n';
            *(output+3) = 'g';
            break;
            default:
            *(output) = 'A';
            *(output+1) = 'c';
            *(output+2) = 'e';
            break;
            }
        }
    }

//implementation of of prototype in class card
void card::copyFrom(card in){
    int cn = in.number;
    int cs = in.suite;
    char css[8] = {' '};
    css[0] = in.cardSuite[0];
    css[1] = in.cardSuite[1];
    css[2] = in.cardSuite[2];
    css[3] = in.cardSuite[3];
    css[4] = in.cardSuite[4];
    css[5] = in.cardSuite[5];
    css[6] = in.cardSuite[6];
    css[7] = in.cardSuite[7];
    this->number = cn;
    this->suite = cs;
    this->cardSuite[0] = css[0];
    this->cardSuite[1] = css[1];
    this->cardSuite[2] = css[2];
    this->cardSuite[3] = css[3];
    this->cardSuite[4] = css[4];
    this->cardSuite[5] = css[5];
    this->cardSuite[6] = css[6];
    this->cardSuite[7] = css[7];
}

//implementation of of prototype in class card
void card::toString(char *out){
    char name[32] = {0};
    char temp[32] = {0};
    this->cardNameAsString(name);
    for(int i  = 0; i<strlen(name); i++){
        temp[i] = name[i];
    }
    int a = strlen(temp);
    temp[a] = 'O';
    temp[a+1] = 'f';
    for(int i = 0; i<8; i++){
        temp[a+2+i] = this->cardSuite[i];
    }
    int b = strlen(temp);
    temp[b+1] = 's';
    for(int i = 0; i<32; i++){
        *(out+i)=temp[i];
    }
}
//checks if the win is a high card
int playerhigh(int player, deck *gamedeck){
    int high = 0;
    if(player==1){
        card player1card1 = (*gamedeck).getCardAt(0);
        card player1card2 = (*gamedeck).getCardAt(1);
        for(int i = 4; i<9; i++){
            card temp = (*gamedeck).getCardAt(i);
            if(temp.getNumber()>high){
                high=temp.getNumber();
            }
        }
        if(player1card1.getNumber()>high){
            high = player1card1.getNumber();
        }
        if(player1card2.getNumber()>high){
            high = player1card2.getNumber();
        }
    }
    else{
        card player2card1 = (*gamedeck).getCardAt(2);
        card player2card2 = (*gamedeck).getCardAt(3);
        for(int i = 4; i<9; i++){
            card temp = (*gamedeck).getCardAt(i);
            if(temp.getNumber()>high){
                high=temp.getNumber();
            }
        }
        if(player2card1.getNumber()>high){
            high = player2card1.getNumber();
        }
        if(player2card2.getNumber()>high){
            high = player2card2.getNumber();
        }
    }
    return high;
}
//checks if the win is a five of the same suite
int fiveOfTheSameSuite(int player, deck *gamedeck){
    int suiteoffive = 0;
    if(player == 1){
        int suites[7];
        suites[0] = (*gamedeck).getCardAt(0).getSuite();
        suites[1] = (*gamedeck).getCardAt(1).getSuite();
        for(int i =4; i<9; i++){
            suites[i-2] = (*gamedeck).getCardAt(i).getSuite();
        } 
        for(int i = 1; i<5; i++){
            int count = 0;
            for(int j = 0; j<7; j++){
                if(i==suites[i]){
                    count++;
                }
            }
            if(count>=5){
                suiteoffive = i;
            }
        }
    }
    else{
        int suites[7];
        suites[0] = (*gamedeck).getCardAt(2).getSuite();
        suites[1] = (*gamedeck).getCardAt(3).getSuite();
        for(int i =4; i<9; i++){
            suites[i-2] = (*gamedeck).getCardAt(i).getSuite();
        } 
        for(int i = 1; i<5; i++){
            int count = 0;
            for(int j = 0; j<7; j++){
                if(i==suites[i]){
                    count++;
                }
            }
            if(count>=5){
                suiteoffive = i;
            }
        }
    }
    return suiteoffive;
}
//checks if the hands can win as a royal flush
int isRoyalFlush(int *cardNumber,card a,card b,card c,card d,card e){
    int cardNumbers[5], isRoyalFlush = 0;
    cardNumbers[0] = a.getNumber(); 
    cardNumbers[1] = b.getNumber();
    cardNumbers[2] = c.getNumber();
    cardNumbers[3] = d.getNumber();
    cardNumbers[4] = e.getNumber();
    int j = 0,k=0,l=0,m=0,n=4;
    for(int i = 0; i<5; i++){
        if(i==0){
            j = 0,k = 1, l = 2,m = 3, n = 4;
        }
        if(i==1){
            j = 4,k = 0, l = 1,m = 2, n = 3;
        }
        if(i==2){
            j = 3,k = 4, l = 0,m = 1, n = 2;
        }
        if(i==3){
            j = 2,k = 3, l = 4,m = 0, n = 1;
        }
        if(i==4){
            j = 1,k = 2, l = 3,m = 4, n = 5;
        }
        if(cardNumbers[j]==1 && cardNumbers[k]==10 && cardNumbers[l] == 11 && cardNumbers[m] == 12 && cardNumbers[n] == 13){
            isRoyalFlush = allSameSuite(cardNumber, a,b,c,d,e);
        }
        if(cardNumbers[j]==1 && cardNumbers[k]==10 && cardNumbers[l] == 11 && cardNumbers[m] == 13 && cardNumbers[n] == 12){
            isRoyalFlush = allSameSuite(cardNumber, a,b,c,d,e);
        }
        if(cardNumbers[j]==1 && cardNumbers[k]==10 && cardNumbers[l] == 12 && cardNumbers[m] == 13 && cardNumbers[n] == 11){
            isRoyalFlush = allSameSuite(cardNumber, a,b,c,d,e);
        }
        if(cardNumbers[j]==1 && cardNumbers[k]==10 && cardNumbers[l] == 12 && cardNumbers[m] == 11 && cardNumbers[n] == 13){
            isRoyalFlush = allSameSuite(cardNumber, a,b,c,d,e);
        }
        if(cardNumbers[j]==1 && cardNumbers[k]==10 && cardNumbers[l] == 13 && cardNumbers[m] == 11 && cardNumbers[n] == 12){
            isRoyalFlush = allSameSuite(cardNumber, a,b,c,d,e);
        }
        if(cardNumbers[j]==1 && cardNumbers[k]==10 && cardNumbers[l] == 13 && cardNumbers[m] == 12 && cardNumbers[n] == 11){
            isRoyalFlush = allSameSuite(cardNumber, a,b,c,d,e);
        }
        if(cardNumbers[j]==1 && cardNumbers[k]==11 && cardNumbers[l] == 10 && cardNumbers[m] == 12 && cardNumbers[n] == 13){
            isRoyalFlush = allSameSuite(cardNumber, a,b,c,d,e);
        }
        if(cardNumbers[j]==1 && cardNumbers[k]==11 && cardNumbers[l] == 10 && cardNumbers[m] == 13 && cardNumbers[n] == 12){
            isRoyalFlush = allSameSuite(cardNumber, a,b,c,d,e);
        }
        if(cardNumbers[j]==1 && cardNumbers[k]==11 && cardNumbers[l] == 12 && cardNumbers[m] == 10 && cardNumbers[n] == 13){
            isRoyalFlush = allSameSuite(cardNumber, a,b,c,d,e);
        }
        if(cardNumbers[j]==1 && cardNumbers[k]==11 && cardNumbers[l] == 12 && cardNumbers[m] == 13 && cardNumbers[n] == 10){
            isRoyalFlush = allSameSuite(cardNumber, a,b,c,d,e);
        }
        if(cardNumbers[j]==1 && cardNumbers[k]==11 && cardNumbers[l] == 13 && cardNumbers[m] == 10 && cardNumbers[n] == 12){
            isRoyalFlush = allSameSuite(cardNumber, a,b,c,d,e);
        }
        if(cardNumbers[j]==1 && cardNumbers[k]==11 && cardNumbers[l] == 13 && cardNumbers[m] == 12 && cardNumbers[n] == 10){
            isRoyalFlush = allSameSuite(cardNumber, a,b,c,d,e);
        }
        if(cardNumbers[j]==1 && cardNumbers[k]==12 && cardNumbers[l] == 10 && cardNumbers[m] == 11 && cardNumbers[n] == 13){
            isRoyalFlush = allSameSuite(cardNumber, a,b,c,d,e);
        }
        if(cardNumbers[j]==1 && cardNumbers[k]==12 && cardNumbers[l] == 10 && cardNumbers[m] == 13 && cardNumbers[n] == 11){
            isRoyalFlush = allSameSuite(cardNumber, a,b,c,d,e);
        }
        if(cardNumbers[j]==1 && cardNumbers[k]==12 && cardNumbers[l] == 11 && cardNumbers[m] == 10 && cardNumbers[n] == 13){
            isRoyalFlush = allSameSuite(cardNumber, a,b,c,d,e);
        }
        if(cardNumbers[j]==1 && cardNumbers[k]==12 && cardNumbers[l] == 11 && cardNumbers[m] == 13 && cardNumbers[n] == 10){
            isRoyalFlush = allSameSuite(cardNumber, a,b,c,d,e);
        }
        if(cardNumbers[j]==1 && cardNumbers[k]==12 && cardNumbers[l] == 13 && cardNumbers[m] == 10 && cardNumbers[n] == 11){
            isRoyalFlush = allSameSuite(cardNumber, a,b,c,d,e);
        }
        if(cardNumbers[j]==1 && cardNumbers[k]==12 && cardNumbers[l] == 13 && cardNumbers[m] == 11 && cardNumbers[n] == 10){
            isRoyalFlush = allSameSuite(cardNumber, a,b,c,d,e);
        }
        if(cardNumbers[j]==1 && cardNumbers[k]==13 && cardNumbers[l] == 10 && cardNumbers[m] == 11 && cardNumbers[n] == 12){
            isRoyalFlush = allSameSuite(cardNumber, a,b,c,d,e);
        }
        if(cardNumbers[j]==1 && cardNumbers[k]==13 && cardNumbers[l] == 10 && cardNumbers[m] == 12 && cardNumbers[n] == 11){
            isRoyalFlush = allSameSuite(cardNumber, a,b,c,d,e);
        }
        if(cardNumbers[j]==1 && cardNumbers[k]==13 && cardNumbers[l] == 11 && cardNumbers[m] == 10 && cardNumbers[n] == 12){
            isRoyalFlush = allSameSuite(cardNumber, a,b,c,d,e);
        }
        if(cardNumbers[j]==1 && cardNumbers[k]==13 && cardNumbers[l] == 11 && cardNumbers[m] == 12 && cardNumbers[n] == 10){
            isRoyalFlush = allSameSuite(cardNumber, a,b,c,d,e);
        }
        if(cardNumbers[j]==1 && cardNumbers[k]==13 && cardNumbers[l] == 12 && cardNumbers[m] == 10 && cardNumbers[n] == 11){
            isRoyalFlush = allSameSuite(cardNumber, a,b,c,d,e);
        }
        if(cardNumbers[j]==1 && cardNumbers[k]==13 && cardNumbers[l] == 12 && cardNumbers[m] == 11 && cardNumbers[n] == 10){
            isRoyalFlush = allSameSuite(cardNumber, a,b,c,d,e);
        }
    }

    return isRoyalFlush;
}
//same as flush
int allSameSuite(int *cardNumber,card a,card b,card c,card d,card e){
    int suite1 = a.getSuite(), allSameFlag = 0;
    if(a.getNumber()>*cardNumber){
        *cardNumber = a.getNumber();
    }
    if(suite1==b.getSuite()){
        if (b.getNumber()>*cardNumber){
            *cardNumber = b.getNumber();
        }
        if(suite1==c.getSuite()){
            if (b.getNumber()>*cardNumber){
                *cardNumber = b.getNumber();
            }
            if(suite1==d.getSuite()){
                if (b.getNumber()>*cardNumber){
                    *cardNumber = b.getNumber();
                }
                if(suite1==e.getSuite()){
                    if (b.getNumber()>*cardNumber){
                        *cardNumber = b.getNumber();
                    }
                    allSameFlag = 1;
                }
            }
        }
    }
    return allSameFlag;
}
//checks if the win can be identified as a straight flush
int straightflush(int *cardNumber,card a,card b,card c,card d,card e){
    int blank, isStrightfFl =0, cardNumbers[5];
    cardNumbers[0] = a.getNumber(); 
    cardNumbers[1] = b.getNumber();
    cardNumbers[2] = c.getNumber();
    cardNumbers[3] = d.getNumber();
    cardNumbers[4] = e.getNumber();
    for(int i = 1; i<10;i++){
        int j = 0,k=0,l=0,m=0,n=4;
        for(int g = 0; g<5; g++){
        if(g==0){
            j = 0,k = 1, l = 2,m = 3, n = 4;
        }
        if(g==1){
            j = 4,k = 0, l = 1,m = 2, n = 3;
        }
        if(g==2){
            j = 3,k = 4, l = 0,m = 1, n = 2;
        }
        if(g==3){
            j = 2,k = 3, l = 4,m = 0, n = 1;
        }
        if(g==4){
            j = 1,k = 2, l = 3,m = 4, n = 5;
        }
        if(cardNumbers[j]==i && cardNumbers[k]==i+1 && cardNumbers[l] == i+2 && cardNumbers[m] == i+3 && cardNumbers[n] == i+4){
            isStrightfFl = allSameSuite(cardNumber, a,b,c,d,e);
        }
        if(cardNumbers[j]==i && cardNumbers[k]==i+1 && cardNumbers[l] == i+2 && cardNumbers[m] == i+4 && cardNumbers[n] == i+3){
            isStrightfFl = allSameSuite(cardNumber, a,b,c,d,e);
        }
        if(cardNumbers[j]==i && cardNumbers[k]==i+1 && cardNumbers[l] == i+3 && cardNumbers[m] == i+4 && cardNumbers[n] == i+2){
            isStrightfFl = allSameSuite(cardNumber, a,b,c,d,e);
        }
        if(cardNumbers[j]==i && cardNumbers[k]==i+1 && cardNumbers[l] == i+3 && cardNumbers[m] == i+2 && cardNumbers[n] == i+4){
            isStrightfFl = allSameSuite(cardNumber, a,b,c,d,e);
        }
        if(cardNumbers[j]==i && cardNumbers[k]==i+1 && cardNumbers[l] == i+4 && cardNumbers[m] == i+2 && cardNumbers[n] == i+3){
            isStrightfFl = allSameSuite(cardNumber, a,b,c,d,e);
        }
        if(cardNumbers[j]==i && cardNumbers[k]==i+1 && cardNumbers[l] == i+4 && cardNumbers[m] == i+3 && cardNumbers[n] == i+2){
            isStrightfFl = allSameSuite(cardNumber, a,b,c,d,e);
        }
        if(cardNumbers[j]==i && cardNumbers[k]==i+2 && cardNumbers[l] == i+1 && cardNumbers[m] == i+3 && cardNumbers[n] == i+4){
            isStrightfFl = allSameSuite(cardNumber, a,b,c,d,e);
        }
        if(cardNumbers[j]==i && cardNumbers[k]==i+2 && cardNumbers[l] == i+1 && cardNumbers[m] == i+4 && cardNumbers[n] == i+3){
            isStrightfFl = allSameSuite(cardNumber, a,b,c,d,e);
        }
        if(cardNumbers[j]==i && cardNumbers[k]==i+2 && cardNumbers[l] == i+3 && cardNumbers[m] == i+1 && cardNumbers[n] == i+4){
            isStrightfFl = allSameSuite(cardNumber, a,b,c,d,e);
        }
        if(cardNumbers[j]==i && cardNumbers[k]==i+2 && cardNumbers[l] == i+3 && cardNumbers[m] == i+4 && cardNumbers[n] == i+1){
            isStrightfFl = allSameSuite(cardNumber, a,b,c,d,e);
        }
        if(cardNumbers[j]==i && cardNumbers[k]==i+2 && cardNumbers[l] == i+4 && cardNumbers[m] == i+1 && cardNumbers[n] == i+3){
            isStrightfFl = allSameSuite(cardNumber, a,b,c,d,e);
        }
        if(cardNumbers[j]==i && cardNumbers[k]==i+2 && cardNumbers[l] == i+4 && cardNumbers[m] == i+3 && cardNumbers[n] == i+1){
            isStrightfFl = allSameSuite(cardNumber, a,b,c,d,e);
        }
        if(cardNumbers[j]==i && cardNumbers[k]==i+3 && cardNumbers[l] == i+1 && cardNumbers[m] == i+2 && cardNumbers[n] == i+4){
            isStrightfFl = allSameSuite(cardNumber, a,b,c,d,e);
        }
        if(cardNumbers[j]==i && cardNumbers[k]==i+3 && cardNumbers[l] == i+1 && cardNumbers[m] == i+4 && cardNumbers[n] == i+2){
            isStrightfFl = allSameSuite(cardNumber, a,b,c,d,e);
        }
        if(cardNumbers[j]==i && cardNumbers[k]==i+3 && cardNumbers[l] == i+2 && cardNumbers[m] == i+1 && cardNumbers[n] == i+4){
            isStrightfFl = allSameSuite(cardNumber, a,b,c,d,e);
        }
        if(cardNumbers[j]==i && cardNumbers[k]==i+3 && cardNumbers[l] == i+2 && cardNumbers[m] == i+4 && cardNumbers[n] == i+1){
            isStrightfFl = allSameSuite(cardNumber, a,b,c,d,e);
        }
        if(cardNumbers[j]==i && cardNumbers[k]==i+3 && cardNumbers[l] == i+4 && cardNumbers[m] == i+1 && cardNumbers[n] == i+2){
            isStrightfFl = allSameSuite(cardNumber, a,b,c,d,e);
        }
        if(cardNumbers[j]==i && cardNumbers[k]==i+3 && cardNumbers[l] == i+4 && cardNumbers[m] == i+2 && cardNumbers[n] == i+1){
            isStrightfFl = allSameSuite(cardNumber, a,b,c,d,e);
        }
        if(cardNumbers[j]==i && cardNumbers[k]==i+4 && cardNumbers[l] == i+1 && cardNumbers[m] == i+2 && cardNumbers[n] == i+3){
            isStrightfFl = allSameSuite(cardNumber, a,b,c,d,e);
        }
        if(cardNumbers[j]==i && cardNumbers[k]==i+4 && cardNumbers[l] == i+1 && cardNumbers[m] == i+3 && cardNumbers[n] == i+2){
            isStrightfFl = allSameSuite(cardNumber, a,b,c,d,e);
        }
        if(cardNumbers[j]==i && cardNumbers[k]==i+4 && cardNumbers[l] == i+2 && cardNumbers[m] == i+1 && cardNumbers[n] == i+3){
            isStrightfFl = allSameSuite(cardNumber, a,b,c,d,e);
        }
        if(cardNumbers[j]==i && cardNumbers[k]==i+4 && cardNumbers[l] == i+2 && cardNumbers[m] == i+3 && cardNumbers[n] == i+1){
            isStrightfFl = allSameSuite(cardNumber, a,b,c,d,e);
        }
        if(cardNumbers[j]==i && cardNumbers[k]==i+4 && cardNumbers[l] == i+3 && cardNumbers[m] == i+1 && cardNumbers[n] == i+2){
            isStrightfFl = allSameSuite(cardNumber, a,b,c,d,e);
        }
        if(cardNumbers[j]==i && cardNumbers[k]==i+4 && cardNumbers[l] == i+3 && cardNumbers[m] == i+2 && cardNumbers[n] == i+1){
            isStrightfFl = allSameSuite(cardNumber, a,b,c,d,e);
        }
    }
    }
    for(int i =0; i<5; i++){
        if(cardNumbers[i]>*cardNumber){
            *cardNumber=cardNumbers[i];
        }
    }
    return isStrightfFl;
}
//checks if the win can be registered as a four of a kind
int fourOfAKind(int *cardNumber, card a,card b,card c,card d,card e){
    int isfFourOfAKind =0, cardNumbers[5];
    cardNumbers[0] = a.getNumber(); 
    cardNumbers[1] = b.getNumber();
    cardNumbers[2] = c.getNumber();
    cardNumbers[3] = d.getNumber();
    cardNumbers[4] = e.getNumber();
    if(cardNumbers[0]==cardNumbers[1]&&cardNumbers[1]==cardNumbers[2]&&cardNumbers[2]==cardNumbers[3]){
        isfFourOfAKind=1;
        *cardNumber = cardNumbers[0];
        }
    if(cardNumbers[0]==cardNumbers[1]&&cardNumbers[1]==cardNumbers[3]&&cardNumbers[3]==cardNumbers[4]){
        isfFourOfAKind=2;
         *cardNumber = cardNumbers[0];
        }
    if(cardNumbers[0]==cardNumbers[1]&&cardNumbers[1]==cardNumbers[2]&&cardNumbers[2]==cardNumbers[4]){
        isfFourOfAKind=3;
     *cardNumber = cardNumbers[0];
    }
    if(cardNumbers[0]==cardNumbers[2]&&cardNumbers[2]==cardNumbers[3]&&cardNumbers[3]==cardNumbers[4]){
        isfFourOfAKind=4;
         *cardNumber = cardNumbers[0];
        }
    if(cardNumbers[1]==cardNumbers[2]&&cardNumbers[2]==cardNumbers[3]&&cardNumbers[3]==cardNumbers[4]){
        isfFourOfAKind=5;
         *cardNumber = cardNumbers[1];
        }
    return isfFourOfAKind;
}

int fullHouse(int *cardNumber1,int *cardNumber2,card a,card b,card c,card d,card e){
    int fullflag = 0, threePair =  threeOfAKind(cardNumber1,a,b,c,d,e);
    card nulla; 
    card nullb(-2,0);
    card nullc(-3,0);     
        if(threePair==1){
            fullflag = onePair(cardNumber2,d,e,nulla,nullb,nullc);
        }
        if(threePair==2){
            fullflag = onePair(cardNumber2,c,e,nulla,nullb,nullc);
        }
        if(threePair==3){
            fullflag = onePair(cardNumber2,c,d,nulla,nullb,nullc);
        }
        if(threePair==4){
            fullflag = onePair(cardNumber2,b,e,nulla,nullb,nullc);
        }
        if(threePair==5){
            fullflag = onePair(cardNumber2,b,d,nulla,nullb,nullc);
        }
        if(threePair==6){
            fullflag = onePair(cardNumber2,b,c,nulla,nullb,nullc);
        }
        if(threePair==7){
            fullflag = onePair(cardNumber2,a,e,nulla,nullb,nullc);
        }
        if(threePair==8){
            fullflag = onePair(cardNumber2,a,c,nulla,nullb,nullc);
        }
        if(threePair==9){
            fullflag = onePair(cardNumber2,a,d,nulla,nullb,nullc);
        }
        if(threePair==10){
            fullflag = onePair(cardNumber2,a,b,nulla,nullb,nullc);
        }
    return fullflag;
}
//checks if the win can be registered as a straight
int straight(int *cardNumber,card a,card b,card c,card d,card e){
int blank, isStrightfFl =0, cardNumbers[5];
    cardNumbers[0] = a.getNumber(); 
    cardNumbers[1] = b.getNumber();
    cardNumbers[2] = c.getNumber();
    cardNumbers[3] = d.getNumber();
    cardNumbers[4] = e.getNumber();
    for(int i = 1; i<10;i++){
        int j = 0,k=0,l=0,m=0,n=4;
    for(int g = 0; g<5; g++){
        if(g==0){
            j = 0,k = 1, l = 2,m = 3, n = 4;
        }
        if(g==1){
            j = 4,k = 0, l = 1,m = 2, n = 3;
        }
        if(g==2){
            j = 3,k = 4, l = 0,m = 1, n = 2;
        }
        if(g==3){
            j = 2,k = 3, l = 4,m = 0, n = 1;
        }
        if(g==4){
            j = 1,k = 2, l = 3,m = 4, n = 5;
        }
        if(cardNumbers[j]==i && cardNumbers[k]==i+1 && cardNumbers[l] == i+2 && cardNumbers[m] == i+3 && cardNumbers[n] == i+4){
            isStrightfFl = 1;
        }
        if(cardNumbers[j]==i && cardNumbers[k]==i+1 && cardNumbers[l] == i+2 && cardNumbers[m] == i+4 && cardNumbers[n] == i+3){
            isStrightfFl = 1;
        }
        if(cardNumbers[j]==i && cardNumbers[k]==i+1 && cardNumbers[l] == i+3 && cardNumbers[m] == i+4 && cardNumbers[n] == i+2){
            isStrightfFl = 1;
        }
        if(cardNumbers[j]==i && cardNumbers[k]==i+1 && cardNumbers[l] == i+3 && cardNumbers[m] == i+2 && cardNumbers[n] == i+4){
            isStrightfFl = 1;
        }
        if(cardNumbers[j]==i && cardNumbers[k]==i+1 && cardNumbers[l] == i+4 && cardNumbers[m] == i+2 && cardNumbers[n] == i+3){
            isStrightfFl = 1;
        }
        if(cardNumbers[j]==i && cardNumbers[k]==i+1 && cardNumbers[l] == i+4 && cardNumbers[m] == i+3 && cardNumbers[n] == i+2){
            isStrightfFl = 1;
        }
        if(cardNumbers[j]==i && cardNumbers[k]==i+2 && cardNumbers[l] == i+1 && cardNumbers[m] == i+3 && cardNumbers[n] == i+4){
            isStrightfFl = 1;
        }
        if(cardNumbers[j]==i && cardNumbers[k]==i+2 && cardNumbers[l] == i+1 && cardNumbers[m] == i+4 && cardNumbers[n] == i+3){
            isStrightfFl = 1;
        }
        if(cardNumbers[j]==i && cardNumbers[k]==i+2 && cardNumbers[l] == i+3 && cardNumbers[m] == i+1 && cardNumbers[n] == i+4){
            isStrightfFl = 1;
        }
        if(cardNumbers[j]==i && cardNumbers[k]==i+2 && cardNumbers[l] == i+3 && cardNumbers[m] == i+4 && cardNumbers[n] == i+1){
            isStrightfFl = 1;
        }
        if(cardNumbers[j]==i && cardNumbers[k]==i+2 && cardNumbers[l] == i+4 && cardNumbers[m] == i+1 && cardNumbers[n] == i+3){
            isStrightfFl = 1;
        }
        if(cardNumbers[j]==i && cardNumbers[k]==i+2 && cardNumbers[l] == i+4 && cardNumbers[m] == i+3 && cardNumbers[n] == i+1){
            isStrightfFl = 1;
        }
        if(cardNumbers[j]==i && cardNumbers[k]==i+3 && cardNumbers[l] == i+1 && cardNumbers[m] == i+2 && cardNumbers[n] == i+4){
            isStrightfFl = 1;
        }
        if(cardNumbers[j]==i && cardNumbers[k]==i+3 && cardNumbers[l] == i+1 && cardNumbers[m] == i+4 && cardNumbers[n] == i+2){
            isStrightfFl = 1;
        }
        if(cardNumbers[j]==i && cardNumbers[k]==i+3 && cardNumbers[l] == i+2 && cardNumbers[m] == i+1 && cardNumbers[n] == i+4){
            isStrightfFl = 1;
        }
        if(cardNumbers[j]==i && cardNumbers[k]==i+3 && cardNumbers[l] == i+2 && cardNumbers[m] == i+4 && cardNumbers[n] == i+1){
            isStrightfFl = 1;
        }
        if(cardNumbers[j]==i && cardNumbers[k]==i+3 && cardNumbers[l] == i+4 && cardNumbers[m] == i+1 && cardNumbers[n] == i+2){
            isStrightfFl = 1;
        }
        if(cardNumbers[j]==i && cardNumbers[k]==i+3 && cardNumbers[l] == i+4 && cardNumbers[m] == i+2 && cardNumbers[n] == i+1){
            isStrightfFl = 1;
        }
        if(cardNumbers[j]==i && cardNumbers[k]==i+4 && cardNumbers[l] == i+1 && cardNumbers[m] == i+2 && cardNumbers[n] == i+3){
            isStrightfFl = 1;
        }
        if(cardNumbers[j]==i && cardNumbers[k]==i+4 && cardNumbers[l] == i+1 && cardNumbers[m] == i+3 && cardNumbers[n] == i+2){
            isStrightfFl = 1;
        }
        if(cardNumbers[j]==i && cardNumbers[k]==i+4 && cardNumbers[l] == i+2 && cardNumbers[m] == i+1 && cardNumbers[n] == i+3){
            isStrightfFl = 1;
        }
        if(cardNumbers[j]==i && cardNumbers[k]==i+4 && cardNumbers[l] == i+2 && cardNumbers[m] == i+3 && cardNumbers[n] == i+1){
            isStrightfFl = 1;
        }
        if(cardNumbers[j]==i && cardNumbers[k]==i+4 && cardNumbers[l] == i+3 && cardNumbers[m] == i+1 && cardNumbers[n] == i+2){
            isStrightfFl = 1;
        }
        if(cardNumbers[j]==i && cardNumbers[k]==i+4 && cardNumbers[l] == i+3 && cardNumbers[m] == i+2 && cardNumbers[n] == i+1){
            isStrightfFl = 1;
        }
    }
    }
    for(int i =0; i<5; i++){
        if(cardNumbers[i]>*cardNumber){
            *cardNumber=cardNumbers[i];
        }
    }
    return isStrightfFl;
}
//checks if the win can be registered as a triplet
int threeOfAKind(int *cardNumber,card a,card b,card c,card d,card e){
int isThreeOfAKind =0, cardNumbers[5];
    cardNumbers[0] = a.getNumber(); 
    cardNumbers[1] = b.getNumber();
    cardNumbers[2] = c.getNumber();
    cardNumbers[3] = d.getNumber();
    cardNumbers[4] = e.getNumber();
if(cardNumbers[0]==cardNumbers[1]&&cardNumbers[1]==cardNumbers[2]){
    isThreeOfAKind = 1;
    *cardNumber = cardNumbers[0];
    }
if(cardNumbers[0]==cardNumbers[1]&&cardNumbers[1]==cardNumbers[3]){
    isThreeOfAKind = 2;
    *cardNumber = cardNumbers[0];
    }
if(cardNumbers[0]==cardNumbers[1]&&cardNumbers[1]==cardNumbers[4]){
    isThreeOfAKind = 3;
    *cardNumber = cardNumbers[0];
    }
if(cardNumbers[0]==cardNumbers[2]&&cardNumbers[2]==cardNumbers[3]){
    isThreeOfAKind = 4;
    *cardNumber = cardNumbers[0];
    }
if(cardNumbers[0]==cardNumbers[2]&&cardNumbers[2]==cardNumbers[4]){
    isThreeOfAKind = 5;
    *cardNumber = cardNumbers[0];
    }
if(cardNumbers[0]==cardNumbers[3]&&cardNumbers[3]==cardNumbers[4]){
    isThreeOfAKind = 6;
    *cardNumber = cardNumbers[0];
    }
if(cardNumbers[1]==cardNumbers[2]&&cardNumbers[2]==cardNumbers[3]){
    isThreeOfAKind = 7;
    *cardNumber = cardNumbers[1];
    }
if(cardNumbers[1]==cardNumbers[3]&&cardNumbers[3]==cardNumbers[4]){
    isThreeOfAKind = 8;
    *cardNumber = cardNumbers[1];
    }
if(cardNumbers[1]==cardNumbers[2]&&cardNumbers[2]==cardNumbers[4]){
    isThreeOfAKind = 9;
    *cardNumber = cardNumbers[1];
    }
if(cardNumbers[2]==cardNumbers[3]&&cardNumbers[3]==cardNumbers[4]){
    isThreeOfAKind = 10;
    *cardNumber = cardNumbers[2];
    }
return isThreeOfAKind;
}
//checks if the win is a two pair
int twoPair(int *cardNumber1,int *cardNumber2,card a,card b,card c,card d,card e){
    int isTwoPair =0, cardNumbers[5];
    cardNumbers[0] = a.getNumber(); 
    cardNumbers[1] = b.getNumber();
    cardNumbers[2] = c.getNumber();
    cardNumbers[3] = d.getNumber();
    cardNumbers[4] = e.getNumber();
    if(cardNumbers[0]==cardNumbers[1]&&cardNumbers[2]==cardNumbers[3]){
        isTwoPair = 1;
        *cardNumber1 = cardNumbers[0];
        *cardNumber2 = cardNumbers[2];
        }
    if(cardNumbers[0]==cardNumbers[1]&&cardNumbers[2]==cardNumbers[4]){
        isTwoPair = 2;
        *cardNumber1 = cardNumbers[0];
        *cardNumber2 = cardNumbers[2];
        }
    if(cardNumbers[0]==cardNumbers[1]&&cardNumbers[3]==cardNumbers[4]){
        isTwoPair = 3;
        *cardNumber1 = cardNumbers[0];
        *cardNumber2 = cardNumbers[3];
        }
    if(cardNumbers[0]==cardNumbers[2]&&cardNumbers[3]==cardNumbers[4]){
        isTwoPair = 4;
        *cardNumber1 = cardNumbers[0];
        *cardNumber2 = cardNumbers[3];
        }
    if(cardNumbers[0]==cardNumbers[4]&&cardNumbers[2]==cardNumbers[3]){
        isTwoPair = 5;
        *cardNumber1 = cardNumbers[0];
        *cardNumber2 = cardNumbers[3];
        }
    if(cardNumbers[0]==cardNumbers[3]&&cardNumbers[2]==cardNumbers[4]){
        isTwoPair = 6;
        *cardNumber1 = cardNumbers[0];
        *cardNumber2 = cardNumbers[2];
        }
    if(cardNumbers[1]==cardNumbers[3]&&cardNumbers[2]==cardNumbers[4]){
        isTwoPair = 7;
        *cardNumber1 = cardNumbers[1];
        *cardNumber2 = cardNumbers[2];
        }
    if(cardNumbers[1]==cardNumbers[2]&&cardNumbers[3]==cardNumbers[4]){
        isTwoPair = 8;
        *cardNumber1 = cardNumbers[1];
        *cardNumber2 = cardNumbers[3];
        }
    if(cardNumbers[2]==cardNumbers[3]&&cardNumbers[1]==cardNumbers[4]){
        isTwoPair = 9;
        *cardNumber1 = cardNumbers[3];
        *cardNumber2 = cardNumbers[1];
        }
    if(cardNumbers[2]==cardNumbers[4]&&cardNumbers[1]==cardNumbers[4]){
        isTwoPair = 10;
        *cardNumber1 = cardNumbers[2];
        *cardNumber2 = cardNumbers[1];
        }
    return isTwoPair;
}
//checks if the win is a one pair
int onePair(int *cardNumber,card a,card b,card c,card d,card e){
    int hasPair =0, cardNumbers[5];
    cardNumbers[0] = a.getNumber(); 
    cardNumbers[1] = b.getNumber();
    cardNumbers[2] = c.getNumber();
    cardNumbers[3] = d.getNumber();
    cardNumbers[4] = e.getNumber();
    if(cardNumbers[0]==cardNumbers[1]){
        hasPair = 1;
        *cardNumber = cardNumbers[0];
        }
    if(cardNumbers[0]==cardNumbers[2]){
        hasPair = 2;
        *cardNumber = cardNumbers[0];
        }
    if(cardNumbers[0]==cardNumbers[3]){
        hasPair = 3;
        *cardNumber = cardNumbers[0];
        }
    if(cardNumbers[0]==cardNumbers[4]){
        hasPair = 4;
        *cardNumber = cardNumbers[0];
        }
    if(cardNumbers[1]==cardNumbers[2]){
        hasPair = 5;
        *cardNumber = cardNumbers[1];
        }
    if(cardNumbers[1]==cardNumbers[3]){
        hasPair = 6;
        *cardNumber = cardNumbers[1];
        }
    if(cardNumbers[1]==cardNumbers[4]){
        hasPair = 7;
        *cardNumber = cardNumbers[1];
        }
    if(cardNumbers[2]==cardNumbers[3]){
        hasPair = 8;
        *cardNumber = cardNumbers[2];
        }
    if(cardNumbers[2]==cardNumbers[4]){
        hasPair = 9;
        *cardNumber = cardNumbers[2];
        }
    if(cardNumbers[3]==cardNumbers[4]){
        hasPair = 10;
        *cardNumber = cardNumbers[3];
        }
    return hasPair;
}
//decision is a 1x2 vector. The first column is what action, and the 2nd is how much to raise.
//for column 1: 1 = raise, 2 = check, 3 = fold
//for column 2 will be a number of points to raise if the first column is a 1.
void aggresiveBot(int cardsOnField, deck *gamedeck, int *decision){
    int deci, bet;
    int confid = 0;
    card hand1 = (*gamedeck).getCardAt(2), hand2 = (*gamedeck).getCardAt(3);
    card onField1 = (*gamedeck).getCardAt(4),onField2 = (*gamedeck).getCardAt(5),onField3 = (*gamedeck).getCardAt(6),onField4 = (*gamedeck).getCardAt(7),onField5 = (*gamedeck).getCardAt(8);
    int cardNumbers[7], cardSuites[7], fill;
    cardNumbers[0] = hand1.getNumber();
    cardNumbers[1] = hand2.getNumber();
    cardNumbers[2] = onField1.getNumber();
    cardNumbers[3] = onField2.getNumber();
    cardNumbers[4] = onField3.getNumber();
    cardNumbers[5] = onField4.getNumber();
    cardNumbers[6] = onField5.getNumber();
    cardSuites[0] = hand1.getSuite();
    cardSuites[1] = hand2.getSuite();
    cardSuites[2] = onField1.getSuite();
    cardSuites[3] = onField2.getSuite();
    cardSuites[4] = onField3.getSuite();
    cardSuites[5] = onField4.getSuite();
    cardSuites[6] = onField5.getSuite();
    if(cardsOnField<3){
        confid=25;
    }
    else if(cardsOnField==3){
        if(hand1.getNumber()==onField1.getNumber()||hand1.getNumber()==onField2.getNumber()||hand1.getNumber()==onField3.getNumber()){
            confid = confid + 10;
        }
        if(hand2.getNumber()==onField1.getNumber()||hand2.getNumber()==onField2.getNumber()||hand2.getNumber()==onField3.getNumber()){
            confid = confid + 10;
        }
        if(hand1.getNumber()==hand2.getNumber()){
            confid = confid + 15;
        }
        if(hand1.getSuite()==hand2.getSuite()&&(hand1.getSuite()==onField1.getSuite()||hand1.getSuite()==onField2.getSuite()||hand1.getSuite()==onField3.getSuite())){
            confid = confid + 5;
        }
        if(hand1.getSuite()==hand2.getSuite()&&((hand1.getSuite()==onField1.getSuite()&&hand1.getSuite()==onField2.getSuite())||(hand1.getSuite()==onField1.getSuite()&&hand1.getSuite()==onField3.getSuite())||(hand1.getSuite()==onField2.getSuite()&&hand1.getSuite()==onField3.getSuite()))){
            confid = confid + 25;
        }
    }
    else if(cardsOnField == 4){
        if(hand1.getNumber()==hand2.getNumber()){
            confid = confid + 15;
        }
        if(cardNumbers[2]==cardNumbers[3]||cardNumbers[2]==cardNumbers[4]||cardNumbers[2]==cardNumbers[5]||cardNumbers[3]==cardNumbers[4]||cardNumbers[3]==cardNumbers[5]||cardNumbers[4]==cardNumbers[5]){
            confid +=10;
        }
        if((cardNumbers[0]==cardNumbers[2]&&cardNumbers[2]==cardNumbers[3])||(cardNumbers[0]==cardNumbers[2]&&cardNumbers[2]==cardNumbers[4])||(cardNumbers[0]==cardNumbers[2]&&cardNumbers[2]==cardNumbers[5])||(cardNumbers[0]==cardNumbers[3]&&cardNumbers[3]==cardNumbers[4])||(cardNumbers[0]==cardNumbers[3]&&cardNumbers[3]==cardNumbers[5])||(cardNumbers[0]==cardNumbers[4]&&cardNumbers[4]==cardNumbers[5])){
            confid +=10;
        }
        if((cardNumbers[1]==cardNumbers[2]&&cardNumbers[2]==cardNumbers[3])||(cardNumbers[1]==cardNumbers[2]&&cardNumbers[2]==cardNumbers[4])||(cardNumbers[1]==cardNumbers[2]&&cardNumbers[2]==cardNumbers[5])||(cardNumbers[1]==cardNumbers[3]&&cardNumbers[3]==cardNumbers[4])||(cardNumbers[1]==cardNumbers[3]&&cardNumbers[3]==cardNumbers[5])||(cardNumbers[1]==cardNumbers[4]&&cardNumbers[4]==cardNumbers[5])){
            confid +=10;   
        }
        if((cardNumbers[0]==cardNumbers[2]||cardNumbers[0]==cardNumbers[3])||(cardNumbers[0]==cardNumbers[4]||cardNumbers[0]==cardNumbers[4])||(cardNumbers[0]==cardNumbers[5])){
            confid +=10;   
        }
        if((cardNumbers[1]==cardNumbers[2]||cardNumbers[1]==cardNumbers[3])||(cardNumbers[1]==cardNumbers[4]||cardNumbers[1]==cardNumbers[4])||(cardNumbers[1]==cardNumbers[5])){
            confid +=10;  
        }
        if(fourOfAKind(&fill, hand1,hand2,onField1,onField2,onField3)||fourOfAKind(&fill, hand1,hand2,onField2,onField3,onField4)||fourOfAKind(&fill, hand1,hand2,onField3,onField4,onField5)||fourOfAKind(&fill, hand1,hand2,onField1,onField4,onField5)||fourOfAKind(&fill, hand1,hand2,onField2,onField3,onField4)||fourOfAKind(&fill, hand1,onField1,onField2,onField3,onField4)||fourOfAKind(&fill,hand2,onField1,onField2,onField3,onField4)){
            confid +=10;
        }
        if(fullHouse(&fill, &fill,hand1,hand2,onField1,onField2,onField3)||fullHouse(&fill,&fill, hand1,hand2,onField2,onField3,onField4)||fullHouse(&fill, &fill,hand1,hand2,onField3,onField4,onField5)||fullHouse(&fill,&fill, hand1,hand2,onField1,onField4,onField5)||fullHouse(&fill, &fill,hand1,hand2,onField2,onField3,onField4)||fullHouse(&fill,&fill, hand1,onField1,onField2,onField3,onField4)||fullHouse(&fill,&fill,hand2,onField1,onField2,onField3,onField4)){
            confid +=10;
        }
        if((cardSuites[0]==cardSuites[1]&&cardSuites[1]==cardSuites[2]&&cardSuites[2]==cardSuites[3])||(cardSuites[0]==cardSuites[1]&&cardSuites[1]==cardSuites[2]&&cardSuites[2]==cardSuites[4])||(cardSuites[0]==cardSuites[1]&&cardSuites[1]==cardSuites[3]&&cardSuites[3]==cardSuites[4])||(cardSuites[0]==cardSuites[1]&&cardSuites[1]==cardSuites[2]&&cardSuites[2]==cardSuites[5])||(cardSuites[0]==cardSuites[1]&&cardSuites[1]==cardSuites[3]&&cardSuites[3]==cardSuites[5])||(cardSuites[0]==cardSuites[2]&&cardSuites[2]==cardSuites[3]&&cardSuites[3]==cardSuites[4])||(cardSuites[0]==cardSuites[2]&&cardSuites[2]==cardSuites[3]&&cardSuites[3]==cardSuites[5])||(cardSuites[0]==cardSuites[3]&&cardSuites[3]==cardSuites[4]&&cardSuites[4]==cardSuites[5])||(cardSuites[1]==cardSuites[2]&&cardSuites[2]==cardSuites[3]&&cardSuites[3]==cardSuites[4])||(cardSuites[1]==cardSuites[2]&&cardSuites[2]==cardSuites[3]&&cardSuites[3]==cardSuites[5])||(cardSuites[1]==cardSuites[2]&&cardSuites[2]==cardSuites[4]&&cardSuites[4]==cardSuites[5])||(cardSuites[1]==cardSuites[3]&&cardSuites[3]==cardSuites[4]&&cardSuites[4]==cardSuites[5])||(cardSuites[2]==cardSuites[3]&&cardSuites[3]==cardSuites[4]&&cardSuites[4]==cardSuites[5])){
            confid +=10;
        }
    }
    else{
        confid = playerBestStats(&fill,&fill, hand1,hand2,onField1,onField2,onField3,onField4,onField5)*10;
    }

    if(confid<10){
        srand(time(NULL));
        int r = rand()%2;
        if(r==1){
        *(decision) = 2;
        }
        else{
        *(decision) = 3; 
        }
    }
    else if(confid>10&&confid<35){
        *(decision) = 2; 
    }
    else if(confid>35&&confid<50){
        srand(time(NULL));
        int r = rand()%60;
        if(r>40&&r<50){
            *(decision) = 1;
            *(decision+1) = 10;
        }
        else if(r<50){
            *(decision) = 1;
            *(decision+1) = 20;
        }
        else{
            *(decision) = 2; 
        }
    }
    else if(confid>50){
        *(decision) = 1;
        *(decision+1) = confid;
    }
}
//returns the most common suite on the field
int suitesOnField(card a, card b, card c, card d, card e){
    int cardSuites[5];
    int spades, clubs, diamonds, hearts;
    for(int i=0;i<5;i++){
        switch(cardSuites[i]){
            case(1):
            spades++;
            break;
            case(2):
            hearts++;
            break;
            case(3):
            diamonds++;
            break;
            case(4):
            clubs++;
            break;
        }
    }
    if(spades>=diamonds&&spades>=clubs&&spades>=hearts){
        return 1;
    }
    if(hearts>=diamonds&&hearts>=clubs&&hearts>=spades){
        return 2;
    }
    if(diamonds>=spades&&diamonds>=clubs&&diamonds>=hearts){
        return 3;
    }
    if(clubs>=diamonds&&clubs>=spades&&clubs>=hearts){
        return 4;
    }
    return 0;
}
//determiens the winner of the round
int currentHandWinner(deck *gamedeck){
    card p1c1 = (*gamedeck).getCardAt(0),p1c2= (*gamedeck).getCardAt(1),p2c1= (*gamedeck).getCardAt(2),p2c2= (*gamedeck).getCardAt(3),comCard1= (*gamedeck).getCardAt(4),comCard2= (*gamedeck).getCardAt(5),comCard3= (*gamedeck).getCardAt(6),comCard4= (*gamedeck).getCardAt(7),comCard5= (*gamedeck).getCardAt(8);
    int comCardsHighest = 0;
    int comHighestCard = 0, highestCardAlt = 0; 
    int winner = 0;
    if(onePair(&comHighestCard,comCard1,comCard2,comCard3,comCard4,comCard5)){
        comCardsHighest = 2;
    }
    if(twoPair(&comHighestCard,&highestCardAlt,comCard1,comCard2,comCard3,comCard4,comCard5)){
        comCardsHighest = 3;
    }
    if(threeOfAKind(&comHighestCard,comCard1,comCard2,comCard3,comCard4,comCard5)){
        comCardsHighest = 4;
    }
    if(straight(&comHighestCard,comCard1,comCard2,comCard3,comCard4,comCard5)){
        comCardsHighest = 5;
    }
    if(allSameSuite(&comHighestCard,comCard1,comCard2,comCard3,comCard4,comCard5)){
        comCardsHighest = 6;
    }
    if(fullHouse(&comHighestCard,&highestCardAlt,comCard1,comCard2,comCard3,comCard4,comCard5)){
        comCardsHighest = 7;
    }
    if(fourOfAKind(&comHighestCard,comCard1,comCard2,comCard3,comCard4,comCard5)){
        comCardsHighest = 8;    
    }
    if(straightflush(&comHighestCard,comCard1,comCard2,comCard3,comCard4,comCard5)){
        comCardsHighest = 9;
    }
    if(isRoyalFlush(&comHighestCard,comCard1,comCard2,comCard3,comCard4,comCard5)){
        comCardsHighest = 10;
    }
    int suitep1,suitep2,p1rank,p2rank;
    int p1HighestNum = 0,p1Highest2ndNum = 0,p2HighestNum = 0,p2Highest2ndNum = 0;
    p1rank = playerBestStats(&p1HighestNum,&p1Highest2ndNum,p1c1,p1c2,comCard1,comCard2,comCard3,comCard4,comCard5);
    p2rank = playerBestStats(&p2HighestNum,&p2Highest2ndNum,p2c1,p2c2,comCard1,comCard2,comCard3,comCard4,comCard5);
    if(p1c1.getSuite()>p1c2.getSuite()){
        suitep1 = p1c1.getSuite();
    }
    else{
        suitep1 = p1c1.getSuite();
    }
    if(p2c1.getSuite()>p2c2.getSuite()){
        suitep2 = p2c1.getSuite();
    }
    else{
        suitep2 = p2c1.getSuite();
    }
    if(p1rank==comCardsHighest&&p1rank==p2rank){
        winner = 0;
    }

    if(p1rank==p2rank){
        if(p1HighestNum==p2HighestNum){
            if(p1Highest2ndNum!=0){
                if(p1HighestNum==p2HighestNum){
                    if(suitep1>suitep2){
                        winner = 1;
                    }
                    else if(suitep1<suitep2){
                        winner = 2;
                    }
                }
                else if(p1HighestNum>p2HighestNum){
                    winner = 1;
                }
                else if(p1HighestNum<p2HighestNum){
                    winner = 2;
                }
            }
        }
        else if(p1HighestNum>p2HighestNum){
            winner = 1;
        }
        else if(p1HighestNum<p2HighestNum){
            winner = 2;
        }
    }
    else if(p1rank>p2rank){
        winner = 1;
    }
    else if(p1rank<p2rank){
        winner = 2;
    }
    return winner;
}
//determines the best hand
int bestHand(int *cardNumber1,int *cardNumber2, card a,card b,card c,card d,card e){
    int bestRank= 0;
    if(onePair(cardNumber1,a,b,c,d,e)){
        bestRank = 2;
    }
    if(twoPair(cardNumber1,cardNumber2,a,b,c,d,e)){
        bestRank = 3;
    }
    if(threeOfAKind(cardNumber1,a,b,c,d,e)){
        bestRank = 4;
    }
    if(allSameSuite(cardNumber1,a,b,c,d,e)){
        bestRank = 5;
    }
    if(straight(cardNumber1,a,b,c,d,e)){
        bestRank = 6;
    }
    if(fullHouse(cardNumber1,cardNumber2,a,b,c,d,e)){
        bestRank = 7;
    }
    if(fourOfAKind(cardNumber1,a,b,c,d,e)){
        bestRank = 8;    
    }
    if(straightflush(cardNumber1,a,b,c,d,e)){
        bestRank = 9;
    }
    if(isRoyalFlush(cardNumber1,a,b,c,d,e)){
        bestRank = 10;
    }  
    return bestRank;
}
//player best stats
int playerBestStats(int *cardNumber1,int *cardNumber2, card pc1, card pc2, card a,card b,card c,card d,card e){
    int tempHighNum = 0, temp2ndHighestNum = 0, HighestNum = 0, Highest2ndNum = 0, bestRank = 0,TempRank = 0;
    card cardss[7];
    cardss[0].copyFrom(pc1);
    cardss[1].copyFrom(pc2);
    cardss[2].copyFrom(a);
    cardss[3].copyFrom(b);
    cardss[4].copyFrom(c);
    cardss[5].copyFrom(d);
    cardss[6].copyFrom(e);
    TempRank = bestHand(&tempHighNum,&temp2ndHighestNum,cardss[0],cardss[1],cardss[2],cardss[3],cardss[4]);
    if(TempRank>bestRank){
        bestRank=TempRank;
        HighestNum = tempHighNum;
        Highest2ndNum = temp2ndHighestNum;
    }
    TempRank = bestHand(&tempHighNum,&temp2ndHighestNum,cardss[0],cardss[1],cardss[2],cardss[3],cardss[5]);
    if(TempRank>bestRank){
        bestRank=TempRank;
        HighestNum = tempHighNum;
        Highest2ndNum = temp2ndHighestNum;
    }
    TempRank = bestHand(&tempHighNum,&temp2ndHighestNum,cardss[0],cardss[1],cardss[2],cardss[3],cardss[6]);
    if(TempRank>bestRank){
        bestRank=TempRank;
        HighestNum = tempHighNum;
        Highest2ndNum = temp2ndHighestNum;
    }
    TempRank = bestHand(&tempHighNum,&temp2ndHighestNum,cardss[0],cardss[1],cardss[2],cardss[4],cardss[5]);
    if(TempRank>bestRank){
        bestRank=TempRank;
        HighestNum = tempHighNum;
        Highest2ndNum = temp2ndHighestNum;
    }
    TempRank = bestHand(&tempHighNum,&temp2ndHighestNum,cardss[0],cardss[1],cardss[2],cardss[4],cardss[6]);
    if(TempRank>bestRank){
        bestRank=TempRank;
        HighestNum = tempHighNum;
        Highest2ndNum = temp2ndHighestNum;
    }
    TempRank = bestHand(&tempHighNum,&temp2ndHighestNum,cardss[0],cardss[1],cardss[2],cardss[5],cardss[6]);
    if(TempRank>bestRank){
        bestRank=TempRank;
        HighestNum = tempHighNum;
        Highest2ndNum = temp2ndHighestNum;
    }
    TempRank = bestHand(&tempHighNum,&temp2ndHighestNum,cardss[0],cardss[1],cardss[3],cardss[4],cardss[5]);
    if(TempRank>bestRank){
        bestRank=TempRank;
        HighestNum = tempHighNum;
        Highest2ndNum = temp2ndHighestNum;
    }
    TempRank = bestHand(&tempHighNum,&temp2ndHighestNum,cardss[0],cardss[1],cardss[3],cardss[4],cardss[6]);
    if(TempRank>bestRank){
        bestRank=TempRank;
        HighestNum = tempHighNum;
        Highest2ndNum = temp2ndHighestNum;
    }
    TempRank = bestHand(&tempHighNum,&temp2ndHighestNum,cardss[0],cardss[1],cardss[3],cardss[5],cardss[6]);
    if(TempRank>bestRank){
        bestRank=TempRank;
        HighestNum = tempHighNum;
        Highest2ndNum = temp2ndHighestNum;
    }
    TempRank = bestHand(&tempHighNum,&temp2ndHighestNum,cardss[0],cardss[1],cardss[4],cardss[5],cardss[6]);
    if(TempRank>bestRank){
        bestRank=TempRank;
        HighestNum = tempHighNum;
        Highest2ndNum = temp2ndHighestNum;
    }
    TempRank = bestHand(&tempHighNum,&temp2ndHighestNum,cardss[0],cardss[2],cardss[3],cardss[4],cardss[5]);
    if(TempRank>bestRank){
        bestRank=TempRank;
        HighestNum = tempHighNum;
        Highest2ndNum = temp2ndHighestNum;
    }
    TempRank = bestHand(&tempHighNum,&temp2ndHighestNum,cardss[0],cardss[2],cardss[3],cardss[4],cardss[6]);
    if(TempRank>bestRank){
        bestRank=TempRank;
        HighestNum = tempHighNum;
        Highest2ndNum = temp2ndHighestNum;
    }
    TempRank = bestHand(&tempHighNum,&temp2ndHighestNum,cardss[0],cardss[2],cardss[3],cardss[5],cardss[6]);
    if(TempRank>bestRank){
        bestRank=TempRank;
        HighestNum = tempHighNum;
        Highest2ndNum = temp2ndHighestNum;
    }
    TempRank = bestHand(&tempHighNum,&temp2ndHighestNum,cardss[0],cardss[2],cardss[4],cardss[5],cardss[6]);
    if(TempRank>bestRank){
        bestRank=TempRank;
        HighestNum = tempHighNum;
        Highest2ndNum = temp2ndHighestNum;
    }
    TempRank = bestHand(&tempHighNum,&temp2ndHighestNum,cardss[0],cardss[3],cardss[4],cardss[5],cardss[6]);
    if(TempRank>bestRank){
        bestRank=TempRank;
        HighestNum = tempHighNum;
        Highest2ndNum = temp2ndHighestNum;
    }
    TempRank = bestHand(&tempHighNum,&temp2ndHighestNum,cardss[1],cardss[2],cardss[3],cardss[4],cardss[5]);
    if(TempRank>bestRank){
        bestRank=TempRank;
        HighestNum = tempHighNum;
        Highest2ndNum = temp2ndHighestNum;
    }
    TempRank = bestHand(&tempHighNum,&temp2ndHighestNum,cardss[1],cardss[2],cardss[3],cardss[4],cardss[6]);
    if(TempRank>bestRank){
        bestRank=TempRank;
        HighestNum = tempHighNum;
        Highest2ndNum = temp2ndHighestNum;
    }
    TempRank = bestHand(&tempHighNum,&temp2ndHighestNum,cardss[1],cardss[2],cardss[3],cardss[5],cardss[6]);
    if(TempRank>bestRank){
        bestRank=TempRank;
        HighestNum = tempHighNum;
        Highest2ndNum = temp2ndHighestNum;
    }
    TempRank = bestHand(&tempHighNum,&temp2ndHighestNum,cardss[1],cardss[2],cardss[4],cardss[5],cardss[6]);
    if(TempRank>bestRank){
        bestRank=TempRank;
        HighestNum = tempHighNum;
        Highest2ndNum = temp2ndHighestNum;
    }
    TempRank = bestHand(&tempHighNum,&temp2ndHighestNum,cardss[1],cardss[3],cardss[4],cardss[5],cardss[6]);
    if(TempRank>bestRank){
        bestRank=TempRank;
        HighestNum = tempHighNum;
        Highest2ndNum = temp2ndHighestNum;
    }
    TempRank = bestHand(&tempHighNum,&temp2ndHighestNum,cardss[2],cardss[3],cardss[4],cardss[5],cardss[6]);
    if(TempRank>bestRank){
        bestRank=TempRank;
        HighestNum = tempHighNum;
        Highest2ndNum = temp2ndHighestNum;
    }
    *cardNumber1 = HighestNum;
    *cardNumber2 = Highest2ndNum;
    return bestRank;
}
//sets bot difficulty to easy
void conservitiveBot(int cardsOnField, deck *gamedeck, int *decision, int *reraised, int oppraised){
    int confid =0, fill = 0, bid = 0;
    card hand1 = (*gamedeck).getCardAt(2), hand2 = (*gamedeck).getCardAt(3);
    card onField1 = (*gamedeck).getCardAt(4),onField2 = (*gamedeck).getCardAt(5),onField3 = (*gamedeck).getCardAt(6),onField4 = (*gamedeck).getCardAt(7),onField5 = (*gamedeck).getCardAt(8);
    int cardNumbers[7], cardSuites[7];
    cardNumbers[0] = hand1.getNumber();
    cardNumbers[1] = hand2.getNumber();
    cardNumbers[2] = onField1.getNumber();
    cardNumbers[3] = onField2.getNumber();
    cardNumbers[4] = onField3.getNumber();
    cardNumbers[5] = onField4.getNumber();
    cardNumbers[6] = onField5.getNumber();
    cardSuites[0] = hand1.getSuite();
    cardSuites[1] = hand2.getSuite();
    cardSuites[2] = onField1.getSuite();
    cardSuites[3] = onField2.getSuite();
    cardSuites[4] = onField3.getSuite();
    cardSuites[5] = onField4.getSuite();
    cardSuites[6] = onField5.getSuite();
    if(cardsOnField==0){
        if(cardNumbers[0]<7&&cardNumbers[0]>1&&cardNumbers[0]!=cardNumbers[1]&&cardNumbers[1]!=1){
            confid = 0;
        }    
        else if(cardNumbers[1]<7&&cardNumbers[1]>1&&cardNumbers[0]!=cardNumbers[1]&&cardNumbers[0]!=1){
            confid = 0;
        }
        else if(cardNumbers[0]<9&&(cardNumbers[1]!=13||cardNumbers[1]!=1)){
            confid = 0;
        }
        else if(cardNumbers[1]<9&&(cardNumbers[0]!=13||cardNumbers[0]!=1)){
            confid = 0;
        }
        else{
            confid = 25;
        }

    }
    else if(cardsOnField==3){
        if(cardNumbers[0]==cardNumbers[1]){
            if(cardNumbers[0]==13||cardNumbers[0]==1){
                confid = 55;
                bid = 30;
            }
            else if(cardNumbers[0]==12||cardNumbers[0]==11){
                if(!(*reraised)){
                confid = 55;
                bid = 20;
                }
                else{
                confid = 45;    
                }
            }
        }
        else{
            confid = 45;
        }
    }
    else if(cardsOnField==4){
       int suite = suitesOnField(onField1,onField2,onField3,onField4,onField5);
       if(suite == cardSuites[0]&&cardSuites[0]==cardSuites[1]){
            if(cardNumbers[0]>9&&cardNumbers[1]>9){
                confid = 40;
            }
            else{
                confid = 10;
            }
        
       }
       else{
        srand(time(NULL));
        int r = rand()%3;
        if(oppraised){
            if(!((cardNumbers[0]==1&&cardNumbers[1]>7)||(cardNumbers[0]==13&&cardNumbers[1]>9)||(cardNumbers[0]==12&&cardNumbers[1]>9)||(cardNumbers[0]==11&&cardNumbers[1]==10))){
            if(r==1){
            confid = 1;
            }
            if(r==1){
            confid = 25;
            }
            if(r==1){
            confid = 45;
            }
            }
            else if(!((cardNumbers[1]==1&&cardNumbers[0]>7)||(cardNumbers[1]==13&&cardNumbers[0]>9)||(cardNumbers[1]==12&&cardNumbers[0]>9)||(cardNumbers[1]==11&&cardNumbers[0]==10))){
            if(r==1){
            confid = 1;
            }
            if(r==1){
            confid = 25;
            }
            if(r==1){
            confid = 45;
            }
            }
            else{
                confid = 10;
            }
        }
        else{
            confid = 30;
        }
       } 
    }
    else if(cardsOnField==5){
        confid = playerBestStats(&fill,&fill, hand1,hand2,onField1,onField2,onField3,onField4,onField5)*10;
    }
    if(confid<10){
        srand(time(NULL));
        int r = rand()%2;
        if(r==1){
        *(decision) = 2;
        }
        else{
        *(decision) = 3; 
        }
    }
    else if(confid>10&&confid<35){
        *(decision) = 2; 
    }
    else if(confid>35&&confid<50){
        srand(time(NULL));
        int r = rand()%60;
        if(r>40&&r<50){
            *(decision) = 1;
            *(decision+1) = 10;
        }
        else if(r<50){
            *(decision) = 1;
            *(decision+1) = 20;
        }
        else{
            *(decision) = 2; 
        }
    }
    else if(confid>50){
        *(decision) = 1;
        if(bid){
            *(decision+1) = bid;
        }
        else{
        *(decision+1) = confid;
        }
    }
}
//implementation of prototype in class deck
card deck::removeTopCard(){
    card temp;
    temp.copyFrom(this->cardDeck[this->atCard]);
    this->atCard = this->atCard+1;
    return temp;
    }

//implementation of prototype in class deck
void deck::shuffleCards(){
    int permu[52] = {0};
    int flag = 0, count = 0;
    card tempdeck[52];
    srand(time(NULL));
    int r = rand()%52;
    for(int i = 0; i<52; i++){
        permu[i]=-1;
    }
    while(count<52){
        r = rand()%52;
        for(int i = count; i>-1; i--){
            if(permu[i]==r){
                flag = 1;
            }
        }
        if(!flag){
            permu[count] = r;
            count++;
        }
        flag = 0;
    }
    for (int i =0; i<52; i++){
        tempdeck[i].copyFrom(this->cardDeck[permu[i]]);
    }
    for(int i =0; i<52; i++){
        this->cardDeck[i].copyFrom(tempdeck[i]);
    }
    }

    //implementation of prototype in class deck
    void deck::print(){
        for(int i = 0; i<52; i++){
            char st[64] = {0};
            this->cardDeck[i].toString(st);
            for(int j = 0; j<strlen(st);j++){
                fprintf(stdout, "%c", st[j]);
            }
            fprintf(stdout, "\n");
        }
        fprintf(stdout, "\n");
    }