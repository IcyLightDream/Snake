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
//����.
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
int d;		//0ǰ1��2��3��

int main(int argc, char* argv[], char** env) {
	srand((unsigned)time(NULL));
	system("title ̰����");
	hideCursor();

	debugModeSetting.by2345Explorer = true;
	debugModeSetting.initTailLength = 0;
	snakeLength = 0;
	startMenu();
	return 0;
}

  //---------- Game Begin ----------\\

void initFruit() {		//��ʼ��ˮ��.
	do {
		fruit.x = rand() % HIGHT, fruit.y = rand() % WIDTH;
		for (int i = 0;i < snakeLength;i++)
			if (fruit.x == tail[i].x && fruit.y == tail[i].y) {
				fruit.x = 0;
				break;
			}
	} while (fruit.x == 0 || fruit.y == 0 || fruit.x == HIGHT - 1 || fruit.y == WIDTH - 1 || (fruit.x == head.x && fruit.y == head.y));
}
void initHead(short ms) {		//��ʼ����ͷ.
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

void printMap() {		//�����ͼ.
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
	cout << "��ǰ����:" << snakeLength << "         ";
	gotoXY(WIDTH + 2, 12);
	cout << "�÷�����ٿ�̰����,ESC��ͣ           ";
}

bool move() {		//�Ƿ�����ƶ�,������Ծ��ƶ�.
	int thx = head.x + dx[d], thy = head.y + dy[d];
	if (debugModeSetting.by2345Explorer)
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

void eatFruit() {		//�Ե�ˮ��.
	tail[snakeLength].x = tail[snakeLength - 1].x + rdx[d];
	tail[snakeLength].y = tail[snakeLength - 1].y + rdy[d];
	snakeLength++;
	initFruit();
}

bool getInput(bool& pausing) {		//��ȡ��������.
	char t = _getch();
	if (t != 0 && t != 224 && t != -32) {
		if (t == 0x1b)
			pausing = true;
		return true;
	}
	char c = _getch();		//�ڶ��ζ�ȡ,Ϊʲô������.
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

bool game(int speed) {		//��Ϸ.
	hideCursor();
	gotoXY(WIDTH, 0);
	cout << "       ";
	d = rand() % 4;	//����.
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
					cout << "������Ϸ                 ";
					gotoXY(WIDTH + 2, 12);
					cout << "�˳������˵�             ";
					gotoXY(WIDTH + 2, 14);
					cout << "�˳���Ϸ                 ";
					printFrame(a[i][0], a[i][1], a[i][2], 1);
					gotoXY(WIDTH + 2, 16);
					cout << "�ϡ��·�����л�, �س�ѡ��";
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
				else if (t == 13) {	//�س�.
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
	//��Ϸ����.
	gotoXY(0, HIGHT + 1);
	cout << "��Ϸ����!\n";
	system("pause");
	system("cls");
	cout << "��Ϸ����!\n";
	cout << "����:" << snakeLength + 1 << endl;
	cout << "�ٶ�:" << SPEED_STR[speed] << endl;
	Sleep(500);
	if (testSetting.sd) {
		cout << "��Ϸ������ �ػ���...";
		system("shutdown /s /t 100");
		Sleep(1000);
		exitGame();
	}
	cout << "Y:����һ�� N:����\n";
	char c = 32;
	while (c != 'Y' && c != 'y' && c != 'N' && c != 'n') {
		c = _getch();
	}
	if (c == 'N' || c == 'n')	exitGame();
	return true;
}

//\\---------- Game End ----------//

  //---------- Menu Begin ----------\\

short startGameMenu() {
	system("cls");
	int f = 0, speed, flag = false;
	int a[6][2] = { 2,2,8,2,14,2,20,2,26,2,32,8 };
	while (1) {
		if (!flag) {
			system("cls");
			cout << "**********̰����**********\n";
			cout << "��ѡ���ٶ�\n\n";
			cout << ' ' << SPEED_STR[1] << "  " << SPEED_STR[2]
				<< "  " << SPEED_STR[3] << "  " << SPEED_STR[4]
				<< "  ���  �������˵�\n\n";
			cout << "���ҷ�����л�,�س�ѡ��\n";
			printFrame(3, a[f][0], a[f][1] + 1, 1);
		} else {
			flag = false;
		}
		char c1 = _getch();
		if (c1 != 0 && c1 != 224 && c1 != -32) {
			if (c1 == 13) {
				if (f == 4) {
					speed = rand() % 4 + 1;	//ɢ��:1 ����:2 ����:3 ���:4 
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
		cout << "��ѡ�ٶ�:" << SPEED_STR[speed] << endl;
	cout << "������...\n";
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
			cout << "********ʵ����********\n\n";
			cout << "������Ϸ�Զ��ػ�:";
			if (testSetting.sd)	cout << "��";
			else	cout << "��";
			cout << "    ����\n\n";
			gotoXY(23, 4);
			cout << "���ص�Debug����\n\n";
		}
		else flag = false;
		printFrame(a[f][0], a[f][1], a[f][2] + 1, 1);
		char c1 = _getch();
		if (c1 != 0 && c1 != 224 && c1 != -32) {
			if (c1 == 13) {		//�س�.
				system("cls");
				if (f == 0) {
					if (testSetting.sd) {
						testSetting.sd = false;
						cout << "������Ϸ�Զ��ػ��Ѹ���Ϊ:��\n";
						Sleep(700);
						continue;
					}
					cout << "ȷ����?(Y/N)";
					char c2 = _getche();
					while (c2 != 'Y' && c2 != 'N' && c2 != 'y' && c2 != 'n')	c2 = _getch();
					if (c2 == 'N' || c2 == 'n')	continue;
					testSetting.sd = true;
					cout << "\nΣ\n";
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
			cout << "********Debug����********\n\n";
			cout << "��ͷײ��������:";
			if (debugModeSetting.by2345Explorer)	cout << "��";
			else	cout << "��";
			cout << "    ����  \n\n";
			cout << "��ǰ��ʼ����Ϊ:" << debugModeSetting.initTailLength + 1;
			gotoXY(21, 4);
			cout << "���ĳ�ʼ����  \n\n";
			cout << "ʵ����";
			gotoXY(21, 6);
			cout << "����\n\n";
			gotoXY(21, 8);
			cout << "�˳�\n\n";
			cout << "�ϡ��·����ѡ��, �س�ȷ�� \n";
		}
		else flag = false;
		printFrame(a[f][0], a[f][1], a[f][2] + 1, 1);
		char c1 = _getch();
		if (c1 != 0 && c1 != 224 && c1 != -32) {
			if (c1 == 13) {		//�س�.
				system("cls");
				if (f == 0) {
					debugModeSetting.by2345Explorer = !debugModeSetting.by2345Explorer;
					cout << "��ͷײ���������Ѹ���Ϊ";
					if (debugModeSetting.by2345Explorer == true)	cout << "��\n";
					else	cout << "��\n";
					cout << "�����ص�Debug���ý���...\n";
					Sleep(700);
				}
				else if (f == 1) {
					cout << "������Ҫ���ĵĳ�ʼ����:";
					long long t;
					cin >> t;
					t--;
					if (t < 0)	t = -t;
					t %= maxSnakeTailLength;
					debugModeSetting.initTailLength = t;
					cout << "\n���ĳɹ�!�����ص�Debug���ý���...";
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
			cout << "**********����***********\n\n";
			cout << " ��ͷ:" << HeadC << "    ����    ���� \n\n";
			cout << " ����:" << TailC << "    ����    ���� \n\n";
			cout << " ʳ��:" << FruitC << "    ����    ���� \n\n";
			cout << "        Debugģʽ  �˳����� \n\n";
			cout << "�������ҷ����ѡ��, �س�ȷ��\n";
		}
		else	flag = false;
		printFrame(a[fx][fy][0], a[fx][fy][1], a[fx][fy][2] + 1, 1);
		char c1 = _getch();
		if (c1 != 0 && c1 != 224 && c1 != -32) {
			if (c1 == 13) {		//�س�.
				if (fy == 0 && fx < 3) {
					system("cls");
					if (fx == 0) {
						HeadC = '#';
						cout << "��ͷ����������Ϊ#\n";
						cout << "�����ص����ý���...";
						Sleep(700);
					}
					else if (fx == 1) {
						TailC = '*';
						cout << "�������������Ϊ*\n";
						cout << "�����ص����ý���...";
						Sleep(700);
					}
					else {
						FruitC = '0';
						cout << "ʳ�����������Ϊ0\n";
						cout << "�����ص����ý���...";
						Sleep(700);
					}
				}
				else if (fy == 1) {
					system("cls");
					if (fx == 0) {
						cout << "������Ҫ���ĵ���ͷ����:";
						HeadC = _getche();
						cout << "\n���ĳɹ�!�����ص����ý���...";
						Sleep(700);
					}
					else if (fx == 1) {
						cout << "������Ҫ���ĵ��������:";
						TailC = _getche();
						cout << "\n���ĳɹ�!�����ص����ý���...";
						Sleep(700);
					}
					else if (fx == 2) {
						cout << "������Ҫ���ĵ�ʳ�����:";
						FruitC = _getche();
						cout << "\n���ĳɹ�!�����ص����ý���...";
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
	cout << "̰���� " << VERSION << " by 2345Explorer\n";
	cout << "����ʱ��: " << TIME << endl;
	cout << "���Star�� QAQ\n�ֿ��ַ:https://github.com/2345Explorer/Snake\n";
	cout << "\n������ Windows����̨���� ��������, ���� Windows10 ����ʹ�� Windows�ն�\n\n";
	system("pause");
}
void startMenu() {		//��ʼ�˵�.
	int f = 0;
	bool flag = false;
	int a[4][2] = { 2,2,11,2,20,2,29,6 };
	while (1) {
		if (!flag) {
			system("cls");
			cout << "**********̰����**********\n\n";
			cout << " ��ʼ     ����     ����     �˳���Ϸ  \n\n";
			cout << "���ҷ�����л�,�س�ѡ��\n";
		}
		else	flag = false;
		printFrame(2, a[f][0], a[f][1] + 1, 1);
		gotoXY(0, 4);
		char c1 = _getch();
		if (c1 != 0 && c1 != 224 && c1 != -32) {
			if (c1 == 13) {		//�س�.
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

//\\---------- Menu End ----------//
