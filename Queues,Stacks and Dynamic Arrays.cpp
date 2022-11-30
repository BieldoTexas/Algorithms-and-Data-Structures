#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct card
{
    int value;
    char suit;
    struct card* next;

}Card;

typedef struct player
{
    short int isPlaying;
    Card* head;
    Card* tail;
    int nCards;

}Player;

typedef struct cuscuz
{
    Card* top;
    Card* bot;
    int nCards;
}Cuscuz;

void initialize_players(Player** players,int numPlayers)
{
    for(int i = 0;i<numPlayers;i++)
    {
        (*players)[i].head = (Card*) malloc(sizeof(Card));
        (*players)[i].tail = (*players)[i].head;
        (*players)[i].isPlaying = 1;
        (*players)[i].nCards = 0;
    }
}

void initialize_cuscuz(Cuscuz** Cuscuz)
{
    (*Cuscuz)->bot = (Card*) malloc(sizeof(Card));
    (*Cuscuz)->nCards = 0;
    (*Cuscuz)->top = (*Cuscuz)->bot;
}

void makeCard(Card** card,int value,int suit)
{
    (*card)->value = value;
    (*card)->suit = suit;
    (*card)->next = NULL;
}

void enqueue(Card** tail, int value, char suit)
{   
    Card* card = (Card*) malloc(sizeof(Card));
    makeCard(&card,value,suit);
    (*tail)->next = card;
    (*tail) = card;
}

void Deal(Player** players, int numPlayers)
{   
    int value;
    char suit;

    for(int i = 0;i<numPlayers;i++)
    {
        scanf("%d%c",&value,&suit);
        if((*players)[i].isPlaying)
        {
            enqueue(&((*players)[i].tail),value,suit);
            (*players)[i].nCards++;
        }
    }
}

void Round(Player** players,int numPlayers,Cuscuz** cuscuz,int* R,int* endFlag)
{
    int draw = 1;
    while(draw)
    {
        draw = 0;
        int saveIndex;
        Card cards[numPlayers],biggestCard;

        biggestCard.value = 0;

        for(int i =0;i<numPlayers;i++)
        {
            if((*players)[i].isPlaying)
            {
                Card* tmp = (*players)[i].head->next;
                (*players)[i].head->next = (*players)[i].head->next->next;
                
                if((*players)[i].nCards == 1)
                    (*players)[i].tail = (*players)[i].head;

                if((*cuscuz)->nCards == 0)
                {
                    (*cuscuz)->bot = tmp;
                    (*cuscuz)->top->next = tmp;
                    tmp->next = NULL;
                }
                else
                {
                    tmp->next = (*cuscuz)->top->next;
                    (*cuscuz)->top->next = tmp;
                }
                
                (*cuscuz)->nCards++;
                (*players)[i].nCards--;

                cards[i].value = tmp->value;
                cards[i].suit = tmp->suit;
            }
            else
                cards[i].value = -1;
        }
        
        for(int i = 0;i<numPlayers;i++)
        {
            if(cards[i].value>biggestCard.value)
            {
                biggestCard = cards[i];
                saveIndex = i;
            }
            else if(cards[i].value == biggestCard.value && cards[i].suit > biggestCard.suit)
            {
                biggestCard = cards[i];
                saveIndex = i;
            }
        }

        int counter = 0;

        for(int i = 0;i<numPlayers;i++)
            if(cards[i].value == biggestCard.value && biggestCard.suit == cards[i].suit)
                counter++;
        
        if(counter>1)
            draw = 1;

        if(!draw)
        {
            (*players)[saveIndex].tail->next = (*cuscuz)->top->next;
            (*players)[saveIndex].tail = (*cuscuz)->bot;
            (*players)[saveIndex].nCards+=(*cuscuz)->nCards;
            printf("%d %d %d\n",*R,saveIndex,(*cuscuz)->nCards);
            initialize_cuscuz(cuscuz);
            (*R)++;
            
            for(int i = 0;i<numPlayers;i++)
                if((*players)[i].nCards == 0)
                    (*players)[i].isPlaying = 0;
            
            for(int i = 0;i<numPlayers;i++)
                if((*players)[i].isPlaying == 0)
                    counter++;

            if(numPlayers-counter==1)
            {
                printf("%d\n",saveIndex);
                draw = 0;
                *endFlag = 1;
            }
        }
        else
        {
            for(int i = 0;i<numPlayers;i++)
                if((*players)[i].nCards == 0)
                    (*players)[i].isPlaying = 0;

            short int flag = 0;
            for(int i = 0;i<numPlayers && !flag;i++)
                if((*players)[i].isPlaying)
                    flag = 1;
            
            if(!flag)
            {
                printf("-1 -1 -1\n");
                draw = 0;
                *endFlag = 1;
            }
            else
                draw = 1;
        }
    }
}

void End(Player* players,int numPlayers,int* endFlag)
{
    int nCards = 0, indexWinner, counter = 0;
    
    for(int i = 0;i<numPlayers;i++)
    {
        if(players[i].nCards > nCards)
        {
            indexWinner = i;
            nCards = players[i].nCards;
        }
    }
        
    for(int i = 0;i<numPlayers;i++)
        if(nCards==players[i].nCards)
            counter++;
        
    if(counter==1)
        printf("%d\n",indexWinner);
    else
        printf("-1\n");
            
    *endFlag = 1;
}

int main()
{
    int endFlag = 0,nPlayers,rndCounter = 0,nCards = 0,indexWinner,counter = 0;
    char event[3];
    
    scanf("%d",&nPlayers);
    Player* Players = (Player*) malloc(sizeof(Player) * nPlayers);
    initialize_players(&Players,nPlayers);

    Cuscuz* cuscuz = (Cuscuz*) malloc(sizeof(Cuscuz));
    initialize_cuscuz(&cuscuz);

     while(!endFlag) 
    {
        scanf("%s",event);
        if(strncmp("DEA",event,3) == 0)
            Deal(&Players,nPlayers);
            
        else if(strncmp("RND",event,3) == 0)
            Round(&Players,nPlayers,&cuscuz,&rndCounter,&endFlag);

        else if(strncmp("END",event,3) == 0)
            End(Players,nPlayers,&endFlag);
    }
    
    return 0;
}