#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>

// To run build the code and then do the following command:
// ```
// cat ../day3/in.txt | day3/day3part2
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

typedef struct Location {
    int x;
    int y;
} Location;

typedef struct Part {
    int num;
    bool hasSymbol;
    Location loc;
} Part;

typedef struct Symbol {
    char type;
    Location loc;
} Symbol;

typedef struct Cell {
    Part* part;
    Symbol* symbol;
} Cell;

// Linked list of the Parts we have parsed.
typedef struct PartList {
    struct PartList* last; 
    Part* part;
} PartList;
#define CREATE_PART_LIST_PTR(X) PartList* X = calloc(1, sizeof(PartList))
PartList* appendPart(PartList* parts, Part* part)
{
    if (parts->part == NULL)
    {
        parts->part = part;
        return parts;
    }
    PartList* newParts = calloc(1, sizeof(PartList));
    newParts->last = parts;
    newParts->part = part;
    return newParts;
}

// Linked list of the Symbols we have parsed.
typedef struct SymbolList {
    struct SymbolList* last;
    Symbol* symbol;
} SymbolList;
#define CREATE_SYMBOL_LIST_PTR(X) SymbolList* X = calloc(1, sizeof(SymbolList))
SymbolList* appendSymbol(SymbolList* symbols, Symbol* symbol)
{
    if (symbols->symbol == NULL)
    {
        symbols->symbol = symbol;
        return symbols;
    }
    SymbolList* newSymbols = calloc(1, sizeof(SymbolList));
    newSymbols->last = symbols;
    newSymbols->symbol = symbol;
    return newSymbols;
}

// Matrix of the Cels we have parsed.
typedef struct CellMatrix {
    Cell** cells;
    int width;
    int height;
} CellMatrix;
#define CREATE_CELL_MATRIX(X) CellMatrix X = {0}
bool appendRow(CellMatrix* matrix, Cell* row, int rowWidth)
{
    if (matrix->width == 0)
    {
        matrix->width = rowWidth;
    }
    else
    {
        if (matrix->width != rowWidth)
        {
            false;
        }
    }
    matrix->height++;
    matrix->cells = realloc(matrix->cells, matrix->height * sizeof(Cell*));
    matrix->cells[matrix->height-1] = row;
    return true;
}

int main()
{
    // Initially parse all lines, storing number/length and symbol locations in a matrix.
    CREATE_PART_LIST_PTR(parts);
    CREATE_SYMBOL_LIST_PTR(symbols);
    CREATE_CELL_MATRIX(cellMatrix); 
        int x;
    int y = 0;
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
        Cell* cellRow = calloc(lineSize, sizeof(Cell));
        for (x = 0; x < lineSize; ++x)
        {
            if (isdigit(line[x]))
            {
                int num = parseNumber(x, line, lineSize);                
                Part* part = malloc(sizeof(Part));
                part->loc.x = x;
                part->loc.y = y;
                part->num = num;
                parts = appendPart(parts, part);
                for (; x < lineSize; ++x)
                {
                    if (!isdigit(line[x]))
                    {
                        x--;
                        break;
                    }
                    cellRow[x].part = part;
                }
            }
            else if (line[x] != '.' && line[x] != '\n')
            {
                Symbol* symbol = malloc(sizeof(Symbol));
                symbol->loc.x = x;
                symbol->loc.y = y;
                symbol->type = line[x];
                symbols = appendSymbol(symbols, symbol);
                cellRow[x].symbol = symbol;
            }
        }
        if (!appendRow(&cellMatrix, cellRow, lineSize))
        {
            printf("Error appending row.");
        }
        y++;
        free(line);
    }
    
    int gearRatioSum = 0; 
    
    
    SymbolList* currentSymbol = symbols;
    while(currentSymbol != NULL)
    {
        if (currentSymbol->symbol->type == '*')
        { 
            int numParts = 0;
            int gearRatioProd = 1;
            int lastPartNum = 0;
            int symbolX = currentSymbol->symbol->loc.x; 
            int symbolY = currentSymbol->symbol->loc.y; 
            for (y = symbolY - 1; y <= symbolY + 1; y++)
            {
                if ((y < 0) || (y > cellMatrix.height))
                {
                    continue;
                }
                for (x = symbolX - 1; x <= symbolX + 1; x++)
                {
                    if ((x < 0) || (x > cellMatrix.width))
                    {
                        continue;
                    }
 
                    if (cellMatrix.cells[y][x].part != NULL)
                    {
                        if (lastPartNum != cellMatrix.cells[y][x].part->num)
                        {
                            lastPartNum = cellMatrix.cells[y][x].part->num;
                            numParts++;
                            gearRatioProd *= cellMatrix.cells[y][x].part->num;
                        }
                    }
                }
            }
            if (numParts == 2)
            {
                gearRatioSum += gearRatioProd;
            }
        }
        // free(currentSymbol);
        currentSymbol = currentSymbol->last;
    }
    printf("Gear ratio sum: %d\n", gearRatioSum);
    // int numParts = 0;
    // int partSum = 0;
    // PartList* currentPart = parts;
    // while(currentPart != NULL)
    // {
    //     if (currentPart->part->hasSymbol)
    //     {
    //         numParts++;
    //         partSum += currentPart->part->num;
    //     }
    //     else
    //     {
    //         // printf("Missed part number %d\n", currentPart->part->num);
    //     }
    //     currentPart = currentPart->last;
    //     // free(currentPart);
    // }
    // for (y = 0; y < cellMatrix.height; ++y)
    // {
    //     // free(cellMatrix.cells[y]);
    // }
    //     
    // printf("Part sum: %d\n", partSum);
    return 0;
}
