using namespace std;
#ifndef INPUTHANDLE_H
#define INPUTHANDLE_H
#include<iostream>
#include"Draw.h"
#include<conio.h>
class InputHandle
{
public:
	static const int ESC = -1;
	static const int KEY_UP = 0;
	static const int KEY_DOWN = 1;
	static const int KEY_LEFT = 2;
	static const int KEY_RIGHT = 3;
	static const int KEY_ENTER = 4;
	static const int KEY_W = 5;
	static const int KEY_S = 6;
	static const int KEY_E = 7;
	static const int KEY_Q = 8;
	static const int CTRL_Z = 9;//³·Ïú
	static const int CTRL_T = 10;//ÖÃ¶¥
	static const int CTRL_D = 11;//É¾³ý
	static const int CTRL_N = 12;//ÇÐ¸è
	static const int CTRL_F = 13;//²éÕÒ
	static const int CTRL_S = 14;//±£´æ
	static int getKeyInput();
	static string getString(int x, int y);
};
#endif // !INPUTHANDLE_H


