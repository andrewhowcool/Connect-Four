#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>//sleep
#include <conio.h>//kbhit
#define ROW 6 //define board
#define COL 7
#define SIZE 3

int robotCheckWin(char *board,int bottom);//檢查電腦下子後下一步人類會不會贏，如果會贏就傳回 1
int robotInputChess(char *board);//判斷電腦要下在哪一行，並傳回行數
bool robotTakeTurn(int round, char *board, char *player);//電腦對戰模式的流程
int inputChess(void);//人類下棋，包含倒數計時
int gameMode(char *board, int round); //選擇遊戲模式
void printBoard(char *board); //印出棋盤
void printWiningBoard(char *board);//印出有紅色連線的獲勝棋盤
int takeTurn(int round, char *board, char *player); //轉換玩家
void savingWinnigPos(int a,int b,int c,int d); //存連線四子的位置
bool checkWin(int insertPos, char *board);
bool checkFour(char *board, int a, int b, int c, int d);
bool checkVertical(int insertPos, char *board);
bool checkHorizontal(int insertPos, char *board);
bool checkTilted(int insertPos, char *board);
bool tieCheck(char *board);
void saveFile(char *board);
int loadFile(char *boar, int round);



int insertPos = 0; //get final insert position in takeTurn function
int winningPos[4];//the four winning position
int position; //user enter number
int playMode;

int main(void){
	char board[ROW * COL];  //初始棋盤格數
	char player[SIZE] = "OX"; //player's sign
	int round = 2; //first round number
	int playagain, i, tieTest = 1;
	
	/**
	 * 1.玩家輸入遊戲模式 : 對玩家、對AI；新遊戲、讀取遊戲
	 * 2.每一步下子後判斷有無輸贏，如果沒有就繼續下
	 * 如果贏了就跳出下棋迴圈
	 * 判斷是誰贏了
	 * 無輸贏則和局
	**/
	
	do{
		gameMode(board, round); //玩家輸入遊戲模式
		
		printBoard(board); //print the first board
	
		if(playMode == 1 || playMode == 3){// new game : human VS. human
		
			if(playMode == 3){ //load game : human v.s human
				round = loadFile(board, round);//回傳舊遊戲的round，決定換誰下
			} 
			
			takeTurn(round, board, player); //player 1 先下

			while(checkWin(insertPos, board) != 1 && !tieCheck(board)){ //  win : 1
				++round; //next round
				round = takeTurn(round, board, player); //如果玩家超過時間就進入下一round
			}
			
			if(checkWin(insertPos, board) == 1 && position != 0){ //獲勝條件
				printWiningBoard(board);
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY | FOREGROUND_RED);//print red
				printf("Player %d (%c) Wins!\n\n\n", round % 2 + 1, player[round % 2]); //win
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);//print white
			}
			if(tieCheck(board)){ //和局
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY | FOREGROUND_RED);
				printf("Tie ! \n\n\n");
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
			}
		}
		
		else if (playMode == 2 || playMode == 4){ //new game :  human VS. robot  
		
			if(playMode == 4){ //load game : human v.s robot
				round = loadFile(board, round);
			}
			
			robotTakeTurn(round, board, player); 
			
			while(checkWin(insertPos, board) != 1){ //  win : 1
		
				if(robotTakeTurn(round, board, player)){
					break; //如果發生和局就停止下棋     
				}
			}
			
			if(tieCheck(board)){
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY | FOREGROUND_RED);
				printf("Tie ! \n\n\n");
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
			}
		}
		if(position != 0){
			printf("Enter (1) to play again\n");
			printf("Enter (0) to end the game\n");
			scanf("%d",&playagain);
		}
		
	}while(playagain == 1);
	
	if(position == 0){ //存檔
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

	/**
	 * 1.玩家輸入下棋位置1 ~ 7, 0存檔
	 * 超過時間則直接換下個玩家輸入
	 * 檢查下棋點能否下棋
	 * 如果可以從底部開始擺放
	 * 如果滿格則重新下棋
	 * */

int takeTurn(int round, char *board, char *player){
	int bottom, test = 0;
	
	do{
		printf("player %d (%c) : Enter 1 ~ 7, 0 to Save Current Game\n", round % 2 + 1, player[round % 2]); //player one go first
		position = inputChess(); //玩家輸入位置並開始計秒
		if(position == -1){
			round++; //超過時間沒下換下一round
		}

	
	}while(position == -1);//下round玩家輸入
	
	
	if(position > 0){ //確認輸入值
		while(test == 0){  //check whether insertion is complete, if complete, test = 1
			for(bottom = position + 34; bottom >= position - 1; bottom -= 7){ //check whether the position in board is blank from bottom
				if(board[bottom] == ' '){
					board[bottom] = player[round % 2]; //insert player sign to board
					test = 1; //insert complete
					insertPos = bottom; //get final insert position store in global variable
					printBoard(board); //print new board
					
					break; //end for loop
				}
			else if(bottom == position - 1 && board[bottom] != ' '){ //欄的最頂部已滿
				printf("\n");
				printf("Column full! Enter other column.\n");
				position = inputChess(); //滿格重下
				}
			}
		}
	}
	else if(position == 0){
		saveFile(board); //save record to file
	}
	return round;
}

void savingWinnigPos(int a,int b,int c,int d){
	winningPos[0] = a;
	winningPos[1] = b;
	winningPos[2] = c;
	winningPos[3] = d;
	}

bool tieCheck(char *board){
	int i, tieTest = 1;
	
	for(i = 0; i < COL; ++i){
		if(board[i] == ' '){
			tieTest = 0;
			return tieTest;
		}
	}
	return tieTest;
}

bool checkWin(int insertPos, char *board){
	if(checkVertical(insertPos, board) || checkHorizontal(insertPos, board) || checkTilted(insertPos, board) || position == 0){
		return 1; //直、橫、斜線任一滿足四子就算贏
	}
	else{
		return 0;
	}
}

bool checkFour(char *board, int a, int b, int c, int d){
	if(board[a] == board[b] && board[b] == board[c] && board[c] == board[d] 
		&& d < COL * ROW && d >= 0 && a >= 0 && a < COL * ROW && board[a] != ' '){ // a:start, d:end must between 0 ~ 41
		return 1; //檢查相同四子
	}
	else{
		return 0;
	}
}

bool checkVertical(int insertPos, char *board){ //檢查垂直線
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
    	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY | FOREGROUND_RED);//紅色 
    	printf("%d  ",sec);
    	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);//白色
    	
    	if (kbhit()){//判定是否有輸入字元 
			input=getche();
			while(input<48||input>55){
    			printf("\nERROR, enter1~7 or 0 to save the file\n");
    			input=getche();
    			if(input>=48&&input<=55){
    				break;//若有，則停止倒數
				}
			} 
			if(input >= 48 && input<=55){
    			break;//若有，則停止倒數
			}
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

bool robotTakeTurn(int round, char *board, char *player){
	int bottom, test = 0, i, tieTest = 1;
	
	//human move
	printf("player (%c) : Enter 1 ~ 7, 0 to Save Current Game\n", player[round % 2]); //player go first
	position = inputChess(); 
	
	
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
					printf("\n");
					printf("Column full! Enter other column.\n");
					position = inputChess();
					}
				}
			}
		}
	else if(position == 0){
		saveFile(board); //save record to file
	}
	
	if(checkWin(insertPos, board) == 1 && position != 0){//if human wins
		printWiningBoard(board);

		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY | FOREGROUND_RED);//print red
		printf("Player 1 (O) Wins!\n\n\n"); //win
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);//print white
		}
		
	if(tieCheck(board)){
		return tieCheck(board);
	}
	
	if(checkWin(insertPos, board)!= 1 && position != 0){// if human didn't win
		round++;
		position = robotInputChess(board);//robot move
	
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
	
		if(checkWin(insertPos, board) == 1 && position != 0){//if robot wins
			printWiningBoard(board);
			{
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY | FOREGROUND_RED);//print red
				printf("Robot (X)  Wins!\n\n\n"); //win
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);//print white
			}		
		}		
	}
	//system("pause");
}

int robotInputChess(char *board){
	int i=0,j=0,k=2;
	int bottom,input;
	
	/*
		判定順序：
		1.判定電腦是否有立即贏的機會(3子連成4子) 
		2.判定玩家有沒有立即贏的威脅(3子連成4子) 
		3.判定電腦或玩家有沒有機會變成有利的局面(2子連成3子) 
		4.若都沒有以上的情況，隨機下棋(不做球給玩家的前提下)
		5.無論如何電腦都輸定了，由左到右找到空的一行便下子 
	*/
	
	
	//檢查電腦(X)有沒有立即贏的機會 
	for(i=0;i<=6;i++){//checking vertical is three or not 
		for(j=i+7;j<=(i+21);j=j+7){
			if(board[j]=='X'&&board[j]==board[j+7]&&board[j+7]==board[j+14]&&board[j-7]==' '){
				input=j%7+1;
				printf("a");//test
				return input;
			}
		}
	}
	
	for(i=1;i<=5;i++){//checking tilted is three or not
		for(j=i+7;j<=(i+28);j=j+7){
			if(i>=1&&i<=4&&j>13&&j<35){// XOOO_ 正斜率  j>13:上面兩列不檢查 
				if(board[j]=='X'&&board[j]==board[j-6]&&board[j]==board[j+6]){
					if(board[j-12]==' '&&board[j-5]!=' '){
						input=(j-12)%7+1;
						printf("b");//test
						return input;
					}
				}
			}
			if(i>=2&&i<=5&&j<28&&j>=7){// _OOOX 正斜率  j<28:下面兩列不檢查	j>=21 : 判斷是否為最底層 !!
				if(board[j]=='X'&&board[j]==board[j-6]&&board[j]==board[j+6]){
					if(board[j+12]==' '&&((j<21&&board[j+19]!=' ')||j>=21)){
						input=(j+12)%7+1;
						printf("c");//test
						return input;
					}
				}
			}
			if(i>=1&&i<=5&&j>13&&j<35){// XOO_OX 正斜率  j>13:上面兩列不檢查 
				if(board[j]=='X'&&board[j]==board[j+6]&&board[j]==board[j-12]){
					if(board[j-6]==' '&&board[j+1]!=' '){
						input=(j-6)%7+1;
						printf("d");//test
						return input;
					}
				}
			}
			if(i>=1&&i<=5&&j<28&&j>=7){// XO_OOX 正斜率  j>13:上面兩列不檢查 
				if(board[j]=='X'&&board[j]==board[j+6]&&board[j]==board[j-12]){
					if(board[j+6]==' '&&board[j+13]!=' '){
						input=(j-6)%7+1;
						printf("e");//test
						return input;
					}
				}
			}
			if(i>=2&&i<=5&&j>13){// _OOOX 負斜率  j>13:上面兩列不檢查 
				if(board[j]=='X'&&board[j]==board[j-8]&&board[j]==board[j+8]){
					if(board[j-16]==' '&&board[j-9]!=' '){
						input=(j-16)%7+1;
						printf("f");//test
						return input;
					}
				}
			}
			if(i>=1&&i<=4&&j<28&&j>6){// XOOO_ 負斜率  j<28:下面兩列不檢查	j>=21 : 判斷是否為最底層 !!
				if(board[j]=='X'&&board[j]==board[j-8]&&board[j]==board[j+8]){
					if(board[j+16]==' '&&((j<21&&board[j+23]!=' ')||j>=21)){
						input=(j+16)%7+1;
						printf("g");//test
						return input;
					}
				}
			}
			if(i>=1&&i<=5&&j>13&&j<35){// XOO_OX 負斜率  j>13:上面兩列不檢查 
				if(board[j]=='X'&&board[j]==board[j-8]&&board[j]==board[j+16]){
					if(board[j+8]==' '&&board[j+15]!=' '){
						input=(j+15)%7+1;
						printf("h");//test
						return input;
					}
				}
			}
			if(i>=1&&i<=5&&j<28&&j>6){// XO_OOX 負斜率  j>13:上面兩列不檢查 
				if(board[j]=='X'&&board[j]==board[j+8]&&board[j]==board[j-16]){
					if(board[j-8]==' '&&board[j-1]!=' '){
						input=(j-8)%7+1;
						printf("i");//test
						return input;
					}
				}
			}
		}
	}
	
	for(i=1;i<=5;i++){//checking horizontal is three or not,  j>=35 : 判斷是否為最底層 !!
		for(j=i;j<=(i+35);j=j+7){
			
			if(i>=1&&i<=4){// XOOO_ 
				if(board[j]=='X'&&board[j]==board[j+1]&&board[j]==board[j-1]){
					if(board[j+2]==' '&&((j<35&&board[j+9]!=' ')||j>=35)){
						input=(j+2)%7+1;
						printf("j");//test
						return input;
					}
				}
			}
			if(i>=2&&i<=5){// _OOOX 
				if(board[j]=='X'&&board[j]==board[j+1]&&board[j]==board[j-1]){
					if(board[j-2]==' '&&((j<35&&board[j+5]!=' ')||j>=35)){
						input=(j-2)%7+1;
						printf("k");//test
						return input;
					}
				}
			}
			if(i>=1&&i<=5){// XOO_OX 
				if(board[j]=='X'&&board[j]==board[j-1]&&board[j]==board[j+2]){
					if(board[j+1]==' '&&((j<35&&board[j+8]!=' ')||j>=35)){
						input=(j+1)%7+1;
						printf("l");//test
						return input;
					}
				}
			}
			if(i>=2&&i<=5){// XO_OOX 
				if(board[j]=='X'&&board[j]==board[j-2]&&board[j]==board[j+1]){
					if(board[j-1]==' '&&((j<35&&board[j+6]!=' ')||j>=35)){
						input=(j-1)%7+1;
						printf("m");//test
						return input;
					}
				}
			}
		}
	}
	
	
	//檢查玩家(O)有沒有立即贏的威脅 
	for(i=0;i<=6;i++){//checking vertical is three or not 
		for(j=i+7;j<=(i+21);j=j+7){
			if(board[j]=='O'&&board[j]==board[j+7]&&board[j+7]==board[j+14]&&board[j-7]==' '){
				input=j%7+1;
				printf("n");//test
				return input;
			}
		}
	}
	
	for(i=1;i<=5;i++){//checking tilted is three or not
		for(j=i+7;j<=(i+28);j=j+7){
			if(i>=1&&i<=4&&j>13&&j<35){// XOOO_ 正斜率  j>13:上面兩列不檢查 
				if(board[j]=='O'&&board[j]==board[j-6]&&board[j]==board[j+6]){
					if(board[j-12]==' '&&board[j-5]!=' '){
						input=(j-12)%7+1;
						printf("o");//test
						return input;
					}
				}
			}
			if(i>=2&&i<=5&&j<28&&j>=7){// _OOOX 正斜率  j<28:下面兩列不檢查	j>=21 : 判斷是否為最底層 !!
				if(board[j]=='O'&&board[j]==board[j-6]&&board[j]==board[j+6]){
					if(board[j+12]==' '&&((j<21&&board[j+19]!=' ')||j>=21)){
						input=(j+12)%7+1;
						printf("p");//test
						return input;
					}
				}
			}
			if(i>=1&&i<=5&&j>13&&j<35){// XOO_OX 正斜率  j>13:上面兩列不檢查 
				if(board[j]=='O'&&board[j]==board[j+6]&&board[j]==board[j-12]){
					if(board[j-6]==' '&&board[j+1]!=' '){
						input=(j-6)%7+1;
						printf("q");//test
						return input;
					}
				}
			}
			if(i>=1&&i<=5&&j<28&&j>=7){// XO_OOX 正斜率  j>13:上面兩列不檢查 
				if(board[j]=='O'&&board[j]==board[j+6]&&board[j]==board[j-12]){
					if(board[j+6]==' '&&board[j+13]!=' '){
						input=(j-6)%7+1;
						printf("r");//test
						return input;
					}
				}
			}
			if(i>=2&&i<=5&&j>13){// _OOOX 負斜率  j>13:上面兩列不檢查 
				if(board[j]=='O'&&board[j]==board[j-8]&&board[j]==board[j+8]){
					if(board[j-16]==' '&&board[j-9]!=' '){
						input=(j-16)%7+1;
						printf("s");//test
						return input;
					}
				}
			}
			if(i>=1&&i<=4&&j<28&&j>6){// XOOO_ 負斜率  j<28:下面兩列不檢查	j>=21 : 判斷是否為最底層 !!
				if(board[j]=='O'&&board[j]==board[j-8]&&board[j]==board[j+8]){
					if(board[j+16]==' '&&((j<21&&board[j+23]!=' ')||j>=21)){
						input=(j+16)%7+1;
						printf("t");//test
						return input;
					}
				}
			}
			if(i>=1&&i<=5&&j>13&&j<35){// XOO_OX 負斜率  j>13:上面兩列不檢查 
				if(board[j]=='O'&&board[j]==board[j-8]&&board[j]==board[j+16]){
					if(board[j+8]==' '&&board[j+15]!=' '){
						input=(j+15)%7+1;
						printf("u");//test
						return input;
					}
				}
			}
			if(i>=1&&i<=5&&j<28&&j>6){// XO_OOX 負斜率  j>13:上面兩列不檢查 
				if(board[j]=='O'&&board[j]==board[j+8]&&board[j]==board[j-16]){
					if(board[j-8]==' '&&board[j-1]!=' '){
						input=(j-8)%7+1;
						printf("v");//test
						return input;
					}
				}
			}
		}
	}
	
	for(i=1;i<=5;i++){//checking horizontal is three or not,  j>=35 : 判斷是否為最底層 !!
		for(j=i;j<=(i+35);j=j+7){
			
			if(i>=1&&i<=4){// XOOO_ 
				if(board[j]=='O'&&board[j]==board[j+1]&&board[j]==board[j-1]){
					if(board[j+2]==' '&&((j<35&&board[j+9]!=' ')||j>=35)){
						input=(j+2)%7+1;
						printf("w");//test
						return input;
					}
				}
			}
			if(i>=2&&i<=5){// _OOOX 
				if(board[j]=='O'&&board[j]==board[j+1]&&board[j]==board[j-1]){
					if(board[j-2]==' '&&((j<35&&board[j+5]!=' ')||j>=35)){
						input=(j-2)%7+1;
						printf("x");//test
						return input;
					}
				}
			}
			if(i>=1&&i<=5){// XOO_OX 
				if(board[j]=='O'&&board[j]==board[j-1]&&board[j]==board[j+2]){
					if(board[j+1]==' '&&((j<35&&board[j+8]!=' ')||j>=35)){
						input=(j+1)%7+1;
						printf("y");//test
						return input;
					}
				}
			}
			if(i>=2&&i<=5){// XO_OOX 
				if(board[j]=='O'&&board[j]==board[j-2]&&board[j]==board[j+1]){
					if(board[j-1]==' '&&((j<35&&board[j+6]!=' ')||j>=35)){
						input=(j-1)%7+1;
						printf("z");//test
						return input;
					}
				}
			}
			
			//檢查玩家或電腦是否有機會三子連線 
			//checking horizontal is two or not
			if(board[j]!=' '&&board[j]==board[j+1]&&board[j-1]==' '&&board[j+2]==' '&&((j<35&&board[j+6]!=' '&&board[j+9]!=' ')||(j>=35))){// _OO_
				if(board[j+2]==' '&&((j<35&&board[j+9]!=' ')||j>=35)){// XOO_ 
					input=(j+2)%7+1;
					printf("A");//test
					return input;
				}
				
				if(board[j-1]==' '&&((j<35&&board[j+6]!=' ')||j>=35)){// _OOX
					input=(j-1)%7+1;
					printf("B");//test
					return input;
				}
			}
			
			if(board[j-1]!=' '&&board[j+1]==board[j-1]&&board[j-2]==' '&&board[j+2]==' '&&((j<35&&board[j+6]!=' '&&board[j+8]!=' ')||(j>=35))){// _O_O_
				if(board[j]==' '&&((j<35&&board[j+7]!=' ')||j>=35)){
					input=j%7+1;
					printf("C");//test
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
					printf("D");//test
					return input;
				}
				
				if(i>=2&&i<=5&&board[j+6]==' '&&((j<28&&board[j+16]!=' ')||j>=28)){// _OOX 正斜率
					input=(j+6)%7+1;
					printf("E");//test
					return input;
				}
			}
			
			if(board[j-6]!=' '&&board[j-6]==board[j+6]&&j>=14&&j<35&&board[j-12]==' '&&board[j+12]==' '){// _O_O_ 正斜率
				if(board[j]==' '&&board[j+7]!=' '){
					input=j%7+1;
					printf("F");//test
					return input;
				}
			}
			
			if(board[j]!=' '&&board[j]==board[j-8]&&board[j-16]==' '&&board[j+8]==' '){// _OO_ 負斜率
				if(j>13&&board[j-16]==' '&&board[j-9]!=' '){// _OOX 負斜率
					input=(j-16)%7+1;
					printf("G");//test
					return input;
				}
				
				if(j<35&&board[j+8]==' '&&((j<28&&board[j+15]!=' ')||j>=28)){// XOO_ 負斜率
					input=(j+8)%7+1;
					printf("H");//test
					return input;
				}
			}
			
			if(board[j-8]!=' '&&board[j-8]==board[j+8]&&j>=14&&j<35&&board[j+16]==' '&&board[j-16]==' '){// _O_O_ 負斜率
				if(board[j]==' '&&board[j+7]!=' '){
					input=j%7+1;
					printf("I");//test
					return input;
				}
			}
		}
	}
	
	if(board[3]==' '){//往中間下 
		input=4;
		if(robotCheckWin(board,input)==0){//預判玩家下一步不會贏 
			printf("J");//test
			return input;
		}
		
	}
	else if(board[3]!=' '){//往右邊下
		for(i=4;i!=7;i++){ 
			if(board[i]==' '){
				input=i+1;
				if(robotCheckWin(board,input)==0){//預判玩家下一步不會贏 
					printf("K");//test
					return input;
				}
			}
			
			
			else if(board[i]!=' '){//右邊不為空，往左兩行 
				j=i;
				j=j-k;
				if(board[j]==' '){
					input=j+1;
					if(robotCheckWin(board,input)==0){//預判玩家下一步不會贏 
						printf("L");//test
						return input;
					}
				}
			}
			k=k+2;//往左四行或六行 
		}	
	}
	
	//萬一玩家贏定了 
	printf("yogamama\n");//test
	k=2;//initialize k=2
	if(board[3]==' '){//往中間下 
		input=4;
		printf("M");//test
		return input;	
	}
	else if(board[3]!=' '){//往右邊下
		for(i=4;i!=7;i++){ 
			if(board[i]==' '){
				input=i+1;
				printf("N");//test
				return input;
			}
			
			
			else if(board[i]!=' '){//右邊不為空，往左兩行 
				j=i;
				j=j-k;
				if(board[j]==' '){
					input=j+1;
					printf("O");//test
					return input;
				}
			}
			k=k+2;//往左四行或六行 
		}	
	}
	
	//棋盤滿了 
	else{
		return -1;//傳回-1，position <0 執行平手的判定 
	}
}

int robotCheckWin(char *board,int input){//檢查電腦下子後下一步人類會不會贏
	int i=0,j=0,k=1;
	int bottom,humanWins=0;
	
	//假設電腦下子
	for(bottom = input + 34; bottom >= input - 1; bottom -= 7){ //check whether the position in board is blank from bottom
		if(board[bottom] == ' '){
			board[bottom] = 'X'; //insert player sign to board						
			break; //end for loop
		}
	}
	
	//開始進行檢查
	for(i=0;i<=6;i++){//checking vertical is three or not 
		for(j=i+7;j<=(i+21);j=j+7){
			if(board[j]!=' '&&board[j]==board[j+7]&&board[j+7]==board[j+14]&&board[j-7]==' '){
				if(board[j]=='O'){//check if human will win or not
					humanWins=1;
					printf("1 ");//test
					board[bottom] = ' '; //復原!!
					return humanWins;
				}
			}
		}
	}
	
	for(i=1;i<=5;i++){//checking tilted is three or not
		for(j=i+7;j<=(i+28);j=j+7){
			if(i>=1&&i<=4&&j>13){// XOOO_ 正斜率  j>13:上面兩列不檢查 
				if(board[j]!=' '&&board[j]==board[j-6]&&board[j]==board[j+6]){
					if(board[j-12]==' '&&board[j-5]!=' '){
						if(board[j]=='O'){//check if human will win or not
							humanWins=1;
							printf("2 ");//test
							board[bottom] = ' '; //復原!!
							return humanWins;
						}
					}
				}
			}
			if(i>=2&&i<=5&&j<28&&j>=7){// _OOOX 正斜率  j<28:下面兩列不檢查	j>=21 : 判斷是否為最底層 !!
				if(board[j]!=' '&&board[j]==board[j-6]&&board[j]==board[j+6]){
					if(board[j+12]==' '&&((j<21&&board[j+19]!=' ')||j>=21)){
						if(board[j]=='O'){//check if human will win or not
							humanWins=1;
							printf("3 ");//test
							board[bottom] = ' '; //復原!!
							return humanWins;
						}
					}
				}
			}
			if(i>=1&&i<=5&&j>13&&j<35){// XOO_OX 正斜率  j>13:上面兩列不檢查 
				if(board[j]!=' '&&board[j]==board[j+6]&&board[j]==board[j-12]){
					if(board[j-6]==' '&&board[j+1]!=' '){
						if(board[j]=='O'){//check if human will win or not
							humanWins=1;
							printf("4 ");//test
							board[bottom] = ' '; //復原!!
							return humanWins;
						}
					}
				}
			}
			if(i>=1&&i<=5&&j<28&&j>=7){// XO_OOX 正斜率  j>13:上面兩列不檢查 
				if(board[j]!=' '&&board[j]==board[j+6]&&board[j]==board[j-12]){
					if(board[j+6]==' '&&board[j+13]!=' '){
						if(board[j]=='O'){//check if human will win or not
							humanWins=1;
							printf("5 ");//test
							board[bottom] = ' '; //復原!!
							return humanWins;
						}
					}
				}
			}
			if(i>=2&&i<=5&&j>13){// _OOOX 負斜率  j>13:上面兩列不檢查 
				if(board[j]!=' '&&board[j]==board[j-8]&&board[j]==board[j+8]){
					if(board[j-16]==' '&&board[j-9]!=' '){
						if(board[j]=='O'){//check if human will win or not
							humanWins=1;
							printf("6 ");//test
							board[bottom] = ' '; //復原!!
							return humanWins;
						}
					}
				}
			}
			if(i>=1&&i<=4&&j<28&&j>6){// XOOO_ 負斜率  j<28:下面兩列不檢查	j>=21 : 判斷是否為最底層 !!
				if(board[j]!=' '&&board[j]==board[j-8]&&board[j]==board[j+8]){
					if(board[j+16]==' '&&((j<21&&board[j+23]!=' ')||j>=21)){
						if(board[j]=='O'){//check if human will win or not
							humanWins=1;
							printf("7 ");//test
							board[bottom] = ' '; //復原!!
							return humanWins;
						}
					}
				}
			}
			if(i>=1&&i<=5&&j>13&&j<35){// XOO_OX 負斜率  j>13:上面兩列不檢查 
				if(board[j]!=' '&&board[j]==board[j-8]&&board[j]==board[j+16]){
					if(board[j+8]==' '&&board[j+15]!=' '){
						if(board[j]=='O'){//check if human will win or not
							humanWins=1;
							printf("8 ");//test
							board[bottom] = ' '; //復原!!
							return humanWins;
						}
					}
				}
			}
			if(i>=1&&i<=5&&j<28&&j>6){// XO_OOX 負斜率  j>13:上面兩列不檢查 
				if(board[j]!=' '&&board[j]==board[j+8]&&board[j]==board[j-16]){
					if(board[j-8]==' '&&board[j-1]!=' '){
						if(board[j]=='O'){//check if human will win or not
							humanWins=1;
							printf("9 ");//test
							board[bottom] = ' '; //復原!!
							return humanWins;
						}
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
						if(board[j]=='O'){//check if human will win or not
							humanWins=1;
							printf("10 ");//test
							board[bottom] = ' '; //復原!!
							return humanWins;
						}
					}
				}
			}
			if(i>=2&&i<=5){// _OOOX 
				if(board[j]!=' '&&board[j]==board[j+1]&&board[j]==board[j-1]){
					if(board[j-2]==' '&&((j<35&&board[j+5]!=' ')||j>=35)){
						if(board[j]=='O'){//check if human will win or not
							humanWins=1;
							printf("11 ");//test
							board[bottom] = ' '; //復原!!
							return humanWins;
						}
					}
				}
			}
			if(i>=1&&i<=5){// XOO_OX 
				if(board[j]!=' '&&board[j]==board[j-1]&&board[j]==board[j+2]){
					if(board[j+1]==' '&&((j<35&&board[j+8]!=' ')||j>=35)){
						if(board[j]=='O'){//check if human will win or not
							humanWins=1;
							printf("12 ");//test
							board[bottom] = ' '; //復原!!
							return humanWins;
						}
					}
				}
			}
			if(i>=2&&i<=5){// XO_OOX 
				if(board[j]!=' '&&board[j]==board[j-2]&&board[j]==board[j+1]){
					if(board[j-1]==' '&&((j<35&&board[j+6]!=' ')||j>=35)){
						if(board[j]=='O'){//check if human will win or not
							humanWins=1;
							printf("13 ");//test
							board[bottom] = ' '; //復原!!
							return humanWins;
						}
					}
				}
			}
			
			//checking horizontal is two or not
			if(board[j]!=' '&&board[j]==board[j+1]&&board[j-1]==' '&&board[j+2]==' '){// _OO_
				if(board[j+2]==' '&&((j<35&&board[j+9]!=' ')||j>=35)){// XOO_ 
					if(board[j]=='O'){//check if human will win or not
						humanWins=1;
						printf("14 ");//test
						board[bottom] = ' '; //復原!!
						return humanWins;
					}
				}
				
				if(board[j-1]==' '&&((j<35&&board[j+6]!=' ')||j>=35)){// _OOX
					if(board[j]=='O'){//check if human will win or not
						humanWins=1;
						printf("15 ");//test
						board[bottom] = ' '; //復原!!
						return humanWins;
					}
				}
			}
			
			if(board[j-1]!=' '&&board[j+1]==board[j-1]&&board[j-2]==' '&&board[j+2]==' '){// _O_O_
				if(board[j]==' '&&((j<35&&board[j+7]!=' ')||j>=35)){
					if(board[j]=='O'){//check if human will win or not
						humanWins=1;
						printf("16 ");//test
						board[bottom] = ' '; //復原!!
						return humanWins;
					}
				}
			}
		}
	}
	
	for(i=1;i<=5;i++){//checking tilted is two or not
		for(j=i+7;j<=(i+28);j=j+7){
			
			if(board[j]!=' '&&board[j]==board[j-6]&&board[j-12]==' '&&board[j+6]==' '){// _OO_ 正斜率
				if(i>=1&&i<=4&&j>13&&board[j-12]==' '&&board[j-5]!=' '){// XOO_ 正斜率
					if(board[j]=='O'){//check if human will win or not
						humanWins=1;
						printf("17 ");//test
						board[bottom] = ' '; //復原!!
						return humanWins;
					}
				}
				
				if(i>=2&&i<=5&&board[j+6]==' '&&((j<28&&board[j+16]!=' ')||j>=28)){// _OOX 正斜率
					if(board[j]=='O'){//check if human will win or not
						humanWins=1;
						printf("18 ");//test
						board[bottom] = ' '; //復原!!
						return humanWins;
					}
				}
			}
			
			if(board[j-6]!=' '&&board[j-6]==board[j+6]&&j>=14&&j<35&&board[j-12]==' '&&board[j+12]==' '){// _O_O_ 正斜率
				if(board[j]==' '&&board[j+7]!=' '){
					if(board[j]=='O'){//check if human will win or not
						humanWins=1;
						printf("19 ");//test
						board[bottom] = ' '; //復原!!
						return humanWins;
					}
				}
			}
			
			if(board[j]!=' '&&board[j]==board[j-8]&&board[j-16]==' '&&board[j+8]==' '){// _OO_ 負斜率
				if(j>13&&board[j-16]==' '&&board[j-9]!=' '){// _OOX 負斜率
					if(board[j]=='O'){//check if human will win or not
						humanWins=1;
						printf("20 ");//test
						board[bottom] = ' '; //復原!!
						return humanWins;
					}
				}
				
				if(j<35&&board[j+8]==' '&&((j<28&&board[j+15]!=' ')||j>=28)){// XOO_ 負斜率
					if(board[j]=='O'){//check if human will win or not
						humanWins=1;
						printf("21 ");//test
						board[bottom] = ' '; //復原!!
						return humanWins;
					}
				}
			}
			
			if(board[j-8]!=' '&&board[j-8]==board[j+8]&&j>=14&&j<35&&board[j+16]==' '&&board[j-16]==' '){// _O_O_ 負斜率
				if(board[j]==' '&&board[j+7]!=' '){
					if(board[j]=='O'){//check if human will win or not
						humanWins=1;
						printf("22 ");//test
						board[bottom] = ' '; //復原!!
						return humanWins;
					}
				}
			}
		}
	}
	
	//若人類不會贏，則傳回 0
	board[bottom] = ' '; //復原!!
	if(humanWins==0){//if human won't win
		printf("0\n");//test
		return humanWins;
	}
}
