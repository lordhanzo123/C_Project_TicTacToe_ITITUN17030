#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <time.h> 

/*

Noughts= O 
Crosses= X
Create a board with 25 element
int board[25] = {
	:,:,:,:,:,
	:,O,_,_,:,
	:,_,_,_,:,
	:,_,X,O,:,
	:,:,:,:,:,
}

	0, 1, 2, 3, 4,
	5, 6, 7, 8, 9,
	10,11,12,13,14,
	15,16,17,18,19,
	20,21,22,23,24
	
	6, 7, 8,
	11,12,13,
	16,17,18
		

*/
/***************Set Up**********************************/
enum { NOUGHTS, CROSSES, BORDER, EMPTY };
enum { HUMANWIN, COMPWIN, DRAW };

const int Directions[4] = { 1, 5, 4, 6 }; //start from 12 --> 13,17,16,18 if you want to go opposite - sign
const int InMiddle = 4; //repesent the middle
const int Corners[4] = { 0, 2, 6, 8 }; //represent the corner
int ply = 0; //how many move deep into the tree we are
int positions = 0; //
int maxPly = 0; //max ply to get how deep we actually go inside the tree
/***************Done.Set Up**********************************/

const int ConvertTo25[9] = { //array to convert 3x3 board
	6, 7, 8,
	11,12,13,
	16,17,18
};

int GetNumForDir(int startSq, const int dir, const int *board, const int us) { //startsq check,us is our move
	int found = 0;
	while(board[startSq] != BORDER) {	//starsq is not touch border	
		if(board[startSq] != us) {	 //starsq is not equal us
			break; //break
		}
		found++;	//otherwise increase found variable
		startSq += dir;  //starting square + direction
	}	
	return found; //return the found result
}

/***************Check Win Condition**********************************/
int FindThreeInARow(const int *board, const int ourindex, const int us) { //ourindex

	int DirIndex = 0; //use for loop in the direction
	int Dir = 0;     
	int threeCount = 1; //count 1-->3
	
	for(DirIndex = 0; DirIndex < 4; ++DirIndex) {
		Dir = Directions[DirIndex]; //direction array take direction index
		threeCount += GetNumForDir(ourindex + Dir, Dir, board, us);   //count in our direction
		threeCount += GetNumForDir(ourindex + Dir * -1, Dir * -1, board, us); //count in our opposite direction
		if(threeCount == 3) {
			break;
		}
		threeCount = 1;
	}
	return threeCount;
}
/***************Done.Check Win Condition**********************************/


/***************Draw the board**********************************/
void InitialiseBoard(int *board) {
	int index = 0;
	
	for(index = 0; index < 25; ++index) { //import element form the array 
		board[index] = BORDER;   
	}
	
	for(index = 0; index < 9; ++index) { //import the 3x3 board
		board[ConvertTo25[index]] = EMPTY;
	}	
}

void PrintBoard(const int *board) {
	int index = 0;
	char pceChars[] = "OX|_"; //make a char array
	printf("\n\nBoard:\n\n");
	for(index = 0; index < 9; ++index) { 
		if(index!=0 && index%3==0) { //make a newline when it have 3 element a row and index
			printf("\n\n");   
		}
		printf("%4c",pceChars[board[ConvertTo25[index]]]);
	}
	printf("\n");
}
/***************Done!.Draw the board**********************************/


int HasEmpty(const int *board) { //find a square it is empty or not
	int index = 0;
	
	for(index = 0; index < 9; ++index) {  //Loop through 9 squares
		if( board[ConvertTo25[index]] == EMPTY) return 1;   //if it is empty then return 1 
	}
	return 0; //otherwise return 0
}

void MakeMove(int *board, const int sq, const int side) { //sq is square
	board[sq] = side;
}

int GetHumanMove(const int *board) { //take human input
	
	char userInput[3]; 	//store user move the valid move is an number and enter
	
	int moveOk = 0;      	//check human move or not
	int move = -1;       	//make move
	
	while (moveOk == 0) { 	//while loop will move until moveOK is not 0
	
		printf("\nPlease enter a move from 1 to 9:");		
		fgets(userInput, 3, stdin); //fgets to read text form the keyboard,stdin standard input
		fflush(stdin); 
		
		if(strlen(userInput) != 2) { //strlen check user input is not equal 2 then its invalid
			printf("Invalid input.Please follow the instruction \n");
			continue;			
		}
		
		if( sscanf(userInput, "%d", &move) != 1) { //sscanf to format our string one int one addres to store if it's not equal to 1 move=-1
			move = -1;
			printf("Invalid input.Please follow the instruction\n");
			continue;
		}
		
		if( move < 1 || move > 9) { //check out of range
			move = -1;
			printf("Invalid range\n");
			continue;
		}
		
		move--; // Zero indexing
		
		if( board[ConvertTo25[move]]!=EMPTY) { //check if the board is empty in that location
			move=-1;
			printf("Square not available\n");
			continue;
		}
		moveOk = 1;
	}
	printf("Your choice is %d\n\n\n",(move+1)); //+1 because current move is 0
	return ConvertTo25[move];
}

int FindThreeInARowAllBoard(const int *board, const int us) { //Find three in a row on the board for the human player
	int threeFound = 0;
	int index;
	for(index = 0; index < 9; ++index) { //loop from 1 to 9
		if(board[ConvertTo25[index]] == us) { //convert to 3x3 system
			if(FindThreeInARow(board, ConvertTo25[index], us) == 3) { //if it found three in a row 				
				threeFound = 1;  
				break;
			}
		}
	}	
	return threeFound;
}

int EvalForWin(const int *board, const int us) { //Check win or loss for human

	if(FindThreeInARowAllBoard(board, us) != 0) //if human win return 1
		return 1; 
	if(FindThreeInARowAllBoard(board, us ^ 1) != 0) //if compunter win return -1
		return -1;
	
	return 0;
}

int MinMax(int *board, int side) {

	/* check is a win
	generate all moves for side
	loop moves, make move, minmax() on move to get score
	assess bestscore
	end moves return bestscore*/
	
	int MoveList[9]; //List of possible move
	int MoveCount = 0; //Count of move
	int bestScore = -2; //best score for computer
	int score = -2; //current score
	int bestMove = -1; //keep track of best move
	int Move; //current move have been made
	int index;
	
	if(ply > 0) { //computer reach the win position or loss
		score = EvalForWin(board, side); 
		if(score != 0) {					
			return score; //return position score for win or loss 
		}		
	}
	
	// fill Move List
	for(index = 0; index < 9; ++index) { //find all possible move on the board
		if( board[ConvertTo25[index]] == EMPTY) {
			MoveList[MoveCount++] = ConvertTo25[index]; //Count the total of possible move
		}
	}
	
	// loop all moves
	for(index = 0; index < MoveCount; ++index) {
		Move = MoveList[index];
		board[Move] = side;	
		
		ply++; //increase the ply
		score = -MinMax(board, side^1); //EvalforWin return a minus number because the board is the win for computers,recursive calling current side of the move is always try to maximize the score
		if(score > bestScore) {	//current for the first move		
			bestScore = score;	
			bestMove = Move;
		}
		board[Move] = EMPTY; //erase the board
		ply--; //decrease the ply and keep moving
	}
	
	if(MoveCount==0) { //movecount is zero because the board is actually full
		bestScore = FindThreeInARowAllBoard(board, side);	//always 0 beacause we already evaluated for a win 
	}
	
	if(ply!=0)
		return bestScore;	//if we are not at the top of the tree then return best score
	else 
		return bestMove;	//if we are  at the top of the tree then return best move
}

int GetComputerMove(int *board, const int side) {
	
	ply=0; 
	positions=0;
	maxPly=0;
	int best = MinMax(board, side);
	return best;
	
}

void RunGame() {
	int GameOver = 0;
	int Side = NOUGHTS;
	int LastMoveMade = 0;
	int board[25];
	InitialiseBoard(&board[0]); //call the address of the board call to the first element of the array
	PrintBoard(&board[0]); 
        
	while(!GameOver) {
		if(Side==NOUGHTS) {	
			LastMoveMade = GetHumanMove(&board[0]);
			MakeMove(&board[0],LastMoveMade,Side); //display move on the board
			Side=CROSSES; 
		} else {
			LastMoveMade = GetComputerMove(&board[0],Side);
			MakeMove(&board[0],LastMoveMade,Side); //display move on the board
			Side=NOUGHTS;
			PrintBoard(&board[0]);
		}
		
		// if three in a row exists Game is over
	
		if( FindThreeInARow(board, LastMoveMade, Side ^ 1) == 3) { //Side change to Opposite Side
			printf("\nGame over!\n");
			GameOver = 1;
			if(Side==NOUGHTS) {
				printf("Computer Wins");
			} else {
				printf("Human Wins");
			}
		}	
		
		// if no more moves, game is a draw	
		if(!HasEmpty(board)) {
			printf("Game over!\n");
			GameOver = 1;
			printf("It's a draw\n");
		}
	}
	
	PrintBoard(&board[0]);
}

int main() {
	char confirm[20];
	srand(time(NULL));
	system("COLOR B0");		
	printf("\n\n\t\t WELCOME TO TIC TAC TOE GAME \n"); 
	printf("\n\tGame Rules:\n\n\t\t1.X(The Insane AI) and O (You - the petty human).\n\t\t2. You will go first.\n\t\t");
	do{
	RunGame();
	printf("\nDo you want to continue(Y/N)\n"); 
	scanf("%s",&confirm);
}
	while(strcmp(confirm,"y")==0);
	
	return 0;
}

