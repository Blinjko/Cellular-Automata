#include "../include/grid.h"
#include "../include/error.h"

#include <stdlib.h>
#include <string.h>

Error GridCreate(Grid* grid, int rows, int cols) {

    grid->array = malloc(rows * cols * sizeof(int));
    if (grid->array == NULL)
    {
        return GenerateError("Failed to allocate array with malloc(). GridCreate, grid.c\n");
    }

    memset(grid->array, 0, rows * cols * sizeof(int));

    grid->rows = rows;
    grid->cols = cols;

    return GenerateError(NULL);
}

void  GridDestroy(Grid* grid) {
    free(grid->array);

    grid->rows = 0;
    grid->cols = 0;
    grid->array = NULL;
}

int   GridGetIndex(Grid* grid, int row, int col) {
    // imagine the array is row after row.
    // if there are x columns then each row has x elements
    // the index of anything in the first row is the the column #
    // for the second row it would be the column # + (row # * x)
    // row # * x will offset the index from all the preceeding rows
    return grid->array[col + (row * grid->cols)];
}
void GridSetIndex(Grid* grid, int row, int col, int value) {
    grid->array[col + (row * grid->cols)] = value;
}

// Assumes grids are the same size
void GridCopy(Grid* sourceGrid, Grid* destGrid) {
    memcpy(destGrid->array, sourceGrid->array, sourceGrid->rows * sourceGrid->cols * sizeof(int));
}
