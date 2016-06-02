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
int playMode;

int main(void){
	char board[ROW * COL];
	char player[SIZE] = "OX"; //player's sign
	int round = 2; //first round number
	int playagain;
	
	do{
		gameMode(board, round);
		
		printBoard(board); //print the first board
	
		if(playMode == 1 || playMode == 3){// new game : human VS. human
		
			if(playMode == 3){ //load game : human v.s human
				round = loadFile(board, round);
			} 
			
			takeTurn(round, board, player); 

			while(checkWin(insertPos, board) != 1 && round != ROW * COL + 1){ //  win : 1
				++round; //next round
		
				takeTurn(round, board, player); //take turn
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
		
		else if (playMode == 2 || playMode == 4){ //new game :  human VS. robot  
		
			if(playMode == 4){ //load game : human v.s robot
				round = loadFile(board, round);
			}
			
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
	}
	else{
		printf("\nThanks for playing.\n\n\n");
	}
	
	
	system("pause");
	return 0;
}

int gameMode(char *board, int round){
	int gameMode, newMode;
	
	printf("Human VS. Human: 1\nHuman VS. Robot: 2\n");
	scanf("%d", &gameMode);
	
	while(gameMode != 1 && gameMode != 2 ){
		printf("Enter 1 or 2 \n");
		scanf("%d", &gameMode);
	}
	
	printf("New Game : 1 \nLoad Game : 2\n");
	scanf("%d", &newMode);
	
	while(newMode != 1 && newMode != 2 ){
		printf("Enter 1 or 2 \n");
		scanf("%d", &newMode);
	}
	
	if(gameMode == 1 && newMode == 1){//New Game : Human VS. Human
		memset(board,' ', ROW * COL); //innitialize array with blank
		playMode = 1;
	}
	else if(gameMode == 2 && newMode == 1){//New Game : Human VS. Robot
		memset(board,' ', ROW * COL); //innitialize array with blank
		playMode = 2;
	}
	else if (gameMode == 1 && newMode == 2){ //Loading Game : human v.s human
		playMode = 3;                  
		return loadFile(board, round); //return old board and old round
	}
	else if (gameMode == 2 && newMode == 2){ //loadding Game : human V.s robot
		playMode = 4;
		return loadFile(board, round);
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
		if(i == insertPos){
				printf("|", board[i]);
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY | FOREGROUND_BLUE);//print blue
				printf(" %c ", board[i]); //print board with winning position
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);//print white
		}
		else{
			printf("| %c ", board[i]); //print |board[0]|board[1]|board[2]|...
		}
		
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
		position=inputChess();
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
					insertPos = bottom; //get final insert position store in global variable
					printBoard(board); //print new board
					
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
} 

int inputChess(void){//倒數計時 
	int sec;//剩餘秒數 
	int position=-1;//輸入第幾行(1~7) 
	char input=-1;//輸入字元 
    
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
		position=input-48;//將char轉換為int 
		return position;//將行數回傳 
	}
	
	
}

void robotTakeTurn(int round, char *board, char *player){
	int bottom, test = 0;
	
	//human move
	printf("player (%c) : Enter 1 ~ 7, 0 to Save Current Game\n", player[round % 2]); //player go first
	position=inputChess(); 
	
	
	if(position > 0){//check human move
		while(test == 0){  //check whether insertion is complete, if complete, test = 1
			for(bottom = position + 34; bottom >= position - 1; bottom -= 7){ //check whether the position in board is blank from bottom
				if(board[bottom] == ' '){
					board[bottom] = player[round % 2]; //insert player sign to board
					test = 1; //insert complete
					insertPos = bottom; //get final insert position store in global variable
					printBoard(board); //print new board
					
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
						insertPos = bottom; //get final insert position store in global variable
						printBoard(board); //print new board
						
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
	
	for(i=1;i<=5;i++){//checking tilted is three or not
		for(j=i+7;j<=(i+28);j=j+7){
			if(i>=1&&i<=4&&j>13){// XOOO_ 正斜率  j>13:上面兩列不檢查 
				if(board[j]!=' '&&board[j]==board[j-6]&&board[j]==board[j+6]){
					if(board[j-12]==' '&&board[j-5]!=' '){
						input=(j-12)%7+1;
						return input;
					}
				}
			}
			if(i>=2&&i<=5&&j<28&&j>=7){// _OOOX 正斜率  j<28:下面兩列不檢查	j>=21 : 判斷是否為最底層 !!
				if(board[j]!=' '&&board[j]==board[j-6]&&board[j]==board[j+6]){
					if(board[j+12]==' '&&((j<21&&board[j+19]!=' ')||j>=21)){
						input=(j+12)%7+1;
						return input;
					}
				}
			}
			
			if(i>=2&&i<=5&&j>13){// _OOOX 負斜率  j>13:上面兩列不檢查 
				if(board[j]!=' '&&board[j]==board[j-8]&&board[j]==board[j+8]){
					if(board[j-16]==' '&&board[j-9]!=' '){
						input=(j-16)%7+1;
						return input;
					}
				}
			}
			if(i>=1&&i<=4&&j<28&&j>6){// XOOO_ 負斜率  j<28:下面兩列不檢查	j>=21 : 判斷是否為最底層 !!
				if(board[j]!=' '&&board[j]==board[j-8]&&board[j]==board[j+8]){
					if(board[j+16]==' '&&((j<21&&board[j+23]!=' ')||j>=21)){
						input=(j+16)%7+1;
						return input;
					}
				}
			}
		}
	}
	
	for(i=1;i<=5;i++){//checking horizontal is three or not,  j>=35 : 判斷是否為最底層 !!
		for(j=i;j<=(i+35);j=j+7){
			
			if(i>=1&&i<=4){// XOOO_ 
				if(board[j]!=' '&&board[j]==board[j+1]&&board[j]==board[j-1]){
					if(board[j+2]==' '&&((j<35&&board[j+9]!=' ')||j>=35)){
						input=(j+2)%7+1;
						return input;
					}
				}
			}
			if(i>=2&&i<=5){// _OOOX 
				if(board[j]!=' '&&board[j]==board[j+1]&&board[j]==board[j-1]){
					if(board[j-2]==' '&&((j<35&&board[j+5]!=' ')||j>=35)){
						input=(j-2)%7+1;
						return input;
					}
				}
			}
		
			
			//checking horizontal is two or not
			if(board[j]!=' '&&board[j]==board[j+1]&&board[j-1]==' '&&board[j+2]==' '){// _OO_
				if(board[j+2]==' '&&((j<35&&board[j+9]!=' ')||j>=35)){// XOO_ 
					input=(j+2)%7+1;
					return input;
				}
				
				if(board[j-1]==' '&&((j<35&&board[j+6]!=' ')||j>=35)){// _OOX
					input=(j-1)%7+1;
					return input;
				}
			}
			
			if(board[j-1]!=' '&&board[j+1]==board[j-1]&&board[j-2]==' '&&board[j+2]==' '){// _O_O_
				if(board[j]==' '&&((j<35&&board[j+7]!=' ')||j>=35)){
					input=j%7+1;
					return input;
				}
			}
		}
	}
	
	for(i=1;i<=5;i++){//checking tilted is two or not
		for(j=i+7;j<=(i+28);j=j+7){
			
			if(board[j]!=' '&&board[j]==board[j-6]&&board[j-12]==' '&&board[j+6]==' '){// _OO_ 正斜率
				if(i>=1&&i<=4&&j>13&&board[j-12]==' '&&board[j-5]!=' '){// XOO_ 正斜率
					input=(j-12)%7+1;
					return input;
				}
				
				if(i>=2&&i<=5&&board[j+6]==' '&&((j<28&&board[j+16]!=' ')||j>=28)){// _OOX 正斜率
					input=(j+6)%7+1;
					return input;
				}
			}
			
			if(board[j-6]!=' '&&board[j-6]==board[j+6]&&j>=14&&j<35&&board[j-12]==' '&&board[j+12]==' '){// _O_O_ 正斜率
				if(board[j]==' '&&board[j+7]!=' '){
					input=j%7+1;
					return input;
				}
			}
			
			if(board[j]!=' '&&board[j]==board[j-8]&&board[j-16]==' '&&board[j+8]==' '){// _OO_ 負斜率
				if(j>13&&board[j-16]==' '&&board[j-9]!=' '){// _OOX 負斜率
					input=(j-16)%7+1;
					return input;
				}
				
				if(j<35&&board[j+8]==' '&&((j<28&&board[j+15]!=' ')||j>=28)){// XOO_ 負斜率
					input=(j+8)%7+1;
					return input;
				}
			}
			
			if(board[j-8]!=' '&&board[j-8]==board[j+8]&&j>=14&&j<35&&board[j+16]==' '&&board[j-16]==' '){// _O_O_ 負斜率
				if(board[j]==' '&&board[j+7]!=' '){
					input=j%7+1;
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
		for(i=4;i!=7;i++){//往中間下 
	
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
