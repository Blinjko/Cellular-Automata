include("raylib.jl")

function UpdateCamera(camera, screendims, matrixdims)
    #=
    # Moving has to take into account zoom
    # We cant move left, or up past 0, this is simple
    # We also cant move right or down past, matrixcols, and matrixrows respectively
    # targetx * zoom + screenwidth > zoom * matrixcols
    =#

    # Move left condition
    targetx = IsKeyDown(key_left) && camera.target.x > 0.0 ? camera.target.x - 1.0 : camera.target.x

    # Move right condition
    targetx = IsKeyDown(key_right) && (targetx * camera.zoom + screendims[2]) < (camera.zoom * matrixdims[2]) ? targetx + 1.0 : targetx 


    # Move down up condition
    targety = IsKeyDown(key_up) && camera.target.y > 0 ? camera.target.y - 1.0 : camera.target.y

    # Move up condition
    targety = IsKeyDown(key_down) && (targety * camera.zoom + screendims[1]) < (matrixdims[2] * camera.zoom) ? targety + 1.0 : targety

    #= 
    # The size of the cells == zoom
    # By default the grid size == the resolution, this prevents zooming too far out
    # But if we goto the edge and zoom out it goes out of bounds.
    # So we have to check for this condition and set the x or y offset accordingly so that it doesn't occur
    =#

    # Increase zoom ( moves closer )
    zoom = IsKeyDown(key_i) && camera.zoom < 10.0 ? camera.zoom + 0.1 : camera.zoom

    # Decrease zoom ( moves farther )
    zoom = IsKeyDown(key_d) && zoom > 1.0 ? zoom - 0.1 : zoom

    # Verify targetx is still in bounds
    targetx = (targetx * zoom + screendims[2]) > (zoom * matrixdims[2]) ? matrixdims[2] - (screendims[2] / zoom) : targetx

    # Verify targety is still in bounds
    targety = (targety * zoom + screendims[1]) > (zoom * matrixdims[1]) ? matrixdims[1] - (screendims[1] / zoom) : targety

    camera = Camera2D(camera.offset,
                      Vector2(targetx, targety),
                      camera.rotation,
                      zoom)
end

function RenderMatrix(matrix)
    for row in 1:size(matrix)[1]
        for col in 1:size(matrix)[2]

            (matrix[row, col] == 1) && DrawPixel((col-1),  # X Pos
                                                 (row-1),  # Y Pos
                                                 black)    # Color
        end
    end

end

# Neighborhood Type
abstract type Neighborhood end
abstract type Moore <: Neighborhood end

function GetNeighborhood(matrix, coordinates, ::Type{Moore})
    # Coordinates are in (y, x) form, (row, col)

    # I hate spelling this shit out....
    nghbr = zeros(Int8, 3, 3)

    # for brevity
    row = coordinates[1]
    col = coordinates[2]

    # top left
    nghbr[1,1] = row > 1 && col > 1 ? matrix[row-1, col-1] : 0 

    # top middle
    nghbr[1,2] = row > 1 ? matrix[row-1, col] : 0 

    # top right
    nghbr[1,3] = row > 1 && col < size(matrix)[2] ? matrix[row-1, col+1] : 0 

    # middle left
    nghbr[2,1] = col > 1 ? matrix[row, col-1] : 0 

    # middle
    nghbr[2,2] = matrix[row, col]

    # middle right
    nghbr[2,3] = col < size(matrix)[2] ? matrix[row, col+1] : 0 
     
    # bottom left
    nghbr[3,1] = row < size(matrix)[1] && col > 1 ? matrix[row+1, col-1] : 0 

    # bottom middle
    nghbr[3,2] = row < size(matrix)[1] ? matrix[row+1, col] : 0 

    # bottom right
    nghbr[3,3] = row < size(matrix)[1] && col < size(matrix)[2] ? matrix[row+1, col+1] : 0 

    return nghbr
end


# Rule types, literally :)
abstract type Rule end

# Composite type because it allows this to be extened in the future, should any rule have any extra parameters
struct Life <: Rule end

function ApplyRule(matrix, rule::Life)
    newmatrix = zeros(Int8, size(matrix))

    for row in 1:size(matrix)[1]
        for col in 1:size(matrix)[2]

            nghbr = GetNeighborhood(matrix, (row, col), Moore)
            livecells = sum(nghbr)

            # Death clause
            if matrix[row, col] == 1 && (livecells < 3 || livecells > 4)
                newmatrix[row, col] = 0

            # Life clause
            elseif (matrix[row, col] == 1 && (livecells == 3 || livecells == 4)) || (matrix[row, col] == 0 && livecells == 3)
                    newmatrix[row, col] = 1

            else
                newmatrix[row, col] = 0
            end

        end
    end

    return newmatrix
end

let
    # Screen parameters
    screendims = (500, 500) # rows, cols | height X width ( backwards for consistency )

    # Cell grid / matrix
    matrixdims = (500, 500) # rows, cols - like julia does it

    cellmatrix = zeros(Int8, matrixdims)

    # Temp set matrix to altering values 
    #=
    for i in 1:matrixdims[1]
        for j in 1:matrixdims[2]

            if (iseven(i) && iseven(j)) || (!iseven(i) && !iseven(j))
                cellmatrix[i, j] = 1
            else
                cellmatrix[i, j] = 0
            end
        end
    end
    =#


    # Camera 2D
    camera = Camera2D(Vector2(0.0,0.0), # Offset
                      Vector2(0.0,0.0), # Target
                      0.0,              # Rotation
                      10.0)             # Zoom

    # Create the window
    InitWindow(screendims[2] ,screendims[1], "Julia Cellular Automata")

    SetTargetFPS(60)

    # Main loop
    # Closes when ESC is pressed or window (X) is clicked
    while !WindowShouldClose()

        camera = UpdateCamera(camera, screendims, matrixdims)
        BeginDrawing()

        ClearBackground(gray)

        BeginMode2D(camera)

        if IsKeyDown(key_space)

            if IsMouseButtonPressed(1)
                row::Int = div(GetMouseY(), camera.zoom, RoundDown) + round(Int, camera.target.y) + 1
                col::Int = div(GetMouseX(), camera.zoom, RoundDown) + round(Int, camera.target.x) + 1

                # Flip the value
                cellmatrix[row, col] = cellmatrix[row, col] == 0 ? 1 : 0

                println("Row: $(row) Col: $(col) set to $(cellmatrix[row, col])")
            end

            RenderMatrix(cellmatrix)
            # Clear
        elseif IsKeyDown(key_c)
            cellmatrix = zero(cellmatrix)
            RenderMatrix(cellmatrix)
        else
            RenderMatrix(cellmatrix)
            cellmatrix = ApplyRule(cellmatrix, Life())

        end
        EndMode2D()

        EndDrawing()

    end
end
