#include <iostream>

using namespace std;





void printGrid(int grid[6][7],int numrows, int numcols , int remaining) { 
    cout << remaining << endl;
    for (int i = numrows - 1; i >= 0; --i) {
        for (int j = 0; j < numcols; ++j) {
            cout << grid[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
}

bool fourinaline(int grid[6][7], int row, int column, int player, int numrows, int numcols){
    int direction[][2] = {{1, 0} , {1, 1} , {0, 1} , {1, -1}};
    for (const auto& dir : direction){
        int count = 0;
        for (int step=-3;step<4;step++){
            int current_row = row + dir[0] * step;
            int current_col = column + dir[1] * step;
            if (0 <= current_row && current_row < numrows && 0 <= current_col && current_col < numcols){
                if (grid[current_row][current_col] == player){
                count += 1;
                if (count == 4){
                    std::cout << "Direction: (" << dir[0] << ", " << dir[1] << ")" << std::endl;
    
                    std::cout << std::endl;
                    return true;
                    }
                } else {
                count=0;}
            } else{
                count=0;}
        }
    }
    return false;
}

int addtocolumn(int grid[6][7],int column,int numrows,int player){
    for (int i=0;i<numrows;i++){
        if (grid[i][column]==0){
            grid[i][column]=player;
            return i;
        }
    }
    return 0;
}


bool validcolumn(int numrows,int numcols,int grid[6][7],int usercol){
    if (1<=usercol && usercol<=numcols){
        if (grid[numrows-1][usercol-1]==0){
            return true;
        }else{  
            cout<<"Column is full\n";}
    }else{
    cout<<"Column number out of range\n";}
    return false;
}
    
