#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>

// To run build the code and then do the following command:
// ```
// cat ../day2/in.txt | day2/day2part1
// ```

bool extractColorNumber(char *colors[3], char *line, ssize_t lineSize, int index, int* colorNumber)
{
    int i, j;
    for (i = 0; i < 3; i++)
    {
        bool foundMatch;
        foundMatch = true;
        for (j = 0; colors[i][j] != '\0'; j++)
        {
            if (j+index >= lineSize)
            {
                foundMatch = false;
                break;
            }
            if (colors[i][j] != line[j+index])
            {
                foundMatch = false;
                break;
            }
        }
        if (foundMatch)
        {

            (*colorNumber) = i;
            return true;
        }
    }
    return false;
}

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

int main()
{
    size_t len = 0;
    ssize_t lineSize = 0;

    char *colorNames[3];
    colorNames[0] = "red";
    colorNames[1] = "green";
    colorNames[2] = "blue";

    int colorAmounts[3];
    colorAmounts[0] = 12;
    colorAmounts[1] = 13;
    colorAmounts[2] = 14;

    int sum = 0;
    int numWithinRange = 0;
    while (true)
    {
        char *line = NULL;
        lineSize = getline(&line, &len, stdin);
        if (lineSize == -1)
        {
            break;
        }
        int gameId;
        gameId = parseNumber(5, line, lineSize);
  
        bool withinRange = true; 
        int i; 
        for (i = 5; i < lineSize && line[i] != ':' ; ++i)
        {
        }
        while (true)
        {
            int num = parseNumber(i+2, line, lineSize);
            i += 2;
            for (; i < lineSize && line[i] != ' ' ; ++i)
            {
            }
            int colorNumber;

            if (!extractColorNumber(colorNames, line, lineSize, i+1, &colorNumber))
            {
                break;
            }
            if (num > colorAmounts[colorNumber])
            {
                withinRange = false;
            }
            for (; i < lineSize && line[i] != ',' && line[i] != ';'; ++i)
            {
            }
            if (i == lineSize)
            {
                if (withinRange)
                {
                    numWithinRange++;
                    sum+=gameId;
                };
                break;
            }
        }
                
        free(line);
    }
    printf("Sum within range: %d", sum);
    return 0;
}
