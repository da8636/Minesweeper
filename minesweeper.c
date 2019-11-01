#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

//creates a cell and its attributes
struct Cell{
  bool isBomb;
  bool isUncovered;
  bool isFlagged;
  int adjacent;
};

typedef struct Cell Cell;

int counter = 0; // to keep a track of what stage/instruction the game is on
int totalMoves = 0; //to help calculate if the game has been won with
int width, height;


void initiateError(char* initiate, char* trash){
	//stage 1 - set the grid.
	//error if any incorrect statement while trying to set grid.
	if (*trash != '\n'){ //if any trailing input after the command, including a space.
		printf("error\n");
		exit(1);
	}
	if (*initiate != 'g'){//if the first command is not to set grid
      printf("error\n");
	  exit(1);
    }
	else{
		if (height < 0 || height > 100 || width < 0 || width > 100 || ((height * width) < 10)){ //the requirements of the grid
        	printf("error\n");
    		exit(1);
      	}
	}
}
void bombError(char* initiate, int* xpos, int* ypos, Cell grid[width][height], char* trash){
	//stage 2 - set 10 bombs
	//error if any incorrect statement while trying to set the bombs.
	if (*trash != '\n'){ //if any trailing input after the command, including a space.
		printf("error\n");
		exit(1);
	}
	if ((*initiate != 'b' )&& (counter < 12)){ //set to < 12, to ensure no more than 10 bombs are placed.
	 	printf("error\n");
	 	exit(1);
  	}
	else{
		if ((*xpos < 0 || *ypos < 0) || (*xpos > width - 1 || *ypos > height - 1) || (grid[*xpos][*ypos].isBomb == true)){ //if not in the boundries of grid or has been perviously set.
			printf("error\n");
			exit(1);
		}
	}
}

void generalError(char* initiate, int* instructX, int* instructY, Cell grid[width][height], char* trash){
	//stage 3 - flag or uncover
	//error if any incorrect statement while trying to set the flag or uncover a cell.
	if (*trash != '\n'){ //if any trailing input after the command, including a space.
		printf("error\n");
		exit(1);
	}
	if ((*initiate != 'u') && (*initiate != 'f')) { //error when not being flagged or uncovered.
		printf("error\n");
		exit(1);
	}
	if ((*initiate == 'u') && (grid[*instructX][*instructY].isFlagged)){ // when uncovering a flagged
		printf("error\n");
		exit(1);
	}
	if ((*initiate == 'f') && (grid[*instructX][*instructY].isUncovered)){ //when flagging a uncovered
		printf("error\n");
		exit(1);
	}
}


void printGrid(Cell grid[width][height]){
	//prints the grid with the individual cell attributes and values
	printf("+");
	int countLines = 0;
	while(countLines < width){//allows for the grid to resize according to the grid size.
		printf("-");
		countLines++;
	}
	printf("+\n");
	//goes through the grid and prints flagged and uncovered according to corresponding boolean
   	for(int rows=0; rows < height; rows++) {
		printf("|");
	 	for(int columns=0; columns < width; columns++) {
	   		if (grid[columns][rows].isUncovered){
		   		printf("%d", grid[columns][rows].adjacent);
	   		}
	   		else if (grid[columns][rows].isFlagged){
		  		printf("f");
	   		}
	   		else{
		  		printf("*");
	   		}
	 	}
		printf("|\n");
   	}
   	printf("+");
   	countLines = 0;
   	while(countLines < width){
	 	printf("-");
	 	countLines++;
   	}
   	printf("+\n");
}

int main(void){
  char trash;
  char initiate;
  char str[101];// n = 100 +1 for null terminator
  fgets(str, 100, stdin);
  sscanf(str, "%c %d %d%c", &initiate, &width, &height, &trash); //recieves input
  initiateError(&initiate, &trash); // prints if any error in the first command
  printf("g %d %d\n", width, height); //prints out the command again

  //sets the values and attributes of all individual cells to default
  Cell grid[width][height];
  counter += 1;
  for (int i = 0; i < width; i++){
	  for (int j = 0; j < height; j++){
		  grid[i][j].isBomb = false;
		  grid[i][j].isUncovered = false;
		  grid[i][j].isFlagged = false;
		  grid[i][j].adjacent = 0;
	  }
  }

  int xpos, ypos;
  while (counter < 11){
	//a while loop until all 10 bombs have been set
	fgets(str, 100, stdin);
    sscanf(str, "%c %d %d%c", &initiate, &xpos, &ypos, &trash);//receive input
	bombError(&initiate, &xpos, &ypos, grid, &trash);//to return a error if incorrect command.
	counter += 1;
	grid[xpos][ypos].isBomb = true;//sets the bomb for that cell
	printf("b %d %d\n", xpos, ypos);

	//goes to the bomb and goes through to add up
	for (int i = xpos-1; i <= xpos+1; i++){
		for (int j = ypos-1; j <= ypos+ 1; j++){
			if (!(i == xpos && j == ypos) && (j >= 0 ) && (i >= 0 ) && (i < width) && (j < height)){
				grid[i][j].adjacent++;
			}
		}
	 }
  }

 printGrid(grid);//after setting the grid and bomb the game is printed
 int flags = 0;

 int instructX, instructY;
 while (((width*height)-totalMoves) > 0 ){
	//you need exactly the width*height no. of instructions for the cells, to win
	//follows the instruction to uncover or flag.
	fgets(str, 100, stdin);
    sscanf(str, "%c %d %d%c", &initiate, &instructX, &instructY, &trash);
	generalError(&initiate, &instructX, &instructY, grid, &trash);
 	if (initiate == 'u'){
	 	//uncovers the cell
	 	grid[instructX][instructY].isUncovered = true; // sets cell as uncovered
	 	totalMoves++;
		if (grid[instructX][instructY].isBomb){//however if cell a bomb, output lost
		 	printf("lost\n");
		 	return 0;
	 	}
		printf("u %d %d\n", instructX, instructY);//print the instruction command
 	}
 	if (initiate == 'f'){
	 	//flags the cell
		totalMoves++;
		if (flags == 10){
			printf("error\n");
			return 0;
		}
		flags++;
	 	grid[instructX][instructY].isFlagged = true; //sets cell as flagged
		printf("f %d %d\n", instructX, instructY);//print the instruction command
 	}
	printGrid(grid);//print grid/game map
 }
 printf("won\n");//you have won!
}
