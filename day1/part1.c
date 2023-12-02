#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

// To run build the code and then do the following command:
// ```
// cat ../day1/in.txt | day1/day1part1
// ```

int main()
{
    size_t len = 0;
    ssize_t lineSize = 0;
    int allSum = 0;
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
        }
        char firstLast[2];
        firstLast[0] = firstDigit;
        firstLast[1] = lastDigit;
        int digit;
        digit = atoi(firstLast);
        allSum += digit;
        free(line);
    }
    printf("All Sum: %d.\n", allSum);
    return 0;
}
