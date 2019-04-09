#pragma once
#ifndef secure
#define secure
#define  _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#endif // !secure
#include <WINSOCK2.H>
#include <WS2tcpip.h>
#include<sstream>
#include <string>
#include<vector>
#include <thread>
#include<iostream>
#include<fstream>
//#pragma comment(lib,"ws2_32.lib")
//using namespace std;

class TcpServer
{
private:
	WORD myVersionRequest;
	WSADATA wsaData;
	SOCKADDR_IN clientSocket;
	SOCKET sockSrv;
	//��Ҫ�󶨵Ĳ���
	SOCKADDR_IN addrSrv;
	volatile bool running;
	
	std::vector<IN_ADDR> addrs;
public:
	volatile int musicMark[100][2];//ͻȻ�����յ������ִ�����info���ֻ����������ֵĲ�����,555
									//musicMark[i][0]��ʾID musicMark[i][1]��ʾscore
	volatile int count;
	TcpServer();
	~TcpServer();
	std::vector<std::string> getClientAddr();
	int init();
	void tcpServerMain();
	void quit();
	std::string receive(SOCKET &sockConn);

	void handleClient(SOCKET clientSocket, int index);


};