#pragma once
#include"TcpServer.h"
#include"Draw.h"
#include<iostream>
#include<thread>
#include"InputHandle.h"
#ifndef ADMINLAYOUT_H
#define ADMINLAYOUT_H

/***************消息框按钮文字****************/
LRESULT __stdcall CBTHookProc(long nCode, WPARAM wParam, LPARAM lParam);
/***********************************/using namespace std;
class AdminLayout
{
public:
	AdminLayout();
	int exec();
	~AdminLayout();
private:
	Info info;
	int page, currPage;
	int currMusic;
	volatile bool saved;
	static const int SCORE_START = 6;
	static const int ID_START = 15;
	static const int NAME_START = 24;
	static const int SINGER_START = 50;
	static const int SINGER_END = 66;
	static const int FUNCTION_Y = 72;
	void render();
	void drawWidget();
	void printFunction();
	/*******************功能键光标移动*********************/
	static const int choiceNum = 7;
	void choiceUp(int &choice);
	void choiceDown(int &choice);
	void choiceHighlight(int choice);
	void choiceHide(int choice);
	/*****************************************/

	/*******************音乐光标移动*********************/
	void MusicUp(int &choice);
	void MusicDown(int &choice);
	void MusicHighlight(int choice);
	void MusicHide(int choice);
	void MusicJump(int index);
	/*********************绘制********************/
	//CRITICAL_SECTION g_cs;
	void printPage(int &currPage);
	void printMusics(int begin, int count);
	void printMusic(int x, int index);
	void erase(int x);
	void chooseFunction(int choice);

	/********************必要功能的实现*******************************/
	void rank();
	void pushHotMusic();
	void add();
	void modify();
	void search();
	void searchChoiceUp(int &choice);
	void searchChoiceDown(int &choice);
	void searchChoiceHighlight(int choice);
	void searchChoiceHide(int choice);
	int search_by_singer();
	int search_by_name();
	void loadMore();
	void save();
	void deleteMusic();
	void refresh();
	thread checkThread;
	void checkMusicMark();
	/************************连接用户小窗************************************/
	void detailWidgetInit();

	/***************************功能输入窗口********************************/
	static const int FunctionWidget_X = 17;
	static const int FunctionWidget_Y = 85;
	static const int FunctionWidget_WIDTH = 55;
	static const int FunctionWidget_HEIGHT = 18;
	void drawFunctionWidget();
	/****************************************************************/

	/***********************TCP传输相关*********************************/
	TcpServer tcpServer;
	thread tcpThread;
	volatile bool isRunning;
	void tcpServe();
	/************************************************************/


};

#endif // !ADMINLAYOUT_H

