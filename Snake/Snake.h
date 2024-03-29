/*
  Snake,a cplusplus game.
  Copyright (C) 2020-2023 IcyLightDream

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

//方向键的ASCII.
#define UP 72
#define DOWN 80
#define LEFT 75
#define RIGHT 77

//20*50的格子.
#define HIGHT 20
#define WIDTH 50
//蛇的最长长度.
const int maxSnakeTailLength = 10000;

char headC = '#', tailC = '*', fruitC = '0';

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
	system("cls");
	exit(0);
}

struct Pos {		//pos从0开始.
	short x, y;
};
struct DebugModeSettings {
	bool byIcyLightDream;	//蛇头撞蛇身是否算死亡.
	long long initTailLength;	//初始蛇长.
};

const short SPEED_MS[5] = { 0,300,125,50,25 };		//速度对应的值.
const string SPEED_STR[5] = { "","散步","行走","奔跑","?" };
const short SPEED_STR_SZ[5] = { 0,4,4,4,1 };

const string VERSION = "1.4.8", TIME = "2023.6.9";
