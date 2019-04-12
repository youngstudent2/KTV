#include "TcpServer.h"
 TcpServer::TcpServer() {
	count = -1;
}

 TcpServer::~TcpServer() {
	running = false;
	//closesocket(sockSrv);//关闭	
	//WSACleanup();//释放资源的操作
}

 std::vector<std::string> TcpServer::getClientAddr() {
	std::vector<std::string> res;
	for (auto& a : addrs) {
		res.push_back(std::string(inet_ntoa(a)));
	}
	return res;
}

 int TcpServer::init() {
	int port = 5321;
	running = true;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		//printf("Failed to load Winsock");
		return 0;
	}

	//创建用于监听的套接字
	sockSrv = socket(AF_INET, SOCK_STREAM, 0);

	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(port); //1024以上的端口号
	addrSrv.sin_addr.S_un.S_addr = htonl(INADDR_ANY);

	int resVal = ::bind(sockSrv, (LPSOCKADDR)&addrSrv, sizeof(SOCKADDR_IN));
	if (resVal == SOCKET_ERROR) {
		int enable = 1;
		setsockopt(sockSrv, SOL_SOCKET, SO_REUSEADDR, (const char*)&enable, sizeof(int));
	}
	if (listen(sockSrv, 10) == SOCKET_ERROR) {
		//printf("Listen failed:%d", WSAGetLastError());
		return 0;
	}


}

 void TcpServer::tcpServerMain() {
	//开始监听	
	std::vector<std::thread> ts;

	SOCKADDR_IN addrClient;
	int len = sizeof(SOCKADDR);
	char buf[] = "Server: hello, I am a server.....";
	while (1)
	{
		if (!running)break;
		//等待客户请求到来	
		SOCKET sockConn = accept(sockSrv, (SOCKADDR *)&addrClient, &len);

		if (sockConn == SOCKET_ERROR) {

			Sleep(3000);
			continue;
		}
		bool equal = false;
		for (auto& addr : addrs) {
			//std::cerr << inet_ntoa(addr) << '\n' << inet_ntoa(addrClient.sin_addr) << '\n';
			if (0 == strcmp(inet_ntoa(addr), inet_ntoa(addrClient.sin_addr))) {
				equal = true;
				break;
			}
		}
		if (!equal) {
			addrs.push_back(addrClient.sin_addr);
			//printf("Accept client IP:[%s]\n", inet_ntoa(addrClient.sin_addr));
		}

		//发送数据
		int iSend = send(sockConn, buf, sizeof(buf), 0);
		if (iSend == SOCKET_ERROR) {
			//printf("send failed");
			break;
		}

		ts.push_back(std::thread(&TcpServer::handleClient, this, sockConn, ts.size()));
		ts.back().detach();

		Sleep(200);

	}
	ts.clear();
	closesocket(sockSrv);
	WSACleanup();
}

 void TcpServer::quit() {
	running = false;
}

 std::string TcpServer::receive(SOCKET & sockConn) {
	while (1) {
		char receiveBuf[1024];
		std::string Buff;
		int len = 1024, flag = 0;
		while (len > 0)
		{
			memset(receiveBuf, 0, len);
			recv(sockConn, receiveBuf, strlen(receiveBuf) + 1, 0);
			len = strlen(receiveBuf);
			if (len <= 0) {
				if (flag > 1024)break;
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
	}
}

 void TcpServer::handleClient(SOCKET clientSocket, int index) {
	std::string res, buff;
	while (1)
	{
		if (!running)return;
		res = receive(clientSocket);
		if (res == "hotmusic") {
			//printf("client%d say you are 666\n", index);
			std::fstream file("hotmusic.txt", std::ios::in);
			std::string sendInfo;
			char tempInfo[200];
			while (file.getline(tempInfo, 200)) {
				sendInfo.assign(tempInfo);
				sendInfo.append("#");
				send(clientSocket, sendInfo.c_str(), sendInfo.length() + 1, 0);
			}
			send(clientSocket, "EOF#", sizeof("EOF#"), 0);
			file.close();/**/
		}
		else if (res == "info") {
			//printf("client%d need info\n", index);
			std::fstream file("music.txt", std::ios::in);
			std::string sendInfo;
			char tempInfo[200];
			while (file.getline(tempInfo, 200)) {
				sendInfo.assign(tempInfo);
				sendInfo.append("#");
				send(clientSocket, sendInfo.c_str(), sendInfo.length() + 1, 0);
			}
			send(clientSocket, "EOF#", sizeof("EOF#"), 0);
			file.close();/**/
		}
		else if (res == "mark") {
			std::istringstream markInfo(receive(clientSocket));
			std::string ID, score;
			markInfo >> ID >> score;
			musicMark[++count][0] = atoi(ID.c_str());
			musicMark[count][1] = atoi(score.c_str());
		}
		else if (res == "goodbye") {
			//addrs.erase(addrs.begin() + index);
			//printf("client%d say goodbye to you\n", index);
			closesocket(clientSocket);
			return;
		}
		//return;
	}
}
