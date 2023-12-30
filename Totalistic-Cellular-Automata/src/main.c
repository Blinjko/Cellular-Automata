#include "../include/grid.h"
#include "../include/rules.h"
#include "../include/error.h"
#include "../include/raylib.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

void GetVonNeumannNeighbors(Grid* grid, int* array, int cellY, int cellX) {
    // Assumes array is of length 5
    // Cellx & y are the coordinates of the central cell
    // Order in the array is negligeble because it all forms an average

    // I'll just implement this conditionally because it is straightforward
    
    // Walls / edges are assumed to have a value of 0

    // Central cell
    array[0] = GridGetIndex(grid, cellY, cellX);

    // upper cell
    array[1] = (cellY == 0) ? 0 : GridGetIndex(grid, cellY - 1, cellX);

    // lower cell
    array[2] = (cellY == (grid->rows - 1)) ? 0 : GridGetIndex(grid, cellY + 1, cellX);

    // left cell
    array[3] = (cellX == 0) ? 0 : GridGetIndex(grid, cellY, cellX - 1);

    // right cell
    array[4] = (cellX == (grid->cols - 1)) ? 0 : GridGetIndex(grid, cellY, cellX + 1);
}

void GetMooreNeighbors(Grid* grid, int* array, int cellY, int cellX) {
    // Assumes array is of length 9
    // Cellx & y are the coordinates of the central cell
    // Order in the array is negligeble because it all forms an average

    // I'll just implement this conditionally because it is straightforward
    
    // Walls / edges are assumed to have a value of 0

    // 0, 0 is in the top left of the grid

    // Central cell
    array[0] = GridGetIndex(grid, cellY, cellX);

    // upper cell
    array[1] = (cellY == 0) ? 0 : GridGetIndex(grid, cellY - 1, cellX);

    // lower cell
    array[2] = (cellY == (grid->rows - 1)) ? 0 : GridGetIndex(grid, cellY + 1, cellX);

    // left cell
    array[3] = (cellX == 0) ? 0 : GridGetIndex(grid, cellY, cellX - 1);

    // right cell
    array[4] = (cellX == (grid->cols - 1)) ? 0 : GridGetIndex(grid, cellY, cellX + 1);

    // upper left
    array[5] = ((cellY == 0) || (cellX == 0)) ? 0 : GridGetIndex(grid, cellY - 1, cellX - 1);

    // upper right
    array[6] = ((cellY == 0) || (cellX == (grid->cols - 1))) ? 0 : GridGetIndex(grid, cellY - 1, cellX + 1);

    // lower left
    array[7] = ((cellY == (grid->cols -1)) || (cellX == 0)) ? 0 : GridGetIndex(grid, cellY + 1, cellX - 1);

    // lower right
    array[8] = ((cellY == (grid->cols - 1)) || (cellX == (grid->cols - 1))) ? 0 : GridGetIndex(grid, cellY + 1, cellX + 1);
}



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

void UpdateTotalisticGrid(Grid* currentGrid, Grid* newGrid, TotalisticRule* tr) {
    // Iterate through every cell and update it according to the rule

    int neighborCells[NEIGHBORHOOD_MAX];

    for(int y=0; y<currentGrid->rows; y++)
    {
        for(int x=0; x<currentGrid->cols; x++)
        {
            // Determine which neighborhood to use
            switch (tr->neighborhood)
            {
                case NEIGHBORHOOD_VON_NEUMANN: GetVonNeumannNeighbors(currentGrid, neighborCells, y, x); break;
                case NEIGHBORHOOD_MOORE: GetMooreNeighbors(currentGrid, neighborCells, y, x); break;
            }

            // Apply the rule, and set the current cell to its new value
            GridSetIndex(newGrid, y, x, ApplyTotalisticRule(tr, neighborCells));
        }
    }
}


void PrintUsage() {
    printf("Usage:\n\n");
    printf("Automata CODE K GRID_ROWS GRID_COLS SCREEN_WIDTH SCREEN_HEIGHT ITERATION_TIME TOTAL_ITERATIONS NEIGHBORHOOD\n\n");
    printf("Deinitions:\n");
    printf("\tCODE = determines the \"rule\" for the cellular automata, but is dependent on K, must be between 0 and k^(3k - 2)\n");
    printf("\tK = the amount of states a cell can have, k = 2 is binary. must be greater than 1\n");
    printf("\tGRID_ROWS, GRID_COLS = number of rows and columns in the automation grid / plane. Positive integer. Must be >= screen window size\n");
    printf("\tSCREEN_WIDTH, SCREEN_HEIGHT = size of the graphical window in pixels\n");
    printf("\tITERATION_TIME = time between each iteration is ms\n");
    printf("\tTOTAL_ITERATIONS = number of iterations to evolve\n");
    printf("\tNEIGHBORHOOD = neighborhood to use, v for von neumann, m for moore\n\n");

    printf("When simulation starts use the arrow keys to move around, and I, D to increase the resolution and decrease the resolution respectively.\n");
}

// Mapping of numbers to the colors they represent
const Color colorMapping[5] = {WHITE, BLACK, LIGHTGRAY, GRAY, DARKGRAY};

int main(int argc, char** argv) {
    
    if (argc < 10)
    {
        PrintUsage();
        return -1;
    }

    // Screen Size
    const int screenWidth = atoi(argv[5]);
    const int screenHeight = atoi(argv[6]);

    // Size of the cells in pixels
    int cellSize = 10;

    // Grid Size
    const int gridRows = atoi(argv[3]);
    const int gridCols = atoi(argv[4]);

    // Veiry grid is the same size as the screen or bigger
    if ((screenWidth > gridCols) || (screenHeight > gridRows))
    {
        printf("ERROR: Screen must not be bigger than the grid\n\n");
        PrintUsage();
        return -1;
    }

    // Rule determines the evolution of the cells
    TotalisticRule tr;

    int neighborhood = argv[9][0] == 'v' ? NEIGHBORHOOD_VON_NEUMANN : 0;
    neighborhood = argv[9][0] == 'm' ? NEIGHBORHOOD_MOORE : neighborhood;
    if (neighborhood == 0)
    {
        printf("Invalid neighborhood\n");
        PrintUsage();
        return -1;
    }

    const int code = atoi(argv[1]);
    const int k = atoi(argv[2]);
    
    if (k < 2)
    {
        printf("ERROR: K must be greater than 2\n\n");
        PrintUsage();
        return -1;
    }

    // Delay between iterations
    struct timespec delayTime;
    delayTime.tv_sec = (long) atoi(argv[7]) / 1000;
    delayTime.tv_nsec = ((long)atoi(argv[7]) - delayTime.tv_sec * 1000) * 1000000L;
    
    const int maxIteration = atoi(argv[8]);
    int iteration = 0;


    // Create the grids
    Grid currentGrid, futureGrid;
    
    Error error = GridCreate(&currentGrid, gridRows, gridCols); 
    if (error.errorMessage != NULL)
    {
        LogError(error, stderr);
        return -1;
    }

    error = GridCreate(&futureGrid, gridRows, gridCols); 
    if (error.errorMessage != NULL)
    {
        LogError(error, stderr);
        GridDestroy(&currentGrid);
        return -1;
    }

    // Generate the totalistic rule
    error = CalculateTotalisticRule(&tr, k, code, neighborhood);
    if (error.errorMessage != NULL)
    {
        LogError(error, stderr);

        GridDestroy(&currentGrid);
        GridDestroy(&futureGrid);
        return -1;
    }

    // Offsets for the view of the grid 
    int offsetX = 0;
    int offsetY = 0;
    int shiftSpeed = 1; // Controls the camera shift speed
    int paused = 0; // 0 = false 1 = true;

    // Create the window
    InitWindow(screenWidth, screenHeight, "Totalistic Cellular Automata Test");

    SetTargetFPS(60);

    // Main game loop
    while (!WindowShouldClose() && iteration < maxIteration)    // Detect window close button or ESC key
    {
        // Key detection for changing x offset
        if (IsKeyPressed(KEY_LEFT) && (offsetX > 0))
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
        else if (IsKeyPressed(KEY_RIGHT) && (offsetX < (gridCols - (screenWidth / cellSize))))
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
        if (IsKeyPressed(KEY_UP) && (offsetY > 0))
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
        else if (IsKeyPressed(KEY_DOWN) && (offsetY < (gridRows -  (screenHeight / cellSize))))
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
        if (IsKeyPressed(KEY_I) && (cellSize < 20))
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
        else if (IsKeyPressed(KEY_D) && (cellSize > 1))
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

        // Control for Pausing
        if (IsKeyPressed(KEY_P))
        {
            paused = (paused == 0) ? 1 : 0;
        }

        // Detect if mouse has been clicked & place cells accordingly
        // Cells can only be placed when paused
        if (paused)
        {
            // These are just in case the mouse moves
            int x = GetMouseX();
            int y = GetMouseY(); 

            // Get the current value
            int value = GridGetIndex(&currentGrid, 
                                     ((y / cellSize) + offsetY), 
                                     ((x / cellSize) + offsetX));

            // Potential issue with both being clicked at the same time? look into

            // if mouse 0 (left) is pressed, increment a cells value.
            // Value is only added if it is within the valid cell range
            value = (IsMouseButtonPressed(0) && (value < (k-1))) ? value + 1 : value;

            // if mouse 1 (right) is pressed, decrement a cells value
            // value cannot be less than 0, so it will only be decremented if it is greater than 0
            value = (IsMouseButtonPressed(1) && (value > 0)) ? value - 1 : value;

            // Update the grid
            GridSetIndex(&currentGrid, 
                    ((y / cellSize) + offsetY), 
                    ((x / cellSize) + offsetX), 
                    value);
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
                              colorMapping[GridGetIndex(&currentGrid, (y + offsetY), (x + offsetX))]);
            }
        }

        DrawText( paused ? "Simulation Paused" : "Simulation not paused", screenWidth - 200, 100, 12, RED);

        EndDrawing();

        if (!paused)
        {
            // Compute next generation
            UpdateTotalisticGrid(&currentGrid, &futureGrid, &tr);

            // Copy next generation to current generation
            GridCopy(&futureGrid, &currentGrid);

            // Delay
            nanosleep(&delayTime, NULL);

            iteration++;
        }
    }

    CloseWindow();        // Close window and OpenGL context

    // Destroy the grids
    GridDestroy(&currentGrid);
    GridDestroy(&futureGrid);
    DestroyTotalisticRule(&tr);

    return 0;
}
