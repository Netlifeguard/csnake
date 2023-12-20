#include <iostream>
#include <graphics.h>
#include <conio.h>
#include <time.h>
#include <stdlib.h>
#include <mmsystem.h>
#pragma comment(lib,"winmm.lib")
#define SNAKE_LEN 500
 struct Snake { //body
	int size;
	int speed;
	int direction;
	POINT coordinate[SNAKE_LEN];
}snake;
 struct Food
 {
	 int x;
	 int y;
	 bool flag;
	 int r;
	 DWORD color;
 }food;
 enum Dir
 {
	 UP,
	 DOWN,
	 LEFT,
	 RIGHT
 };
void gameInit();
void gameDraw();
void move();
void changeDir();
void eat();
void crush();
int score = 0;
char over[2];
char choose;
bool playAgain = true;
BOOL SetConsoleWindowSize(int columns, int rows);
int main()
{
	SetConsoleWindowSize(80, 30);
	initgraph(600, 600,SHOWCONSOLE);// -- init graph
	//IMAGE me;
	//loadimage(&me,"D:\\photo\\day life\\me.png");
	//putimage(0,0,&me);
	gameInit();
	//gameDraw();
	while (playAgain){
		gameDraw();
		move();
		eat();
		changeDir();
		Sleep(30);
	}
	closegraph();
}
void gameInit(){
	char buffer[100];
	errno_t result;
	result=mciSendString("open ./resource/toilet-soap.mp3 alias bgm",0,0,0);
	if (result == 0) {
		fprintf_s(stderr, "Error message: %s\n", buffer);
	}
	else {
		printf("Error getting error message: %d\n", result);
	}
	mciSendString("play bgm repeat",0,0,0);
	snake.size = 3;//--- init snake
	snake.direction = RIGHT;
	snake.speed = 20;
	for (int i = 0;i<snake.size; i++)
	{
		snake.coordinate[i].x = 50-20*i;
		snake.coordinate[i].y = 20;
	}
	//food data
	srand((unsigned int)time(NULL));
	food.r = 10;
	food.x = rand() % 600;
	food.y = rand() % 600;
	food.flag = true;
	food.color = RGB(rand()%256, rand() % 256, rand() % 256);
}
void gameDraw() {
	BeginBatchDraw();
	setbkcolor(RGB(226, 263, 230));
	cleardevice();
	for (int i = 0; i < snake.size; i++)
	{
		if (i==0)
		{
			setfillcolor(RED);
		}
		else {
			setfillcolor(GREEN);
		}
		solidcircle(snake.coordinate[i].x, snake.coordinate[i].y, 10);
	}
	if (food.flag)
	{
		setfillcolor(food.color);
		solidcircle(food.x,food.y,food.r);
	}
	EndBatchDraw();
}
void move() {  //move and determinate the dir
	for (int i = snake.size-1; i >0; i--)
	{
		snake.coordinate[i] = snake.coordinate[i - 1];
	}
	switch (snake.direction)
	{
	case UP:
		snake.coordinate[0].y-=snake.speed;
		if (snake.coordinate[0].y<0)
		{
			//snake.coordinate[0].y = 600;
			crush();
			snake.coordinate[0].y = 600;
		}
		break;			 
	case DOWN:			 
		snake.coordinate[0].y+= snake.speed;
		if (snake.coordinate[0].y >600)
		{
			system("pause");
			crush();
			snake.coordinate[0].y = 0;
		}
		break;			 
	case LEFT:			 
		snake.coordinate[0].x-= snake.speed;
		if (snake.coordinate[0].x< 0)
		{
			crush();
			snake.coordinate[0].x= 600;
		}
		break;			 
	case RIGHT:			 
		snake.coordinate[0].x+= snake.speed;
		if (snake.coordinate[0].x+food.r >600)
		{
			crush();
			snake.coordinate[0].x=0;
		}
		break;
	}
}
void changeDir() {
	if (_kbhit())
		switch (_getch())
		{
		case 72:
			if (snake.direction!=DOWN)
			{
				snake.direction = UP;
			}
			break;
		case 75:
			if (snake.direction!=RIGHT)
			{
				snake.direction = LEFT;
			}
			break;
		case 77:
			if (snake.direction!=LEFT)
			{
				snake.direction = RIGHT;
			}
			break;
		case 80:
			if (snake.direction!=UP)
			{
				snake.direction = DOWN;
			}
			break;
		case ' ':
			while (1)
			{
				if (_getch() == ' ') {
					return;
				}
			}
			break;
		}
	}
void eat() {
	if (food.flag&&snake.coordinate[0].x>=food.x-food.r&& snake.coordinate[0].x <=food.x+food.r&&
		snake.coordinate[0].y>=food.y-food.r&& snake.coordinate[0].y<=food.y+food.r
		)
	{
		food.flag = false;
		snake.size++;
		score += 10;
	}
	if (!food.flag)
	{
		food.r = 10;
		food.x = rand() % 600;
		food.y = rand() % 600;
		food.flag = true;
		food.color = RGB(rand() % 256, rand() % 256, rand() % 256);
	}
	
}
void crush() {
	printf("your score :%d\n",score);
	printf("游戏结束，是否要重新开始游戏[Y/N]:");
	if (fgets(over, sizeof(over), stdin) != NULL)
	{
		choose = over[0];
		if (choose == 'n' || choose == 'N')
		{
			playAgain = false;
			printf("%c--", choose);
		}
		else
		{
			getchar();
			main();
		}
	}
	getchar();
}
BOOL SetConsoleWindowSize(int columns, int rows) {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SMALL_RECT rect = { 0, 0, static_cast<SHORT>(columns - 1), static_cast<SHORT>(rows - 1) };
	return SetConsoleWindowInfo(hConsole, TRUE, &rect);
}