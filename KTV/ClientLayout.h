#pragma once
#include"TcpClient.h"
#include<iostream>
#include<string>
#include<thread>
#include"Draw.h"
using namespace std;
#ifndef CLIENTLAYOUT_H
#define CLIENTLAYOUT_H

/***************消息框按钮文字****************/

LRESULT __stdcall CBTHookProc2(long nCode, WPARAM wParam, LPARAM lParam);
/***********************************/

class ClientLayout
{
public:
	ClientLayout();
	~ClientLayout();
	int exec();
private:
	Info playMusics;
	Info musicBank;
	int page, currPage;
	int currMusic;
	int playMusic;
	static const int SCORE_START = 86;
	static const int ID_START = 95;
	static const int NAME_START = 104;
	static const int SINGER_START = 130;
	static const int SINGER_END = 142;

	void render();
	void drawWidget();
	void printFunction();
	void clearFuntion();
	/*******************功能键光标移动*********************/
	static const int choiceNum = 2;
	void choiceUp(int &choice);
	void choiceDown(int &choice);
	void choiceHighlight(int choice);
	void choiceHide(int choice);
	/*****************************************/

	/*******************音乐光标移动*********************/
	const int musicNum = 12;
	void MusicUp(int &choice, int y = SCORE_START - 3);
	void MusicDown(int &choice, int y = SCORE_START - 3);
	void MusicHighlight(int choice, int y = SCORE_START - 3);
	void MusicHide(int choice, int y = SCORE_START - 3);
	void MusicJump(int index);
	/*******************绘制**********************/
	CRITICAL_SECTION g_cs;
	void printPage(int &currPage, Info &info, int dis = 0, int playMusic = 0);
	void printMusics(int begin, int count, Info &info, int dis = 0);
	void printMusic(int x, int index, Info &info, int dis = 0);
	static void erase(int x, int dis = 0);
	void printPlayMusic();
	void chooseFunction(int choice);

	/********************必要功能的实现*******************************/
	void hotMusics();
	const int DISTANCE = 74;
	void vod();
	void clickMusic(int index);
	int search();
	bool playEmpty;
	void nextMusic();
	void putOnTop();
	void mark();

	void refresh();
	void refresh2();

	/*****************tcpClient**********************/
	string ip;
	//CRITICAL_SECTION g_cs;
	TcpClient tcpClient;
	thread tcpThread;
	string greetWord;
	volatile bool quit;
	volatile int connected;
	void get_IP();
	void tcpRequest();
	void tcpRequestInfo();
	void tcpRequestHotMusics();
	void tcpSendScore(int ID, int score);
	/**************************************************/
};
#endif // !CLIENTLAYOUT_H

