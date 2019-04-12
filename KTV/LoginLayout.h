#pragma once
#include<windows.h>
#include<conio.h>
#include<iostream>
#include"Draw.h"
#include"InputHandle.h"
#ifndef LAYOUT1_H
#define LAYOUT1_H

using namespace std;
class LoginLayout
{
public:
	int exec();
private:
	int chooseFuntion(int choice);
	void choiceUp(int &choice);
	void choiceDown(int &choice);
	void choiceHighlight(int choice);
	void choiceHide(int choice);
	boolean adminLayout();
	boolean userLayout();
	void render();
	void changeKey();
	bool checkNewKey(string newKey);
};
#endif // !LAYOUT1_H

