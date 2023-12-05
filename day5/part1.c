#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <limits.h>

// To run build the code and then do the following command:
// ```
// cat ../day5/in.txt | day5/day5part1
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

typedef struct MapEntry {
    long origin;
    long destination; 
    long width;
} MapEntry;
#define CREATE_MAP_ENTRY_PTR(X) MapEntry* X = calloc(1, sizeof(MapEntry))

typedef struct Map {
    MapEntry** entries;
    int numEntries;
} Map;
void appendMapEntry(Map* map, MapEntry* entry)
{
    map->numEntries++;
    map->entries = realloc(map->entries, map->numEntries * sizeof(MapEntry));
    map->entries[map->numEntries-1] = entry;
}
long processValue(Map* map, long value) 
{
    int i;
    for (i = 0; i < map->numEntries; ++i)
    {
        MapEntry* entry = map->entries[i];
        if ((value >= entry->origin) && (value <= (entry->origin + entry->width - 1)))
        {
            return entry->destination + (value - entry->origin);
        }
    }
    return value;
}

int main()
{
    // Parse seeds.
    long* seeds = malloc(0);
    int numSeeds = 0;
    size_t len = 0;
    ssize_t lineSize = 0;
    char *firstLine = NULL;
    lineSize = getline(&firstLine, &len, stdin);
    int i;
    for (i = 0; i < lineSize; ++i)
    {
        if (isdigit(firstLine[i]))
        {
            long num = parseNumber(i, firstLine, lineSize);
            for (; i < lineSize && isdigit(firstLine[i]); ++i) {} 
            numSeeds++;
            seeds = realloc(seeds, numSeeds * sizeof(long));
            seeds[numSeeds-1] = num;
        }
    }
    free(firstLine);

    Map* maps = calloc(0, sizeof(Map));
    int numMaps = 0;

    while (true)
    {
        char *line = NULL;
        lineSize = getline(&line, &len, stdin);
        if (lineSize == -1)
        {
            break;
        }
        if (!isdigit(line[0]))
        {
            free(line);
            line = malloc(0);
            // Read one more line to skip to the numbers.
            lineSize = getline(&line, &len, stdin);
            numMaps++;
            maps = realloc(maps, numMaps * sizeof(Map));
            maps[numMaps-1].numEntries = 0;
            maps[numMaps-1].entries = calloc(0, sizeof(MapEntry*));
        }
        else
        {
            int i = 0; 
            long firstNum = parseNumber(i, line, lineSize);
            for (; (i < lineSize) && isdigit(line[i]); ++i) {} 
            ++i;
            long secondNum = parseNumber(i, line, lineSize);
            for (; (i < lineSize) && isdigit(line[i]); ++i) {} 
            ++i;
            long thirdNum = parseNumber(i, line, lineSize);
            CREATE_MAP_ENTRY_PTR(entry);
            entry->origin = secondNum;   
            entry->destination = firstNum;   
            entry->width = thirdNum;   
            appendMapEntry(&maps[numMaps-1], entry);
        }
        free(line);
    }
    long lowestLocation = LONG_MAX;
    for (i = 0; i < numSeeds; ++i)
    {
        long seedNum = seeds[i];
        int j; 
        for (j = 0; j < numMaps; ++j)
        {
            seedNum = processValue(&maps[j], seedNum);
        }
        if (seedNum < lowestLocation)
        {
            lowestLocation = seedNum;
        }
    }     

    printf("Answer: %ld\n", lowestLocation);
    return 0;
}
