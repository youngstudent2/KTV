
#ifndef  DRAW_H
#define DRAW_H
#include<windows.h>
#include<iostream>
#include<vector>
#include<string>
#include"Info.h"
#include"InputHandle.h"
using namespace std;

class draw
{
public:
	static void gotoxy(int x, int y);
	static void printRow(int x, int y, int width, char ch = '*');
	static void printCol(int x, int y, int height, char ch = '*');
	static void printRect(int x, int y, int width, int height, char ch1 = '*', char ch2 = '*');
	static void drawFunction(int x, int y, const string functionName);
	static void drawTitle(int x, int y, const string title);
	static void clearWidget(int x, int y, int width, int height);
	static void printAddIntro(int x, int y);
	static void printSearchIntro(int x, int y);
	static void printDeleteIntro(int x, int y);
	static void clearRow(int x, int y, int width);
	static void cerrWidgetBottom(int x, int y, int width, int height, string errInfo);
	static void printModifyInfo(int x, int y, Music &m);
	static void printMarkInfo(int x, int y);
	static void starHightlight(int x, int y);
	static void starHide(int x, int y);
	static void printClientInfo(vector<string> adds);
};

#endif // ! DRAW_H

