#pragma once
#ifndef secure
#define secure
#define  _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#endif // !secure


#include <WINSOCK2.H>
#include <WS2tcpip.h>
#include <string>
//#pragma comment(lib,"ws2_32.lib")
using namespace std;

class TcpClient
{
private:
	WORD versionRequired;
	WSADATA wsaData;
	SOCKET sockClient;
	SOCKADDR_IN clientsock_in;
	char greetWord[100];
public:
	int init(string ip);
	string recvMessage();
	char* getGreetWord();

	int request(string requestCommand);

	~TcpClient();
};