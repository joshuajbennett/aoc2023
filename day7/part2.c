#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <limits.h>
#include <math.h>

// To run build the code and then do the following command:
// ```
// cat ../day7/in.txt | day7/day7part1
// ```

typedef enum HandType {
    HighCard = 0,
    OnePair = 1,
    TwoPair = 2,
    ThreeOfaKind = 3,
    FullHouse = 4,
    FourOfaKind = 5,
    FiveOfaKind = 6
} HandType;

typedef struct Hand {
    HandType type;
    long totalStrength;
    int bid;
} Hand;

int parseNumber(int index, char* line, ssize_t lineSize)
{
    int i;
    int numberStart = index;
    int numberEnd;
    for (i = numberStart; i < lineSize; ++i)
    {
        if (!isdigit(line[i]))
        {
            break;
        }
        numberEnd = i;
    } 
    char numberStr[numberEnd-numberStart+2];
    numberStr[numberEnd-numberStart+1] = '\0';
    strncpy(numberStr, line + numberStart, numberEnd-numberStart+1);
    int number;
    number = atoi(numberStr);
    return number;
}

char *relativeStrength = "J23456789TQKA";

int extractRelativeStrength(char card)
{
    int j;
    for (j = 0; j < 13; ++j)
    {
        if (card == relativeStrength[j])
        {
            return j;
        }
    }
    return 13;
}

void parseHand(char* line, ssize_t lineSize, Hand* hand)
{
    int numCards = 13;
    int cardCount[13] = {0};
    int i;
    hand->totalStrength = 0;
    for (i = 0; i < 5; ++i)
    {
        int strength = extractRelativeStrength(line[i]);
        cardCount[strength]++;
        hand->totalStrength += pow(numCards, 5-i) * strength;
    }
    hand->bid = parseNumber(6, line, lineSize);
   
    bool foundType = false; 
    int numThrees = 0;
    int numTwos = 0;
    // First parse without the jokers.
    for (i = 1; i < 13; ++i)
    {
        if (cardCount[i] == 3)
        {
            numThrees++;
        }
        if (cardCount[i] == 2)
        {
            numTwos++;
        }
        if (cardCount[i] == 5)
        {
            foundType = true;
            hand->type = FiveOfaKind;
        }     
        if (cardCount[i] == 4)
        {
            foundType = true;
            hand->type = FourOfaKind;
        }
    }
    if (!foundType)
    {
        if (numThrees == 1 && numTwos == 1)
        {
            hand->type = FullHouse;
        } 
        else if (numThrees == 1)
        {
            hand->type = ThreeOfaKind;
        }
        else if (numTwos == 2)
        {
            hand->type = TwoPair;
        }
        else if (numTwos == 1)
        {
            hand->type = OnePair;
        }
        else
        {
            hand->type = HighCard;
        }
    }
    if (cardCount[0] == 1)
    {
        if (hand->type == FourOfaKind)
        {
            hand->type = FiveOfaKind;
        }
        else if (hand->type == ThreeOfaKind)
        {
            hand->type = FourOfaKind;
        }
        else if (hand->type == TwoPair)
        {
            hand->type = FullHouse;
        }
        else if (hand->type == OnePair)
        {
            hand->type = ThreeOfaKind;
        }
        else {
            hand->type = OnePair;
        }

    }
    if (cardCount[0] == 2)
    {
        if (hand->type == ThreeOfaKind)
        {
            hand->type = FiveOfaKind;
        }
        else if (hand->type == OnePair)
        {
            hand->type = FourOfaKind;
        }
        else
        {
            hand->type = ThreeOfaKind;
        }
    }
    if (cardCount[0] == 3)
    {
        if (hand->type == OnePair)
        {
            hand->type = FiveOfaKind;
        }
        else
        {
            hand->type = FourOfaKind;
        }
    }
    if (cardCount[0] == 4)
    {
        hand->type = FiveOfaKind;
    }
    if (cardCount[0] == 5)
    {
        hand->type = FiveOfaKind;
    }

}

void swap(Hand** hand1, Hand** hand2)
{
    Hand* temp;
    temp = *hand1;
    *hand1 = *hand2;
    *hand2 = temp;
}

int main()
{
    Hand** hands = NULL;
    int numHands = 0; 
    size_t len = 0;
    ssize_t lineSize = 0;
    while(true)
    {
       char *line = NULL;
       lineSize = getline(&line, &len, stdin);
       if (lineSize == -1)
       {
           break;
       }
       numHands++;
       hands = realloc(hands, numHands * sizeof(Hand*));
       hands[numHands-1] = calloc(1, sizeof(Hand));
       parseHand(line, lineSize, hands[numHands-1]);
       free(line);
    }
    int i;
    for (i = 0; i < numHands; ++i)
    {
        int j;
        for (j = 0; j < numHands-1; ++j)
        {
            Hand* first = hands[j];
            Hand* second = hands[j+1];
            if (first->type > second->type)
            {
                swap(&hands[j], &hands[j+1]);
            }
            if (first->type == second->type)
            {
                if (first->totalStrength > second->totalStrength)
                {
                    swap(&hands[j], &hands[j+1]);
                }
            }
        } 
    }
    long sum = 0;
    for (i = 0; i < numHands; ++i)
    {
        int rank = i + 1;
        sum += hands[i]->bid * rank;
    }
    
    printf("Answer: %ld\n", sum);
    return 0;
}
