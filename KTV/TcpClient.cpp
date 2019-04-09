#include "TcpClient.h"

int TcpClient::init(string ip) {
	char buff[1024];
	memset(buff, 0, sizeof(buff));

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		//printf("Failed to load Winsock");
		return 0;
	}

	SOCKADDR_IN addrSrv;
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(5321);
	addrSrv.sin_addr.S_un.S_addr = inet_addr(ip.c_str());//ip

														 //创建套接字
	sockClient = socket(AF_INET, SOCK_STREAM, 0);
	if (SOCKET_ERROR == sockClient) {
		//printf("Socket() error:%d", WSAGetLastError());
		return 0;
	}

	//向服务器发出连接请求
	if (connect(sockClient, (struct  sockaddr*)&addrSrv, sizeof(addrSrv)) == INVALID_SOCKET) {
		//printf("Connect failed:%d", WSAGetLastError());
		return 0;
	}
	else
	{
		//接收数据
		recv(sockClient, greetWord, sizeof(greetWord), 0);
		//printf("%s\n", buff);
		return 1;
	}
}

string TcpClient::recvMessage() {  //若返回"EOF"表示所有信息发送完毕
	char receiveBuf[1024];
	string Buff;
	int len = 1024, flag = 0;
	while (len > 0)
	{
		memset(receiveBuf, 0, len);
		recv(sockClient, receiveBuf, strlen(receiveBuf) + 1, 0);
		len = strlen(receiveBuf);
		if (len <= 0) {
			if (flag > 1024)return "error";
			flag++;
			continue;
		}
		if (receiveBuf[len - 1] == '#')
		{
			Buff.append(receiveBuf, len - 1);
			return Buff;
		}
		else
			Buff.append(receiveBuf, len);
	}
	return "error";
}

char * TcpClient::getGreetWord() {
	return greetWord;
}

int TcpClient::request(string requestCommand) {
	requestCommand.append("#");
	send(sockClient, requestCommand.c_str(), requestCommand.length(), 0);
	return 1;
}

TcpClient::~TcpClient() {
	request("goodbye");
	closesocket(sockClient);
	WSACleanup();
}
