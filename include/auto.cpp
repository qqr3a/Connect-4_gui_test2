#include <iostream>
#include <cstdlib>


bool only,passed;
int col,input;
int auto_row;
int temp;
int chosenrandom;
bool mal;



int randwithrange(int rangemax){
    srand((unsigned) time(NULL));
    return rand()%rangemax;
}

int FASTrandwithrange(int rangemax){
    return rand()%rangemax;
}



bool botfourinaline(int grid[6][7], int row_test, int column, int player, int numrows, int numcols){
    int direction[][2] = {{1, 0},{1, 1}, {0, 1}, {1, -1}};
    for (const auto& dir : direction){
        int count = 0;
        for (int step=-3;step<4;++step){
            int current_row = row_test + dir[0] * step;
            int current_col = column + dir[1] * step;
            if (0 > current_row || current_row >= numrows || 0 > current_col || current_col >= numcols){
                count=0;
                continue;
            }

            if (grid[current_row][current_col] != player){
              count=0;
              continue;
            }


            count += 1;
            if (count == 4){
                return true;
            }   
        }
    }
    return false;
}


void isMalOption(int grid[6][7] , int row , int col , int donotpick[7]){
    row++;

    grid[row][col] = 1;
    grid[row-1][col] = 2;
    if (botfourinaline(grid , row , col , 1 , 6 , 7)){
        donotpick[col]=1;
        std::cout << "Column " << col+1 << " flaged as bad option"<< std::endl;
    }
    grid[row][col] = 0;     
    grid[row-1][col] = 0;

}

int rowposition(int grid[6][7],int column,int numrows){
    for (int i=0;i<numrows;++i){
        if (grid[i][column]==0){
            return i;
        }
    }
    return -1;
}


bool isFreeColumns(int numcols , int numrows, int grid[6][7] , int donotpick[7]){
    for (int i=0;i<numcols;++i){
        if (grid[numrows-1][i]==0 && donotpick[i]==0){
            return true;
        }
    }
    return false;
}

int autoplayer(int grid[6][7], int numrows, const int numcols, int level){
    if (level==3){
        chosenrandom=-1;
    } else {
        chosenrandom=1;
    }
    int donotpick[numcols]={0};
    temp=-1;
    for (int col=0;col<numcols;++col){
        auto_row=rowposition(grid , col , numrows);

        if (auto_row==-1){
            continue;
        }

        grid[auto_row][col] = 2;
        if (botfourinaline(grid, auto_row, col, 2, numrows, numcols)){
            grid[auto_row][col] = 0;
            std::cout << "WIN FOUND at ( " << col+1 << " , " << auto_row+1 << " )" <<std::endl;
            return col;
        }
        
        
        if (temp!=-1){
            grid[auto_row][col] = 0;
            continue;
        }

        grid[auto_row][col] = 1;

        if (botfourinaline(grid, auto_row, col, 1, numrows, numcols)){
            std::cout << "CAN BE BLOCKED at ( " << col+1 << " , " << auto_row+1 << " )" << std::endl;
            temp=col;
        }

        grid[auto_row][col] = 0;
    
    } 
    if (temp!=-1){
        std::cout << "BLOCKED at " << temp+1 << std::endl;
        return temp;
    }

    for (int col=0;col<numcols;++col){
        
        auto_row=rowposition(grid , col , numrows);
        
             
        if (auto_row==-1 || auto_row==5){
            continue;
        }
        isMalOption(grid , auto_row , col , donotpick);
        
    } 
    

    if (isFreeColumns(numcols , numrows , grid , donotpick)){
        while (true){

            input=FASTrandwithrange(numcols+1);
            
            if (grid[numrows-1][input]==0 && donotpick[input]==0){
                std::cout<<"Bot input column: " << input+1 << ", do not pick is: " << donotpick[input] << std::endl;
                return input;
            }
        }
    }

    input=0;
    while (true){
        
        
        if (grid[numrows-1][input]==0){
            std::cout<<"No prefered columns left, option chosen: " << input+1 << std::endl;   
            return input;
        }
        input++;

    }
}
