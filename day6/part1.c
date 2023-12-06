#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <limits.h>

// To run build the code and then do the following command:
// ```
// cat ../day6/in.txt | day6/day6part1
// ```

long parseNumber(int index, char* line, ssize_t lineSize)
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
    long number;
    number = atol(numberStr);
    return number;
}

int main()
{
    size_t len = 0;
    ssize_t lineSize = 0;
    long* times = NULL;
    int numTimes = 0;
    long* distances = NULL;
    int numDistances = 0;
    char *line = NULL;
    lineSize = getline(&line, &len, stdin);
    int i;
    for (i = 0; i < lineSize && line[i] != ':'; ++i) {}
    ++i;
    for (; i < lineSize; ++i)
    {
        for (; i < lineSize && line[i] == ' '; ++i) {}
        long num = parseNumber(i, line, lineSize);
        numTimes++;
        times = realloc(times, numTimes*sizeof(long));
        times[numTimes-1] = num;
        for (; i < lineSize && isdigit(line[i]); ++i) {}
        ++i;
    }
    free(line);
    line = NULL;
    lineSize = getline(&line, &len, stdin);
    for (i = 0; i < lineSize && line[i] != ':'; ++i) {}
    ++i;
    for (; i < lineSize; ++i)
    {
        for (; i < lineSize && line[i] == ' '; ++i) {}
        long num = parseNumber(i, line, lineSize);
        numDistances++;
        distances = realloc(distances, numDistances*sizeof(long));
        distances[numDistances-1] = num;
        for (; i < lineSize && isdigit(line[i]); ++i) {}
        ++i;
    }
    free(line);
    
    long product = 1; 
    for (i = 0; i < numTimes; ++i)
    {
        long numWaysToBeat = 0;
        long j;
        for (j = 0; j < times[i]; ++j)
        {
            long remainingTime = times[i] - j;
            long distance = remainingTime * j;
            if (distance > distances[i])
            {
                numWaysToBeat++;
            }
        }
        printf("Ways to beat: %ld\n", numWaysToBeat);
        product *= numWaysToBeat;
    }

    printf("Answer: %ld\n", product);
    return 0;
}
