#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>
#define ROW 6 //define board
#define COL 7
#define SIZE 3

int robotInputChess(char *board);
void robotTakeTurn(int round, char *board, char *player);
int inputChess(void);
int gameMode(char *board, int round);
void printBoard(char *board);
void printWiningBoard(char *board);
void takeTurn(int round, char *board, char *player);
void savingWinnigPos(int a,int b,int c,int d);
bool checkWin(int insertPos, char *board);
bool checkFour(char *board, int a, int b, int c, int d);
bool checkVertical(int insertPos, char *board);
bool checkHorizontal(int insertPos, char *board);
bool checkTilted(int insertPos, char *board);
void saveFile(char *board);
int loadFile(char *boar, int round);

int insertPos = 0; //get final insert position in takeTurn function
int winningPos[4];//the four winning position
int position; //user enter number

int main(void){
	char board[ROW * COL];
	char player[SIZE] = "OX"; //player's sign
	int round = 2; //first round number
	int playagain;
	
	do{
		round = gameMode(board, round);
		printf("round = %d", round);
		
		printBoard(board); //print the first board
	
		if(round==2){//human VS. human
			takeTurn(round, board, player); 
			//printf("insertPOs %d = %d\n", round % 2 + 1 , insertPos);
			while(checkWin(insertPos, board) != 1 && round != ROW * COL + 1){ //  win : 1
				++round; //next round
		
				takeTurn(round, board, player); //take turn
				//printf("insertPOs %d = %d\n", round % 2 + 1 , insertPos);

			}
			
			if(checkWin(insertPos, board)==1 && position != 0){
				printWiningBoard(board);
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY | FOREGROUND_RED);//print red
				printf("Player %d (%c) Wins!\n\n\n", round % 2 + 1, player[round % 2]); //win
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);//print white
			}
			else if(checkWin(insertPos, board) != 1 && round == ROW * COL + 1){ //tie
				printf("Tie !\n");
			}
		}
		
		
		if(round==4){// round==4 : new human VS. robot  
			
			robotTakeTurn(round, board, player);
			while(checkWin(insertPos, board) != 1 && round != ROW * COL + 1){ //  win : 1
		
				robotTakeTurn(round, board, player); //take turn
			}
		}
		

		if(position != 0){
			printf("Enter (1) to play again\n");
			printf("Enter (0) to end the game\n");
			scanf("%d",&playagain);
		}
		
	}while(playagain == 1);
	
	if(position == 0){
		printf("\nSaving Complete!\n");
		printf("\nThanks for playing.\n\n\n");
		printf("round = %d, position = %d", round, position);
	}
	else{
		printf("\nThanks for playing.\n\n\n");
	}
	
	
	system("pause");
	return 0;
}

int gameMode(char *board, int round){
	int mode;
	
	printf("New Game Human VS. Human: 1\nNew Game Human VS. Robot: 2\nLoading Game : 3\n");
	scanf("%d", &mode);
	
	while(mode != 1 && mode != 2 && mode != 3){
		printf("Enter 1 or 2 or 3.\n");
		scanf("%d", &mode);
	}
	
	if(mode == 1){//New Game Human VS. Human
		memset(board,' ', ROW * COL); //innitialize array with blank
		return 2;
	}
	else if(mode == 2){//New Game Human VS. Robot
		memset(board,' ', ROW * COL); //innitialize array with blank
		return 4;
	}
	else if (mode == 3){//Loading Game
		return loadFile(board, round); //return old board and old round
	}
}

void printBoard(char *board){
	
	size_t i;
	
	puts("\n--------Connect Four---------\n");
	
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
	int bottom, test = 0;
	
	do{
		printf("player %d (%c) : Enter 1 ~ 7, 0 to Save Current Game\n", round % 2 + 1, player[round % 2]); //player one go first
		position = inputChess();
		if(position==-1){
			round++;
		}
		//system("pause");
	
	}while(position==-1);//若沒有輸入行數則輪到下個玩家 
	
	
	if(position > 0){
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
	else if(position == 0){
		saveFile(board); //save record to file
	}

}

void savingWinnigPos(int a,int b,int c,int d){
	winningPos[0] = a;
	winningPos[1] = b;
	winningPos[2] = c;
	winningPos[3] = d;
	}

bool checkWin(int insertPos, char *board){
	if(checkVertical(insertPos, board) || checkHorizontal(insertPos, board) || checkTilted(insertPos, board) || position == 0){
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
				savingWinnigPos(add, add + 7, add + 7*2, add + 7*3);
				
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
			savingWinnigPos(add, add + 1, add + 2, add + 3);
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
				savingWinnigPos(add, add + 8, add + 8*2, add + 8*3);
			
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
				savingWinnigPos(add, add + 6, add + 6*2, add + 6*3);
				
				return 1;
			}
		}
		j = 1;
	} 
	return 0;
}

void saveFile(char *board){
	FILE *file;
	
	file = fopen("record.txt", "w");
	fwrite(board, sizeof(char), COL * ROW, file); //write board to file
	fclose(file);
}

int loadFile(char *board, int round){
	int i = 0, count = 0;
	FILE *file;
	
	file = fopen("record.txt", "r");
	
	while((fscanf(file,"%c",&board[i])) != EOF) //scanf and check EOF
        {
            i++;
        }
        
    fclose(file);
    
    for(i = 0; i < COL*ROW; ++i){
    	if(board[i] != ' '){
    		count++;
		}
	}
	if(count % 2 == 1 || count == 1){
		return 3; //player two play
	}
	else{
		return 2; //player one play
	}
} 

int inputChess(void){//倒數計時 
	int sec;//剩餘秒數 
	int position = -1;//輸入第幾行(1~7) 
	char input = -1;//輸入字元 
    
    for(sec=10;sec>0;sec--){//每秒印出剩餘秒數 
    	Sleep(1000);
    	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY | FOREGROUND_RED);//紅字 
    	printf("%d  ",sec);
    	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);//白字 
    	
    	if (kbhit()){//判定是否有輸入字元 
    		input=getche();
    		break;//若有，則停止倒數 
		}
	}

    
	if(input==-1){
		printf("\nThe time is up\n");
    	printf("Take turn!!\n\n");
		return position;//若沒輸入行數，則傳回-1
	}
	else if(input!=-1){
		position = input-48;//將char轉換為int 
		return position;//將行數回傳 
	}
	
	
}

void robotTakeTurn(int round, char *board, char *player){
	int bottom, test = 0;
	
	//human move
	printf("player (%c) : Enter 1 ~ 7, 0 to Save Current Game\n", player[round % 2]); //player go first
	position = inputChess(); 
	
	
	if(position > 0){//check human move
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
	else if(position == 0){
		saveFile(board); //save record to file
	}
	
	if(checkWin(insertPos, board)==1 && position != 0){//if human wins
				printWiningBoard(board);
				if(round%2==0){
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY | FOREGROUND_RED);//print red
					printf("Player %d (%c) Wins!\n\n\n", round % 2 + 1, player[round % 2]); //win
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);//print white
				}
			}
			else if(checkWin(insertPos, board) != 1 && round == ROW * COL + 1){ //tie
				printf("Tie !\n");
			}
	
	
	if(checkWin(insertPos, board)!=1 && position != 0){// if human didn't win
		round++;
		position=robotInputChess(board);//robot move
	
		if(position > 0){//check robot move
			test=0;
			while(test == 0){  //check whether insertion is complete, if complete, test = 1
				for(bottom = position + 34; bottom >= position - 1; bottom -= 7){ //check whether the position in board is blank from bottom
					if(board[bottom] == ' '){
						board[bottom] = player[round % 2]; //insert player sign to board
						test = 1; //insert complete
					
						printBoard(board); //print new board
						insertPos = bottom; //get final insert position store in global variable
						break; //end for loop
					}
				}
			}
		}
	
		if(checkWin(insertPos, board)==1 && position != 0){//if robot wins
			printWiningBoard(board);
			if(round%2==1){
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY | FOREGROUND_RED);//print red
				printf("Robot (%c) Wins!\n\n\n", round % 2 + 1, player[round % 2]); //win
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);//print white
			}		
		}
		else if(checkWin(insertPos, board) != 1 && round == ROW * COL + 1){ //tie
			printf("Tie !\n");
		}	
	}
	//system("pause");
}

int robotInputChess(char *board){
	int i=0,j=0,k=1,input;
	
	for(i=0;i<=6;i++){//checking vertical is three or not 
		for(j=i+7;j<=(i+21);j=j+7){
			if(board[j]!=' '&&board[j]==board[j+7]&&board[j+7]==board[j+14]&&board[j-7]==' '){
				input=j%7+1;
				return input;
			}
		}
	}
	
	for(i=1;i<=5;i++){//checking horizontal is three or not,  j>=35 : 判斷是否為最底層 !!
		for(j=i;j<=(i+35);j=j+7){
			
			if(i>=1&&i<=4){// OOO_
				if(board[j]!=' '&&board[j]==board[j+1]&&board[j]==board[j-1]){
					if(board[j+2]==' '&&(board[j+9]!=' '||j>=35)){
						input=(j+2)%7+1;
						return input;
					}
				}
			}
			if(i>=2&&i<=5){// _OOO 
				if(board[j]!=' '&&board[j]==board[j+1]&&board[j]==board[j-1]){
					if(board[j-2]==' '&&(board[j+5]!=' '||j>=35)){
						input=(j-2)%7+1;
						return input;
					}
				}
			}
			
			//printf("\nj is %d:%c\n",j,board[j]);
			
			if(board[j-1]!=' '&&board[j+1]==board[j-1]){// _O_O_
				if(board[j]==' '&&(board[j+7]!=' '||j>=35)){
					input=j%7+1;
					return input;
				}
			}
			
			if(board[j]!=' '&&board[j]==board[j+1]&&board[j-1]==' '&&board[j+2]==' '){// _OO_
				if(board[j+2]==' '&&(board[j+9]!=' '||j>=35)){// XOO_ 
					input=(j+2)%7+1;
					return input;
				}
				
				if(board[j-1]==' '&&(board[j+6]!=' '||j>=35)){// _OOX
					input=(j-1)%7+1;
					return input;
				}
			}
		}
	}

	
	if(board[3]==' '){//往中間下 
		input=4;
		return input;
	}
	else if(board[3]!=' '){
		for(i=4;i!=6;i++){//往中間下 
	
			if(board[i]==' '){
				input=i+1;
				return input;
			}
			else if(board[i]!=' '){
				j=i;
				j=j-2*k;
				if(board[j]==' '){
					input=j+1;
					return input;
				}
			}
			k++;
		}	
	}
}
