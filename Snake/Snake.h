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
#pragma once
#include <iostream>
#include <string>
#include <windows.h>
using std::cin;
using std::cout;
using std::endl;
using std::string;

//�������ASCII.
#define UP 72
#define DOWN 80
#define LEFT 75
#define RIGHT 77

//20*50�ĸ���.
#define HIGHT 20
#define WIDTH 50
//�ߵ������.
const int maxSnakeTailLength = 10000;

char HeadC = '#', TailC = '*', FruitC = '0';

void gotoXY(short x, short y) {
	COORD pos = { x, y };
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hOut, pos);
}

void hideCursor() {
	CONSOLE_CURSOR_INFO cursor_info = { 1, 0 };
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);
}

void exitGame() {
	gotoXY(0, 0);
	system("cls");
	system("pause");
	exit(0);
}

struct Pos {		//pos��0��ʼ.
	short x, y;
};
struct DebugModeSettings {
	bool by2345Explorer;	//��ͷײ�����Ƿ�������.
	long long initTailLength;	//��ʼ�߳�.
};
struct TestSettings {
	bool sd;	//��Ϸ�����ػ�.
};

const short SPEED_MS[5] = { 0,300,125,50,25 };		//�ٶȶ�Ӧ��ֵ.
const string SPEED_STR[5] = { "","ɢ��","����","����","���" };	//������������.

const string VERSION = "1.4.7", TIME = "2022.7.26";
