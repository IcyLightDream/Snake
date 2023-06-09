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
#include "Snake.h"
using std::cin;
using std::cout;
using std::endl;
using std::string;
void printFrame(int x,int y,int xLen,int yLen,bool clear) {
	gotoXY(y - 1,x - 1);
	for(int i = 0; i <= xLen + 1; i++) {
		if(clear)	cout << ' ';
		else	cout << '-';
	}
	for(int i = 1; i <= yLen; i++) {
		gotoXY(y - 1,x - 1 + i);
		if(clear)	cout << ' ';
		else	cout << '|';
		gotoXY(y + xLen,x - 1 + i);
		if(clear)	cout << ' ';
		else	cout << '|';
	}
	gotoXY(y - 1,x + yLen);
	for(int i = 0; i <= xLen + 1; i++) {
		if(clear)	cout << ' ';
		else	cout << '-';
	}
	gotoXY(y - 1,x - 1);
}