#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

// To run build the code and then do the following command:
// ```
// cat ../day1/in.txt | day1/day1part2
// ```

bool extractNumber(char *characters[9], char *line, ssize_t lineSize, int index, char* number)
{
    int i, j;
    for (i = 0; i < 9; i++)
    {
        bool foundMatch;
        foundMatch = true; 
        for (j = 0; characters[i][j] != '\0'; j++)
        {
            if (j+index >= lineSize)
            {
                foundMatch = false;
                break;
            }
            if (characters[i][j] != line[j+index])
            {
                foundMatch = false;
                break;
            }
        }
        if (foundMatch)
        {

            (*number) = i + 1 + '0';
            return true;
        }
    }
    return false;
}

int main()
{
    size_t len = 0;
    ssize_t lineSize = 0;
    int allSum = 0;
    char *characters[9];
    characters[0] = "one";
    characters[1] = "two";
    characters[2] = "three";
    characters[3] = "four";
    characters[4] = "five";
    characters[5] = "six";
    characters[6] = "seven";
    characters[7] = "eight";
    characters[8] = "nine";
    
    while (true)
    {
        char *line = NULL;
        lineSize = getline(&line, &len, stdin);
        if (lineSize == -1)
        {
            break;
        }
        char firstDigit;
        char lastDigit;
        int i;
        bool foundFirstDigit;
        foundFirstDigit = false;
        for (i = 0; i < lineSize; ++i)
        {
            if (isdigit(line[i]))
            {
                if (!foundFirstDigit)
                {
                    firstDigit = line[i];
                    foundFirstDigit = true;
                }
                lastDigit = line[i];
            }
            char number;
            if (extractNumber(characters, line, lineSize, i, &number))
            {
                if (!foundFirstDigit)
                {
                    firstDigit = number;
                    foundFirstDigit = true;
                }
                lastDigit = number;
            }
        }
        char firstLast[3];
        firstLast[0] = firstDigit;
        firstLast[1] = lastDigit;
        firstLast[2] = '\0';
        int digit;
        digit = atoi(firstLast);
        allSum += digit;
        free(line);
    }
    printf("All Sum: %d.\n", allSum);
    return 0;
}
