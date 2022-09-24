#include <iostream>
#include <fstream>
#include <windows.h>
//#include <unistd.h>  

const char * MATRIX_FILE = "matrix.txt";
const int SLEEPTIME = 1000;
const int ALIVE = 1;
const int DEAD = 0;

int showFieldStatus(int ** field, int rows, int cols, int generation) {
    int alive_cells = 0;
    for (int height_cnt = 0; height_cnt < rows; ++height_cnt) {
        for (int width_cnt = 0; width_cnt < cols; ++width_cnt)
        {
            if (field[height_cnt][width_cnt] == ALIVE) {
                std::cout << "*";
                alive_cells++;
            } else {
                std::cout << "-";
            }
            std::cout << " ";
        }
        std::cout << std::endl;
    }
    std::cout << "Generation: " << generation << " Alive cells: " << alive_cells << std::endl;
    return alive_cells;
}

void arrCopy(int** source, int ** dest, int rows, int cols) {
    for (int height_cnt = 0; height_cnt < rows; ++height_cnt) {
        for (int width_cnt = 0; width_cnt < cols; ++width_cnt)
        {
            dest[height_cnt][width_cnt] = source[height_cnt][width_cnt];
        }
    }
}

int main()
{
    int generation = 0;
    int matrix_width = 0;
    int matrix_height = 0;
    int ** matrix_field1;
    int ** matrix_field2;

    std::ifstream in_file(MATRIX_FILE);

    if (!in_file.is_open()) {
        std::cout << "Error can't open matrix file!";
        std::exit(-1);
    }

    in_file >> matrix_height;
    in_file >> matrix_width;
    if (matrix_width < 2 || matrix_height < 2){
        std::cout << "Error matrix wrong!";
        std::exit(-1);
    }

    matrix_field1 = new int* [matrix_height];
    matrix_field2 = new int* [matrix_height];
    for (int height_cnt = 0; height_cnt < matrix_height; ++height_cnt)
    {
        matrix_field1[height_cnt] = new int [matrix_width]();
        matrix_field2[height_cnt] = new int [matrix_width]();
    }

    int y, x;
    while (in_file >> y && in_file >> x) {
        if (y >= 0 && y < matrix_height && x >= 0 && x < matrix_width) {
            matrix_field1[y][x] = ALIVE;
        }
    }
    
    while (true)
    {
        generation++;
        int aliveCells = showFieldStatus(matrix_field1, matrix_height, matrix_width, generation);
        Sleep(SLEEPTIME);

        if (aliveCells == 0) {
            std::cout << "Game Over, no alive cells.";
            break;
        }

        boolean isStable = true;

        for (int y_cnt = 0; y_cnt < matrix_height; ++y_cnt)
        {
            for (int x_cnt = 0; x_cnt < matrix_width; ++x_cnt)
            {
                // ++
                int alive_friends = 0;
                for (int yinnr_cnt = y_cnt-1; yinnr_cnt <= y_cnt+1; ++yinnr_cnt)
                {
                    for (int xinnr_cnt = x_cnt-1; xinnr_cnt <= x_cnt+1; ++xinnr_cnt)
                    {
                        if (yinnr_cnt >= 0 && yinnr_cnt <= matrix_height-1 && xinnr_cnt >= 0 && xinnr_cnt <= matrix_width-1 && (yinnr_cnt != y_cnt || xinnr_cnt != x_cnt)) {
                            if (matrix_field1[yinnr_cnt][xinnr_cnt] == ALIVE) {
                                alive_friends++;
                            }
                        }
                    }
                }
                matrix_field2[y_cnt][x_cnt] = matrix_field1[y_cnt][x_cnt]; // fill new matrix with default values
                
                if (matrix_field1[y_cnt][x_cnt] == ALIVE && (alive_friends < 2 || alive_friends > 3)) {
                    matrix_field2[y_cnt][x_cnt] = DEAD;
                    isStable = false;
                }
                if (matrix_field1[y_cnt][x_cnt] == DEAD && alive_friends == 3) {
                    matrix_field2[y_cnt][x_cnt] = ALIVE;
                    isStable = false;
                }
                // inner algo
            }
        }
        arrCopy(matrix_field2, matrix_field1, matrix_height, matrix_width);
        //std::swap(matrix_field1, matrix_field2); // C++ better choice (tested: working)

        if (isStable) {
            std::cout << "Game Over, field stable.";
            break;
        }

        std::system("cls");
    }
    
    for (int height_cnt = 0; height_cnt < matrix_height; ++height_cnt) {
        delete [] matrix_field1[height_cnt];
        delete [] matrix_field2[height_cnt];
    }
    delete [] matrix_field1;
    delete [] matrix_field2;
    
    return 0;
}


