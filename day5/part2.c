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

typedef struct Range {
    long value;
    long width;
    bool handled;
} Range;
typedef struct RangeList {
    Range* ranges;
    int numRanges;
} RangeList;

RangeList processRangeValue(Map* map, Range range) 
{
    // printf("Input range value: %ld width: %ld\n", range.value, range.width);
    RangeList list = {0};
    list.numRanges = 1;
    list.ranges = realloc(list.ranges, sizeof(Range));
    list.ranges[0].value = range.value;
    list.ranges[0].width = range.width;
    list.ranges[0].handled = false;
    int i;
    for (i = 0; i < map->numEntries; ++i)
    {
        MapEntry* entry = map->entries[i];
        int j;
        for (j = 0; j < list.numRanges; ++j)
        {
            if (list.ranges[j].handled)
            {
                continue;
            }
            long leftRange = list.ranges[j].value;
            long rightRange = list.ranges[j].value + list.ranges[j].width - 1;
            long leftEntry = entry->origin;
            long rightEntry = entry->origin + entry->width - 1;
            // The range is fully contained in the map entry.
            if (leftRange >= leftEntry && rightRange <= rightEntry)
            {
                // printf("Fully contained.\n");
                // Shift the range.
                list.ranges[j].value = entry->destination + (leftRange - leftEntry);
                list.ranges[j].handled = true;
            }
            // The map entry is fully contained in the range.
            else if (leftEntry >= leftRange && rightEntry <= rightRange)
            {
                // printf("The map entry is fully contained in the range.\n");
                // Split the range three ways:
                // left
                list.numRanges++;
                list.ranges = realloc(list.ranges, list.numRanges * sizeof(Range));
                list.ranges[list.numRanges-1].value = leftRange;
                list.ranges[list.numRanges-1].width = leftEntry-leftRange;
                // right
                list.numRanges++;
                list.ranges = realloc(list.ranges, list.numRanges * sizeof(Range));
                list.ranges[list.numRanges-1].value = rightEntry + 1;
                list.ranges[list.numRanges-1].width = rightRange - rightEntry;
                // middle
                list.ranges[j].width = entry->width;
                list.ranges[j].value = entry->destination;
                list.ranges[j].handled = true;
            }
            // The range is to the left of the map entry.
            else if (rightRange < leftEntry)
            {
                // printf("The range is to the left of the map entry.\n");
                continue;
            }
            // The range is to the right of the map entry.
            else if (leftRange > rightEntry)
            {
                // printf("The range is to the right of the map entry.\n");
                continue;
            }
            // The range is to the right of the map entry.
            else if (leftRange >= leftEntry && leftRange <= rightEntry)
            {
                // printf("The range is to the right of the map entry and contained.\n");
                // Split the range two ways:
                // right
                list.numRanges++;
                list.ranges = realloc(list.ranges, list.numRanges * sizeof(Range));
                list.ranges[list.numRanges-1].value = rightEntry+1;
                list.ranges[list.numRanges-1].width = rightRange-rightEntry;
                // left
                list.ranges[j].width = rightEntry - leftRange + 1;
                list.ranges[j].value = entry->destination + (leftRange - leftEntry);
                list.ranges[j].handled = true;
            }
            // The range is to the left of the map entry.
            else
            {
                // printf("The range is to the left of the map entry and contained.\n");
                // Split the range two ways:
                // left
                list.numRanges++;
                list.ranges = realloc(list.ranges, list.numRanges * sizeof(Range));
                list.ranges[list.numRanges-1].value = leftRange;
                list.ranges[list.numRanges-1].width = leftEntry - leftRange;
                // right
                list.ranges[j].width = rightRange - leftEntry + 1;
                list.ranges[j].value = entry->destination;
                list.ranges[j].handled = true;
            }
        }
    }
    int k;
    // printf("Output ranges:\n");
    // for (k = 0; k < list.numRanges; ++k)
    // {
    //     printf("value: %ld width: %ld\n", list.ranges[k].value, list.ranges[k].width);
    // }
    return list;
}

long recurseMaps(Map* maps, int numMaps, int mapIndex, RangeList list, long lowestLocation)
{
    int i;
    long lowest = lowestLocation;
    // printf("Processing index %d\n", mapIndex);
    for (i = 0; i < list.numRanges; ++i)
    {
        RangeList newList = processRangeValue(&maps[mapIndex], list.ranges[i]);
        if (mapIndex == (numMaps - 1))
        {
            int j;
            for (j = 0; j < newList.numRanges; ++j)
            {
                if (newList.ranges[j].value < lowest)
                {
                    lowest = newList.ranges[j].value;
                }
            }
        }
        else
        {
            long newLowest = recurseMaps(maps, numMaps, mapIndex + 1, newList, lowest);
            if (newLowest < lowest)
            {
                lowest = newLowest;
            }
        }
    }
    return lowest;
} 


int main()
{
    // Parse seeds.
    RangeList list = {0};
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
            ++i;
            long width = parseNumber(i, firstLine, lineSize);
            for (; i < lineSize && isdigit(firstLine[i]); ++i) {} 
            list.numRanges++;
            list.ranges = realloc(list.ranges, list.numRanges * sizeof(Range));
            list.ranges[list.numRanges-1].value = num;
            list.ranges[list.numRanges-1].width = width;
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

    lowestLocation = recurseMaps(maps, numMaps, 0, list, lowestLocation);

    printf("Answer: %ld\n", lowestLocation);
    return 0;
}
