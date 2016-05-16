#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>
#define ROW 6 //define board
#define COL 7
#define SIZE 3

void printBoard(char *board);
void printWiningBoard(char *board);
void takeTurn(int round, char *board, char *player);
void savingWinnnigPos(int a,int b,int c,int d);
bool checkWin(int insertPos, char *board);
bool checkFour(char *board, int a, int b, int c, int d);
bool checkVertical(int insertPos, char *board);
bool checkHorizontal(int insertPos, char *board);
bool checkTilted(int insertPos, char *board);

int insertPos = 0; //get final insert position in takeTurn function
int winningPos[4];//the four winning position

int main(void){
	char board[ROW * COL];
	char player[SIZE] = "OX"; //player's sign
	int round = 2; //first round number
	int playagain;
	
	do{
		memset(board,' ', ROW * COL); //innitialize array with blank
	
		printBoard(board); //print the first board
	
		takeTurn(round, board, player); 
		//printf("insertPOs %d = %d\n", round % 2 + 1 , insertPos);
		while(checkWin(insertPos, board) != 1 && round != ROW * COL + 1){ //  win : 1
			++round; //next round
		
			takeTurn(round, board, player); //take turn
			//printf("insertPOs %d = %d\n", round % 2 + 1 , insertPos);
		}
		if(checkWin(insertPos, board) == 1){
			printWiningBoard(board);
			printf("Player %d (%c) Wins!\n\n\n", round % 2 + 1, player[round % 2]); //win
		}
		else if(checkWin(insertPos, board) != 1 && round == ROW * COL + 1){ //tie
			printf("Tie !\n");
		}
	
		printf("Enter (1) to play again\n");
		printf("Enter (0) to end the game\n");
		scanf("%d",&playagain);
	}while(playagain==1);
	
	printf("Thanks for playing.\n\n\n");
	
	system("pause");
	return 0;
}

void printBoard(char *board){
	
	size_t i;
	
	puts("--------Connect Four---------\n");
	
	for(i = 0; i < ROW * COL; ++i){
		if(i % COL == 0 && i != 0){  //change line every seven elements in one row
			printf("|\n");
			puts("-----------------------------");
		}
		
		printf("| %c ", board[i]); //print |board[0]|board[1]|board[2]|...
	}
	printf("|\n"); //print the last | 
	puts("-----------------------------");
	puts("  1   2   3   4   5   6   7\n\n\n");
}

void printWiningBoard(char *board){
	size_t i;
	
	puts("--------Connect Four---------\n");
	
	for(i = 0; i < ROW * COL; ++i){
		if(i % COL == 0 && i != 0){  //change line every seven elements in one row
			printf("|\n");
			puts("-----------------------------");
		}
		
		if(i==winningPos[0]||i==winningPos[1]||i==winningPos[2]||i==winningPos[3]){
			printf("|", board[i]);
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY | FOREGROUND_RED);//print red
			printf(" %c ", board[i]); //print board with winning position
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);//print white
		}
		
		else{
			printf("| %c ", board[i]); //print board without winning position
		}
		
	}
	printf("|\n"); //print the last | 
	puts("-----------------------------");
	puts("  1   2   3   4   5   6   7\n\n\n");
}

void takeTurn(int round, char *board, char *player){
	int position, bottom, test = 0;
	
	printf("player %d (%c) : Enter 1 ~ 7\n", round % 2 + 1, player[round % 2]); //player one go first
	scanf("%d", &position);
	
	while(position > 7 || position < 1){ //anti-fool
		printf("Enter 1 ~ 7\n");
		scanf("%d", &position);
	}
	while(test == 0){  //check whether insertion is complete, if complete, test = 1
		for(bottom = position + 34; bottom >= position - 1; bottom -= 7){ //check whether the position in board is blank from bottom
			if(board[bottom] == ' '){
				board[bottom] = player[round % 2]; //insert player sign to board
				test = 1; //insert complete
				
				printBoard(board); //print new board
				insertPos = bottom; //get final insert position store in global variable
				break; //end for loop
			}
		else if(bottom == position - 1 && board[bottom] != ' '){ //column full, insertion fail, test = 0
			printf("Column full! Enter other column.\n");
			scanf("%d", &position);
			}
		}
	}
}

void savingWinnnigPos(int a,int b,int c,int d){
	winningPos[0]=a;
	winningPos[1]=b;
	winningPos[2]=c;
	winningPos[3]=d;
}

bool checkWin(int insertPos, char *board){
	if(checkVertical(insertPos, board) == 1 || checkHorizontal(insertPos, board) == 1 || checkTilted(insertPos, board) == 1){
		return 1;
	}
	else{
		return 0;
	}
}

bool checkFour(char *board, int a, int b, int c, int d){
	if(board[a] == board[b] && board[b] == board[c] && board[c] == board[d] 
		&& d < COL * ROW && d >= 0 && a >= 0 && a < COL * ROW && board[a] != ' '){ // a:start, d:end must between 0 ~ 41
		return 1;
	}
	else{
		return 0;
	}
}

bool checkVertical(int insertPos, char *board){
	int start, add, i = 1, j = 1;
	
	for(start = 0; i <= 3; start += 7, ++i){
		for(add = start; j <= 7; ++add, ++j){
			if(checkFour(board, add, add + 7, add + 7*2, add + 7*3)){
				savingWinnnigPos(add, add + 7, add + 7*2, add + 7*3);
				
				return 1;
			}
		}
		j = 1;
	}
	return 0;
}

bool checkHorizontal(int insertPos, char *board){
	int start, add, i = 1, j = 1;
	
	for(start = 0; i <= 6; start += 7, ++i){
		for(add = start; j <= 4; add++, j++){
			if(checkFour(board, add, add + 1, add + 2, add + 3) == 1){
			
			//check form the last left side to last right side
			savingWinnnigPos(add, add + 1, add + 2, add + 3);
			return 1;
			}
		}
			j = 1;
	}
	return 0;
}
bool checkTilted(int insertPos, char *board){
	int start, i = 1, j = 1 , add;
	
	for(start = 0; i <= 3; start += 7, ++i){ //check negative slop
		for(add = start; j <= 4; add++, j++ ){
			if(checkFour(board, add, add + 8, add + 8*2, add + 8*3) == 1){
				savingWinnnigPos(add, add + 8, add + 8*2, add + 8*3);
			
				return 1;
			}
		}
		j = 1;
	}
	i = 1;
	j = 1;
	
	for(start = 3; i <= 3; start += 7, ++i){ //check positive slope
		for(add = start; j <= 4; add++, j++){
			if(checkFour(board, add, add + 6, add + 6*2, add + 6*3)){
				savingWinnnigPos(add, add + 6, add + 6*2, add + 6*3);
				
				return 1;
			}
		}
		j = 1;
	} 
	return 0;
}
