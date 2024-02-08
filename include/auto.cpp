#include <iostream>
#include <cstdlib>
using namespace std;


bool passed,only;
int col,input;
int auto_row;
int temp;
int chosenrandom;

int randwithrange(int rangemax){
    srand((unsigned) time(NULL));
    return rand()%rangemax;
}


bool botfourinaline(int grid[6][7], int row_test, int column, int player, int numrows, int numcols){
    int direction[][2] = {{1, 0},{1, 1}, {0, 1}, {1, -1}};
    for (const auto& dir : direction){
        int count = 0;
        for (int step=-3;step<4;step++){
            int current_row = row_test + dir[0] * step;
            int current_col = column + dir[1] * step;
            if (0 > current_row || current_row >= numrows && 0 > current_col || current_col >= numcols){
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


bool isFreeColumns(int numcols , int numrows, int grid[6][7] , int donotpick[7]){
    for (int i=0;i<numcols;i++){
        if (grid[numrows-1][i]==0 && donotpick[i]==0){
            return false;
        }
    }
    return true;
}

int autoplayer(int grid[6][7], int numrows, const int numcols, int level){
  if (level==3){
    chosenrandom=-1;
  } else {
    chosenrandom=1;
  }
  int donotpick[numcols]={0};
  temp=-1;
  for (int col=0;col<numcols;col++){
      passed=false;
      for (int i=0;i<numrows;i++){
          if (grid[i][col]==0 && passed==false){
              auto_row=i;
              passed=true;
          }
      }
      if (passed==true){
          grid[auto_row][col] = 2;
          if (botfourinaline(grid, auto_row, col, 2, numrows, numcols)){
            if (randwithrange(level*5)==chosenrandom){
              cout<<"WOOPS"<<endl;
            } else{
              grid[auto_row][col] = 0;
              cout<<"WIN FOUND "<<col<<endl;
              return col;
            }
          }
          
          grid[auto_row][col] = 1;
          if (botfourinaline(grid, auto_row, col, 1, numrows, numcols)){
            if (randwithrange(level*5)==chosenrandom){
              cout<<"WOOPS"<<endl;
            } else{
              grid[auto_row][col] = 0;
              cout<<"GET BLOCKED"<<endl;
              temp=col;
            }
          }
          grid[auto_row][col] = 0;
      }
  }
  if (temp!=-1){
      return temp;
  }

  for (int col=0;col<numcols;col++){
    passed=false;
    for (int i=0;i<numrows-1;i++){
      if (grid[i][col]==0 && grid[i+1][col]==0 && passed==false){
        passed=true;
        auto_row=i+1;
      }    
    }
    if (passed==true){
      grid[auto_row][col] = 1;
      grid[auto_row-1][col] = 2;
      if (botfourinaline(grid, auto_row, col, 1, numrows, numcols)){
        donotpick[col] = 1;
        cout<<"I WILL TRY NOT TO PICK "<<col+1<<endl;
      }
      grid[auto_row-1][col] = 0;
      grid[auto_row][col] = 0;
    }
  }
  while (true){
    input=randwithrange(numcols+1);
    
    if (grid[numrows-1][input]==0 && donotpick[input]==0){
      cout<<"Bot input column: "<<input+1<<", do not pick is: "<<donotpick[input]<<endl;
      return input;
    }
      
    
    if (isFreeColumns(numcols , numrows , grid , donotpick) && grid[numrows-1][input]==0){
      cout<<"No good columns left, option chosen: " << input+1 << endl;
      return input;
    }
  }
}
