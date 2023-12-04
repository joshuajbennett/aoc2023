#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>

// To run build the code and then do the following command:
// ```
// cat ../day3/in.txt | day3/day3part1
// ```

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

typedef struct Number {
    int num;
} Number; 

int main()
{
    int totalScore = 0;
    while (true)
    {
        size_t len = 0;
        ssize_t lineSize = 0;
        char *line = NULL;
        lineSize = getline(&line, &len, stdin);
        if (lineSize == -1)
        {
            break;
        }

        int x;
        for (x = 0; x < lineSize; ++x)
        {
            if (line[x] == ':')
            {
                x++;
                break;
            }
        }
        Number* winningNumbers = malloc(0);
        int numWinningNumbers = 0;
        for (; x < lineSize; ++x)
        {
            if (line[x] == ' ')
            {
                continue;
            }
            if (line[x] == '|')
            {
                x++;
                break;
            }
            int num = parseNumber(x, line, lineSize);
            while (isdigit(line[x]))
            {
                x++;
            }
            numWinningNumbers++;
            winningNumbers = realloc(winningNumbers, numWinningNumbers * sizeof(Number)); 
            winningNumbers[numWinningNumbers-1].num = num;
        }
        int score = 0;
        for (; x < lineSize; ++x)
        {
            if (line[x] == ' ')
            {
                continue;
            }
            if (line[x] == '\n')
            {
                break;
            }
            int num = parseNumber(x, line, lineSize);
            while (isdigit(line[x]))
            {
                x++;
            }
            int i;
            for (i = 0; i < numWinningNumbers; ++i)
            {
                if (num == winningNumbers[i].num)
                {
                    if (score == 0)
                    {
                        score = 1;
                    }
                    else
                    {
                        score *= 2;
                    }
                }
            }
        }
        totalScore += score;

        free(line);
    }
        
    printf("Part sum: %d \n", totalScore);
    return 0;
}
