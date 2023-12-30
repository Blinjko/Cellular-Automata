#include "../include/grid.h"
#include "../include/rules.h"
#include "../include/error.h"
#include "../include/raylib.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>


void UpdateGrid(Grid* currentGrid, Grid* newGrid, Rule rule) {
    // Iterate through every cell and update it according to the rule

    for(int y=0; y<currentGrid->rows; y++)
    {
        for(int x=0; x<currentGrid->cols; x++)
        {
            int left, middle, right;

            // Cell to the left of the current cell, if were at the left
            // edge make it 0
            left   = (x == 0) ? 0 : GridGetIndex(currentGrid, y, x-1);

            // Current cell
            middle = GridGetIndex(currentGrid, y, x);

            // Cell to the right of the current cell, if at the rightmost edge of
            // the grid, make it 0
            right  = (x == (currentGrid->cols - 1)) ? 0 : GridGetIndex(currentGrid, y, x+1);

            // Apply the rule, and set the current cell to its new value
            GridSetIndex(newGrid, y, x, ApplyRule(rule, left, middle, right));
        }
    }
}

void PrintUsage() {
    printf("Usage:\n\n");
    printf("Automata RULE GRID_ROWS GRID_COLS SCREEN_WIDTH SCREEN_HEIGHT ITERATION_TIME\n\n");
    printf("Deinitions:\n");
    printf("\tRULE = rule for the elementry cellular automata, is intger between 0-256\n");
    printf("\tGRID_ROWS, GRID_COLS = number of rows and columns in the automation grid / plane. Positive integer. Must be >= screen window size\n");
    printf("\tSCREEN_WIDTH, SCREEN_HEIGHT = size of the graphical window in pixels\n");
    printf("\tITERATION_TIME = time between each iteration is ms\n\n");

    printf("When simulation starts use the arrow keys to move around, and I, D to increase the resolution and decrease the resolution respectively.\n");
}

int main(int argc, char** argv) {
    
    if (argc < 7)
    {
        PrintUsage();
        return -1;
    }

    // Screen Size
    const int screenWidth = atoi(argv[4]);
    const int screenHeight = atoi(argv[5]);

    // Size of the cells in pixels
    int cellSize = 10;

    // Grid Size
    const int gridRows = atoi(argv[2]);
    const int gridCols = atoi(argv[3]);

    // Veiry grid is the same size as the screen or bigger
    if ((screenWidth > gridCols) || (screenHeight > gridRows))
    {
        printf("ERROR: Screen must not be bigger than the grid\n\n");
        PrintUsage();
        return -1;
    }

    // Rule determines the evolution of the cells
    const Rule rule = atoi(argv[1]);

    // Delay between iterations
    struct timespec delayTime;
    delayTime.tv_sec = (long) atoi(argv[6]) / 1000;
    delayTime.tv_nsec = ((long)atoi(argv[6]) - delayTime.tv_sec * 1000) * 1000000L;
    
    // Total Iterations = gridRows to prevent overflow.
    // Also is pointless to make it more because it would just
    // stop evolution
    const int maxIteration = gridRows;
    int iteration = 0;


    // Create the grids
    Grid currentGrid, futureGrid, displayGrid;
    Error error = GridCreate(&currentGrid, 1, gridCols); // Note 1-dimension ( no y-axis ) 

    if (error.errorMessage != NULL)
    {
        LogError(error, stderr);
        return -1;
    }

    error = GridCreate(&futureGrid, 1, gridCols); // Note 1-dimension ( no y-axis ) 
    if (error.errorMessage != NULL)
    {
        LogError(error, stderr);
        GridDestroy(&currentGrid);
        return -1;
    }

    error = GridCreate(&displayGrid, gridRows, gridCols); // 2 dimensions, but 2nd dimension holds data from previous iterations
    if (error.errorMessage != NULL)
    {
        LogError(error, stderr);
        GridDestroy(&currentGrid);
        GridDestroy(&futureGrid);
        return -1;
    }

    // Create the starter cell in the center of the row
    GridSetIndex(&currentGrid, 0, gridCols/2, 1);

    // Offsets for the view of the grid 
    int offsetX = gridCols / 2;
    int offsetY = 0;
    int shiftSpeed = 1; // Controls the camera shift speed

    InitWindow(screenWidth, screenHeight, "Elemetry Cellular Automata Test");

    SetTargetFPS(60);

    // Main game loop
    while (!WindowShouldClose() && iteration < maxIteration)    // Detect window close button or ESC key
    {
        // Only the display grid gets rendered.

        // Copy the next iteration into the display Grid
        for (int x=0; x<gridCols; x++)
        {
            GridSetIndex(&displayGrid, iteration, x, GridGetIndex(&currentGrid, 0, x));
        }

        // Key detection for changing x offset
        if (IsKeyDown(KEY_LEFT) && (offsetX > 0))
        {
            if ((offsetX - shiftSpeed) < 0)
            {
                offsetX = 0;
            }
            else
            {
                offsetX-=shiftSpeed;
            }
        }
        else if (IsKeyDown(KEY_RIGHT) && (offsetX < (gridCols - (screenWidth / cellSize))))
        {
            if ((offsetX + shiftSpeed) > (gridCols - (screenWidth / cellSize)))
            {
                offsetX = gridCols - (screenWidth / cellSize);
            }
            else
            {
                offsetX+=shiftSpeed;
            }
        }

        // Key detection for changing y offset
        if (IsKeyDown(KEY_UP) && (offsetY > 0))
        {
            if ((offsetY - shiftSpeed) < 0)
            {
                offsetY = 0;
            }
            else
            {
                offsetY-=shiftSpeed;
            }
        }
        else if (IsKeyDown(KEY_DOWN) && (offsetY < (gridRows -  (screenHeight / cellSize))))
        {
            if ((offsetY + shiftSpeed) > (gridRows - (screenHeight / cellSize)))
            {
                offsetY = gridCols - (screenHeight / cellSize);
            }
            else
            {
                offsetY+=shiftSpeed;
            }
        }

        // Key detection for changing the size of the cells and picture resoultion
        if (IsKeyDown(KEY_I) && (cellSize < 20))
        {
            cellSize++;
            shiftSpeed--;

            // Make sure were still in bounds
            if (offsetX > (gridCols - (screenWidth / cellSize)))
            {
                offsetX = gridCols - (screenWidth / cellSize);
            }

            if (offsetY > (gridRows - (screenHeight / cellSize)))
            {
                offsetY = gridRows - (screenHeight / cellSize);
            }
        }
        else if (IsKeyDown(KEY_D) && (cellSize > 1))
        {
            cellSize--;
            shiftSpeed++;

            // Make sure were still in bounds
            if (offsetX > (gridCols - (screenWidth / cellSize)))
            {
                offsetX = gridCols - (screenWidth / cellSize);
            }

            if (offsetY > (gridRows - (screenHeight / cellSize)))
            {
                offsetY = gridRows - (screenHeight / cellSize);
            }
        }


        // Rendering starts
        BeginDrawing();

        ClearBackground(RAYWHITE);

        // Render the display grid
        for (int y=0; y<(screenHeight / cellSize); y++)
        {
            for (int x=0; x<(screenWidth / cellSize); x++)
            {
                DrawRectangle(x * cellSize, 
                              y * cellSize, 
                              cellSize, 
                              cellSize, 
                              GridGetIndex(&displayGrid, (y + offsetY), (x + offsetX)) ? BLACK : WHITE);
            }
        }

        EndDrawing();

        // Compute next generation
        UpdateGrid(&currentGrid, &futureGrid, rule);

        // Copy next generation to current generation
        GridCopy(&futureGrid, &currentGrid);

        // Delay
        nanosleep(&delayTime, NULL);

        iteration++;
    }

    CloseWindow();        // Close window and OpenGL context

    // Destroy the grids
    GridDestroy(&currentGrid);
    GridDestroy(&futureGrid);
    GridDestroy(&futureGrid);

    return 0;
}
