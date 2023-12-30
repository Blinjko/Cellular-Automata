#ifndef GRID_H
#define GRID_H

#include "../include/error.h"

typedef struct Grid
{
    int rows;
    int cols;
    int* array;
} Grid;

Error GridCreate(Grid* grid, int rows, int cols);
void  GridDestroy(Grid* grid);
int   GridGetIndex(Grid* grid, int row, int col);
void  GridSetIndex(Grid* grid, int row, int col, int value);
void GridCopy(Grid* sourceGrid, Grid* destGrid);

#endif
