/*
  Snake,a cplusplus game.
  Copyright (C) 2022  2345Explorer

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/
#include <iostream>
#include <string>
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <time.h>
#include "printFrame.h"
#include "Snake.h"
using std::cin;
using std::cout;
using std::endl;
using std::string;
//定义.
void initFruit();
void initHead(short ms);
void printMap();
void printTips();
bool move();
void eatFruit();
bool getInput(bool& pausing);
bool game(int speed);
void startMenu();
void debugSettings();
void testSettings();
short startGameMenu();
void settings();
void about();
Pos head, tail[maxSnakeTailLength], fruit;
DebugModeSettings debugModeSetting;
TestSettings testSetting;
int snakeLength = 0;
const short dx[4] = { -1,1,0,0 }, dy[4] = { 0,0,-1,1 };
const short rdx[4] = { 1,-1,0,0 }, rdy[4] = { 0,0,1,-1 };
int d;		//0前1后2左3右

int main(int argc, char* argv[], char** env) {
	srand((unsigned)time(NULL));
	system("title 贪吃蛇");
	hideCursor();

	debugModeSetting.HBT = true;
	debugModeSetting.initTailLength = 0;
	snakeLength = 0;
	startMenu();
	return 0;
}

void initFruit() {		//初始化水果.
	do {
		fruit.x = rand() % HIGHT, fruit.y = rand() % WIDTH;
		for (int i = 0;i < snakeLength;i++)
			if (fruit.x == tail[i].x && fruit.y == tail[i].y) {
				fruit.x = 0;
				break;
			}
	} while (fruit.x == 0 || fruit.y == 0 || fruit.x == HIGHT - 1 || fruit.y == WIDTH - 1 || (fruit.x == head.x && fruit.y == head.y));
}
void initHead(short ms) {		//初始化蛇头.
	snakeLength = 0;
	int a[4] = { 2,4,6,8 };
	int x = 0;
	if (ms == SPEED_MS[1])	x = 0;
	else if (ms == SPEED_MS[2])	x = 1;
	else if (ms == SPEED_MS[3])	x = 2;
	else if (ms == SPEED_MS[4])	x = 3;
	do {
		head.x = rand() % HIGHT, head.y = rand() % WIDTH;
	} while (head.x <= a[x] || head.y <= a[x] || head.x >= HIGHT - a[x] || head.y >= WIDTH - a[x]);
}

void printMap() {		//输出地图.
	bool mp[HIGHT + 2][WIDTH + 2];
	memset(mp, 0, sizeof(mp));
	mp[head.x][head.y] = 1;
	for (int i = 0; i < snakeLength; i++) {
		mp[tail[i].x][tail[i].y] = 1;
	}
	for (int i = 0; i < HIGHT; i++) {
		for (int j = 0; j < WIDTH; j++) {
			if (i == fruit.x && j == fruit.y) {
				gotoXY(j, i);
				cout << FruitC;
			}
			else if (mp[i][j]) {
				gotoXY(j, i);
				if (i == head.x && j == head.y)
					cout << HeadC;
				else
					cout << TailC;
			}
			else if (i == 0 || j == 0 || i == HIGHT - 1 || j == WIDTH - 1) {
				gotoXY(j, i);
				if (i == 0 || i == HIGHT - 1) {
					cout << "-";
				}
				else {
					cout << "|";
				}
			}
			else {
				gotoXY(j, i);
				cout << " ";
			}
		}
	}
}
void printTips() {
	gotoXY(WIDTH + 2, 10);
	cout << "当前分数:" << snakeLength << "         ";
	gotoXY(WIDTH + 2, 12);
	cout << "用方向键操控贪吃蛇,ESC暂停           ";
}

bool move() {		//是否可以移动,如果可以就移动.
	int thx = head.x + dx[d], thy = head.y + dy[d];
	if (debugModeSetting.HBT)
		for (int i = 0; i < snakeLength; i++)
			if (thx == tail[i].x && thy == tail[i].y)
				return false;
	int lx = head.x, ly = head.y;
	head.x += dx[d], head.y += dy[d];
	for (int i = 0; i < snakeLength; i++) {
		int tx = tail[i].x, ty = tail[i].y;
		tail[i].x = lx, tail[i].y = ly;
		lx = tx, ly = ty;
	}
	return true;
}

void eatFruit() {		//吃到水果.
	tail[snakeLength].x = tail[snakeLength - 1].x + rdx[d];
	tail[snakeLength].y = tail[snakeLength - 1].y + rdy[d];
	snakeLength++;
	initFruit();
}

bool getInput(bool& pausing) {		//获取键盘输入.
	char t = _getch();
	if (t != 0 && t != 224 && t != -32) {
		if (t == 0x1b)
			pausing = true;
		return true;
	}
	char c = _getch();		//第二次读取,为什么网上有.
	if (c == UP) {
		if (d != 1) {
			d = 0;
			if (!move())
				return false;
		}
	}
	else if (c == DOWN) {
		if (d != 0) {
			d = 1;
			if (!move())
				return false;
		}
	}
	else if (c == LEFT) {
		if (d != 3) {
			d = 2;
			if (!move())
				return false;
		}
	}
	else if (c == RIGHT) {
		if (d != 2) {
			d = 3;
			if (!move())
				return false;
		}
	}
	return true;
}

bool game(int speed) {		//游戏.
	hideCursor();
	gotoXY(WIDTH, 0);
	cout << "       ";
	d = rand() % 4;	//方向.
	initHead(SPEED_MS[speed]);
	initFruit();
	bool pausing = false;
	for (int i = 1;i <= debugModeSetting.initTailLength;i++)	eatFruit();
	while (head.x > 0 && head.y > 0 && head.x < HIGHT - 1 && head.y < WIDTH - 1) {
		if (pausing) {
			int a[3][3] = { 10,WIDTH + 2,8,12,WIDTH + 2,12,14,WIDTH + 2,8 }, i = 0;
			bool flag = false;
			while (1) {
				if (!flag) {
					system("cls");
					printMap();
					gotoXY(WIDTH + 2, 10);
					cout << "返回游戏                 ";
					gotoXY(WIDTH + 2, 12);
					cout << "退出至主菜单             ";
					gotoXY(WIDTH + 2, 14);
					cout << "退出游戏                 ";
					printFrame(a[i][0], a[i][1], a[i][2], 1);
					gotoXY(WIDTH + 2, 16);
					cout << "上、下方向键切换, 回车选择";
				}
				else {
					flag = false;
				}
				char t = _getch();
				if (t == 0 || t == 224 || t == -32) {
					char c = _getch();
					if (c == UP) {
						if (i > 0) i--;
						else	i = 2;
					}
					else if (c == DOWN) {
						if (i < 2)	i++;
						else	i = 0;
					}
					else {
						flag = true;
					}
				}
				else if (t == 13) {	//回车.
					if (i == 0) {
						break;
					}
					else if (i == 1) {
						return true;
					}
					else if (i == 2) {
						exitGame();
					}
				}
			}
			pausing = false;
			system("cls");
			printMap();
			printTips();
		}
		if (head.x == fruit.x && head.y == fruit.y) {
			eatFruit();
		}
		if (_kbhit()) {
			if (!getInput(pausing))
				break;
		}
		else {
			if (!move())
				break;
		}
		Sleep(SPEED_MS[speed]);
		printMap();
		printTips();
	}
	printMap();
	printTips();
	//游戏结束.
	gotoXY(0, HIGHT + 1);
	cout << "游戏结束!\n";
	system("pause");
	system("cls");
	cout << "游戏结束!\n";
	cout << "分数:" << snakeLength + 1 << endl;
	cout << "速度:" << SPEED_STR[speed] << endl;
	Sleep(500);
	if (testSetting.sd) {
		cout << "游戏玩完了 关机中...";
		system("shutdown /s /t 100");
		Sleep(1000);
		exitGame();
	}
	cout << "Y:再来一次 N:结束\n";
	char c = 32;
	while (c != 'Y' && c != 'y' && c != 'N' && c != 'n') {
		c = _getch();
	}
	if (c == 'N' || c == 'n')	exitGame();
	return true;
}

short startGameMenu() {
	system("cls");
	int f = 0, speed, flag = false;
	int a[6][2] = { 2,2,8,2,14,2,20,2,26,2,32,8 };
	while (1) {
		if (!flag) {
			system("cls");
			cout << "**********贪吃蛇**********\n";
			cout << "请选择速度\n\n";
			cout << ' ' << SPEED_STR[1] << "  " << SPEED_STR[2]
				<< "  " << SPEED_STR[3] << "  " << SPEED_STR[4]
				<< "  随机  返回主菜单\n\n";
			cout << "左、右方向键切换,回车选择\n";
			printFrame(3, a[f][0], a[f][1] + 1, 1);
		} else {
			flag = false;
		}
		char c1 = _getch();
		if (c1 != 0 && c1 != 224 && c1 != -32) {
			if (c1 == 13) {
				if (f == 4) {
					speed = rand() % 4 + 1;	//散步:1 行走:2 奔跑:3 疯狂:4 
				} else if (f == 5) {
					return 0;
				} else {
					speed = f + 1;
				}
				break;
			}
			else {
				flag = true;
			}
		}
		else {
			char c2 = _getch();
			if (c2 == LEFT) {
				if (f == 0)	f = 5;
				else	f--;
			}
			else if (c2 == RIGHT) {
				if (f == 5)	f = 0;
				else	f++;
			}
			else {
				flag = true;
			}
		}
	}
	system("cls");
	if (f == 4)
		cout << "已选速度:" << SPEED_STR[speed] << endl;
	cout << "加载中...\n";
	Sleep(500);
	if (game(speed))	return 0;
	return speed;
}

void testSettings() {
	int f = 0;
	int a[2][3] = { 2,24,2,4,24,13 };
	bool flag = false;
	while (1) {
		if (!flag) {
			system("cls");
			cout << "********实验性********\n\n";
			cout << "玩完游戏自动关机:";
			if (testSetting.sd)	cout << "是";
			else	cout << "否";
			cout << "    更改\n\n";
			gotoXY(23, 4);
			cout << "返回到Debug设置\n\n";
		}
		else flag = false;
		printFrame(a[f][0], a[f][1], a[f][2] + 1, 1);
		char c1 = _getch();
		if (c1 != 0 && c1 != 224 && c1 != -32) {
			if (c1 == 13) {		//回车.
				system("cls");
				if (f == 0) {
					if (testSetting.sd) {
						testSetting.sd = false;
						cout << "玩完游戏自动关机已更改为:否\n";
						Sleep(700);
						continue;
					}
					cout << "确定嘛?(Y/N)";
					char c2 = _getche();
					while (c2 != 'Y' && c2 != 'N' && c2 != 'y' && c2 != 'n')	c2 = _getch();
					if (c2 == 'N' || c2 == 'n')	continue;
					testSetting.sd = true;
					cout << "\n危\n";
					Sleep(700);
				}
				else {
					break;
				}
			}
			else {
				flag = true;
				continue;
			}
		}
		else {
			char c2 = _getch();
			if (c2 == UP) {
				if (f == 0)	f = 1;
				else	f--;
			}
			else if (c2 == DOWN) {
				if (f == 1)	f = 0;
				else	f++;
			}
			else {
				flag = true;
				continue;
			}
		}
	}
}
void debugSettings() {
	int f = 0;
	int a[4][3] = { 2,22,2,4,22,10,6,22,4,8,22,2 };
	bool flag = false;
	while (1) {
		if (!flag) {
			system("cls");
			cout << "********Debug设置********\n\n";
			cout << "蛇头撞蛇身死亡:";
			if (debugModeSetting.HBT)	cout << "是";
			else	cout << "否";
			cout << "    更改  \n\n";
			cout << "当前初始长度为:" << debugModeSetting.initTailLength + 1;
			gotoXY(21, 4);
			cout << "更改初始长度  \n\n";
			cout << "实验性";
			gotoXY(21, 6);
			cout << "设置\n\n";
			gotoXY(21, 8);
			cout << "退出\n\n";
			cout << "上、下方向键选择, 回车确认 \n";
		}
		else flag = false;
		printFrame(a[f][0], a[f][1], a[f][2] + 1, 1);
		char c1 = _getch();
		if (c1 != 0 && c1 != 224 && c1 != -32) {
			if (c1 == 13) {		//回车.
				system("cls");
				if (f == 0) {
					debugModeSetting.HBT = !debugModeSetting.HBT;
					cout << "蛇头撞蛇身死亡已更改为";
					if (debugModeSetting.HBT == true)	cout << "是\n";
					else	cout << "否\n";
					cout << "即将回到Debug设置界面...\n";
					Sleep(700);
				}
				else if (f == 1) {
					cout << "请输入要更改的初始长度:";
					long long t;
					cin >> t;
					t--;
					if (t < 0)	t = -t;
					t %= maxSnakeTailLength;
					debugModeSetting.initTailLength = t;
					cout << "\n更改成功!即将回到Debug设置界面...";
					Sleep(700);
				}
				else if (f == 2) {
					testSettings();
				}
				else {
					break;
				}
			}
			else {
				flag = true;
				continue;
			}
		}
		else {
			char c2 = _getch();
			if (c2 == UP) {
				if (f == 0)	f = 3;
				else	f--;
			}
			else if (c2 == DOWN) {
				if (f == 3)	f = 0;
				else	f++;
			}
			else {
				flag = true;
				continue;
			}
		}
	}
}

void settings() {
	int fx = 0, fy = 0;
	int a[4][2][3] = { {{2,12,2},{2,20,2}},
					{{4,12,2},{4,20,2}},
					{{6,12,2},{6,20,2}},
					{{8,8,8},{8,20,6}} };
	bool flag = false;
	while (1) {
		if (!flag) {
			system("cls");
			cout << "**********设置***********\n\n";
			cout << " 蛇头:" << HeadC << "    重置    更改 \n\n";
			cout << " 蛇身:" << TailC << "    重置    更改 \n\n";
			cout << " 食物:" << FruitC << "    重置    更改 \n\n";
			cout << "        Debug模式  退出设置 \n\n";
			cout << "上下左右方向键选择, 回车确认\n";
		}
		else	flag = false;
		printFrame(a[fx][fy][0], a[fx][fy][1], a[fx][fy][2] + 1, 1);
		char c1 = _getch();
		if (c1 != 0 && c1 != 224 && c1 != -32) {
			if (c1 == 13) {		//回车.
				if (fy == 0 && fx < 3) {
					system("cls");
					if (fx == 0) {
						HeadC = '#';
						cout << "蛇头符号已重置为#\n";
						cout << "即将回到设置界面...";
						Sleep(700);
					}
					else if (fx == 1) {
						TailC = '*';
						cout << "蛇身符号已重置为*\n";
						cout << "即将回到设置界面...";
						Sleep(700);
					}
					else {
						FruitC = '0';
						cout << "食物符号已重置为0\n";
						cout << "即将回到设置界面...";
						Sleep(700);
					}
				}
				else if (fy == 1) {
					system("cls");
					if (fx == 0) {
						cout << "请输入要更改的蛇头符号:";
						HeadC = _getche();
						cout << "\n更改成功!即将回到设置界面...";
						Sleep(700);
					}
					else if (fx == 1) {
						cout << "请输入要更改的蛇身符号:";
						TailC = _getche();
						cout << "\n更改成功!即将回到设置界面...";
						Sleep(700);
					}
					else if (fx == 2) {
						cout << "请输入要更改的食物符号:";
						FruitC = _getche();
						cout << "\n更改成功!即将回到设置界面...";
						Sleep(700);
					}
					else if (fx == 3)	break;
				}
				else if (fx == 3 && fy == 0) {
					debugSettings();
				}
			}
			else {
				flag = true;
				continue;
			}
		}
		else {
			char c2 = _getch();
			if (c2 == UP) {
				if (fx == 0)	fx = 3;
				else	fx--;
			}
			else if (c2 == DOWN) {
				if (fx == 3)	fx = 0;
				else	fx++;
			}
			else if (c2 == LEFT) {
				if (fy == 0)	fy = 1;
				else	fy--;
			}
			else if (c2 == RIGHT) {
				if (fy == 1)	fy = 0;
				else	fy++;
			}
			else {
				flag = true;
				continue;
			}
		}
	}
}
void about() {
	system("cls");
	cout << "贪吃蛇 " << VERSION << " by 2345Explorer\n";
	cout << "更新时间: " << TIME << endl;
	cout << "点个Star⑧ QAQ\n仓库地址:https://gitee.com/Explorer2345/snake\n";
	cout << "\n好像用 Windows控制台主机 打开有问题, 建议 Windows10 以上使用 Windows终端\n\n";
	system("pause");
}
void startMenu() {		//初始菜单.
	int f = 0;
	bool flag = false;
	int a[4][2] = { 2,2,11,2,20,2,29,6 };
	while (1) {
		if (!flag) {
			system("cls");
			cout << "**********贪吃蛇**********\n\n";
			cout << " 开始     设置     关于     退出游戏  \n\n";
			cout << "左、右方向键切换,回车选择\n";
		}
		else	flag = false;
		printFrame(2, a[f][0], a[f][1] + 1, 1);
		gotoXY(0, 4);
		char c1 = _getch();
		if (c1 != 0 && c1 != 224 && c1 != -32) {
			if (c1 == 13) {		//回车.
				if (f == 0) {
					short t = startGameMenu();
					if (t != 0)	break;
					else	continue;
				}
				else if (f == 1)
					settings();
				else if (f == 2)
					about();
				else if (f == 3)
					exit(0);
			}
			else {
				flag = true;
				continue;
			}
		}
		else {
			char c2 = _getch();
			if (c2 == LEFT) {
				if (f == 0)	f = 3;
				else	f--;
			}
			else if (c2 == RIGHT) {
				if (f == 3)	f = 0;
				else	f++;
			}
			else {
				flag = true;
				continue;
			}
		}
	}
	return ;
}
