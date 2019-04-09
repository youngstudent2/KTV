#include "ClientLayout.h"
HHOOK hHook2;
LRESULT __stdcall CBTHookProc2(long nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode == HCBT_ACTIVATE)
	{
		SetDlgItemText((HWND)wParam, IDYES, "按歌手查找");
		SetDlgItemText((HWND)wParam, IDNO, "按歌曲查找");
		UnhookWindowsHookEx(hHook2);
	}
	return 0;
}

 ClientLayout::ClientLayout() {
	connected = 0;
	InitializeCriticalSection(&g_cs);
	playMusic = 0;
	//playMusics.loadFile("music.txt");
	if (playMusics.musics_count() > 0)playEmpty = false;
	//musicBank.loadFile("music.txt");
	tcpThread = thread(&ClientLayout::tcpRequest, this);

}

 ClientLayout::~ClientLayout() {
	tcpClient.~TcpClient();
	Sleep(200);//必须等待thread结束后再析构
	tcpThread.~thread();
}

 int ClientLayout::exec() {
	//tcpRequest();
	get_IP();
	tcpThread.detach();

	render();

	currPage = 0;
	printPage(currPage, playMusics, 0, playMusic + 1);
	int choice = 0;
	currMusic = 0;
	MusicHighlight(currMusic);
	choiceHighlight(choice);
	while (1) {
		switch (InputHandle::getKeyInput())
		{
		case InputHandle::KEY_UP:
		case InputHandle::KEY_LEFT:
			choiceUp(choice);
			break;
		case InputHandle::KEY_DOWN:
		case InputHandle::KEY_RIGHT:
			choiceDown(choice);
			break;
		case InputHandle::KEY_ENTER:
			chooseFunction(choice);
			break;
		case InputHandle::KEY_Q://上一页
			printPage(--currPage, playMusics, 0, playMusic + 1);

			break;
		case InputHandle::KEY_E://下一页
			printPage(++currPage, playMusics, 0, playMusic + 1);

			break;
		case InputHandle::KEY_W:
			MusicUp(currMusic);

			break;
		case InputHandle::KEY_S:
			MusicDown(currMusic);
			break;
		case InputHandle::CTRL_N://切歌
			nextMusic();
			break;
		case InputHandle::ESC://界面返回
			quit = true;
			return -1;
			break;
		case InputHandle::CTRL_Z://撤销
			mark();
			break;
		case InputHandle::CTRL_D://删除
			playMusics.eraseMusic(currMusic + currPage * musicNum + playMusic + 1);
			printPage(currPage, playMusics, 0, playMusic + 1);
			break;
		case InputHandle::CTRL_T://置顶
			putOnTop();
			break;
		default:
			break;
		}
	}
	return 1;
}

 void ClientLayout::render() {
	//EnterCriticalSection(&g_cs);
	system("cls");
	system("color 0B");
	draw::printRect(2, 5, 140, 35);

	drawWidget();
	printFunction();
	//LeaveCriticalSection(&g_cs);
}

 void ClientLayout::drawWidget()
{
	draw::gotoxy(3, SCORE_START + 12);
	cout << "已点歌曲";
	draw::printRow(4, SCORE_START + 8, 18);
	draw::drawTitle(9, SCORE_START + 1, "评分");
	draw::drawTitle(9, ID_START + 1, "编号");
	draw::drawTitle(9, NAME_START + 8, "歌曲名");
	draw::drawTitle(9, SINGER_START + 2, "歌手");
	draw::drawTitle(5, SCORE_START, "正在播放:");

	printPlayMusic();
	for (int i = 0; i <= musicNum; i++) {
		draw::printRow(10 + i * 2, SCORE_START, SINGER_END - SCORE_START + 1, '-');
	}

	draw::printCol(9, SCORE_START, musicNum * 2 + 2, '|');
	draw::printCol(9, ID_START, musicNum * 2 + 1, '|');
	draw::printCol(9, NAME_START, musicNum * 2 + 1, '|');
	draw::printCol(9, SINGER_START, musicNum * 2 + 1, '|');
	draw::printCol(9, SINGER_END, musicNum * 2 + 1, '|');
}

 void ClientLayout::printFunction() {
	draw::printRect(6, 20, 40, 10);
	draw::drawTitle(11, 34, "热门推荐");
	draw::printRect(20, 20, 40, 10);
	draw::drawTitle(25, 34, "曲库点歌");

}

 void ClientLayout::clearFuntion() {
	draw::clearWidget(5, 19, 42, 32);
}

 void ClientLayout::choiceUp(int & choice) {
	choiceHide(choice);
	choice--;
	if (choice < 0)choice += choiceNum;
	choiceHighlight(choice);
}

 void ClientLayout::choiceDown(int & choice) {
	choiceHide(choice);
	choice++;
	if (choice >= choiceNum)choice = 0;
	choiceHighlight(choice);
}

 void ClientLayout::choiceHighlight(int choice) {
	draw::printRect(10 + choice * 14, 32, 12, 3, '-', '|');
}

 void ClientLayout::choiceHide(int choice) {
	draw::printRect(10 + choice * 14, 32, 12, 3, ' ', ' ');
}

 void ClientLayout::MusicUp(int & choice, int y) {
	MusicHide(choice, y);
	choice--;
	if (choice < 0)choice += musicNum;
	MusicHighlight(choice, y);
}

 void ClientLayout::MusicDown(int & choice, int y) {
	MusicHide(choice, y);
	choice++;
	if (choice >= musicNum)choice = 0;
	MusicHighlight(choice, y);
}

 void ClientLayout::MusicHighlight(int choice, int y) {
	draw::gotoxy(11 + choice * 2, y);
	printf("->");
}

 void ClientLayout::MusicHide(int choice, int y) {
	draw::gotoxy(11 + choice * 2, y);
	printf("  ");
}

 void ClientLayout::MusicJump(int index) {
	if (index < currPage * musicNum || index>(currPage + 1) * musicNum) {
		currPage = index / musicNum;
		printPage(currPage, playMusics);
	}
	MusicHide(currMusic);
	currMusic = index % musicNum;
	MusicHighlight(currMusic);
}

/*****************************************/

 void ClientLayout::printPage(int & currPage, Info & info, int dis, int playMusic) {
	this->page = (info.musics_count() - 1 - playMusic) / musicNum;
	if (currPage < 0)currPage = page;
	if (currPage > this->page)currPage = 0;

	//eraseAll();
	printMusics(playMusic + musicNum * currPage, musicNum, info, dis);
}

 void ClientLayout::printMusics(int begin, int count, Info & info, int dis) {
	//vector<thread> printThreads;
	for (int i = 0; i < count; i++) {
		if (i + begin < info.musics_count()) {
			//printThreads.push_back(thread(&AdminLayout::printMusic, this, 7 + i * 2, i + begin));				
			//printThreads.back().join();
			printMusic(11 + i * 2, i + begin, info, dis);
		}
		else {
			erase(11 + i * 2, dis);
		}
	}
}

 void ClientLayout::printMusic(int x, int index, Info & info, int dis) {
	//EnterCriticalSection(&g_cs);
	Music m = info.getMusic(index);
	draw::gotoxy(x, SCORE_START + 1 - dis);
	printf("%-*.1f", ID_START - SCORE_START - 1, m.score);
	//cout << std::left << setw(ID_START - SCORE_START - 1) << m.score;
	draw::gotoxy(x, ID_START + 1 - dis);
	printf("%-*d", NAME_START - ID_START - 1, m.ID);
	//cout << std::left << setw(NAME_START - ID_START - 1) << m.ID;
	draw::gotoxy(x, NAME_START + 1 - dis);
	printf("%-*s", SINGER_START - NAME_START - 1, m.name.c_str());
	//cout << std::left << setw(SINGER_START - NAME_START - 1) << m.name;
	draw::gotoxy(x, SINGER_START + 1 - dis);
	printf("%-*s", SINGER_END - SINGER_START - 1, m.singer.c_str());
	//LeaveCriticalSection(&g_cs);
	//cout << std::left << setw(SINGER_END - SINGER_START - 1) << m.singer;
}

 void ClientLayout::erase(int x, int dis) {
	//EnterCriticalSection(&g_cs);
	draw::gotoxy(x, SCORE_START + 1 - dis);
	printf("%-*c", ID_START - SCORE_START - 1, ' ');
	//cout << setfill(' ') << setw(ID_START - SCORE_START - 1) << ' ';
	draw::gotoxy(x, ID_START + 1 - dis);
	printf("%-*c", NAME_START - ID_START - 1, ' ');
	//cout << setw(NAME_START - ID_START - 1) << ' ';
	draw::gotoxy(x, NAME_START + 1 - dis);
	printf("%-*c", SINGER_START - NAME_START - 1, ' ');
	//cout << setw(SINGER_START - NAME_START - 1) << ' ';
	draw::gotoxy(x, SINGER_START + 1 - dis);
	printf("%-*c", SINGER_END - SINGER_START - 1, ' ');
	//cout << setw(SINGER_END - SINGER_START - 1) << ' ';
	//LeaveCriticalSection(&g_cs);
}

 void ClientLayout::printPlayMusic() {
	if (playMusics.musics_count() == 0)return;
	playMusics.setMusicState(playMusic, 0);
	Music m = playMusics.getMusic(playMusic);
	if (m.ID == -1)return;
	draw::drawTitle(6, SCORE_START + 5, m.name);
	draw::drawTitle(7, SCORE_START + 5, m.singer);
}

 void ClientLayout::chooseFunction(int choice) {
	switch (choice)
	{

	case 0://热门歌曲
		hotMusics();
		break;
	case 1://曲库点歌
		vod();
		break;
	default:
		break;
	}
}

/********************必要功能的实现*******************************/

 void ClientLayout::hotMusics() {
	musicBank.clear();
	thread t(&ClientLayout::tcpRequestHotMusics, this);
	t.detach();
	int bankPage = 0;
	int currBankMusic = 0;
	EnterCriticalSection(&g_cs);
	clearFuntion();
	for (int i = 0; i <= musicNum; i++) {
		draw::printRow(10 + i * 2, SCORE_START - DISTANCE, SINGER_END - SCORE_START + 1, '-');
	}
	draw::drawTitle(7, 40, "热门歌曲");
	draw::printCol(10, SCORE_START - DISTANCE, musicNum * 2 + 1, '|');
	draw::printCol(10, ID_START - DISTANCE, musicNum * 2 + 0, '|');
	draw::printCol(10, NAME_START - DISTANCE, musicNum * 2 + 0, '|');
	draw::printCol(10, SINGER_START - DISTANCE, musicNum * 2 + 0, '|');
	draw::printCol(10, SINGER_END - DISTANCE, musicNum * 2 + 0, '|');
	draw::drawTitle(4, 7, "查找:ctrl+F");
	//InputHandle::getString(5, 11);
	printPage(bankPage, musicBank, DISTANCE);
	MusicHide(currMusic);
	MusicHighlight(currBankMusic, SCORE_START - DISTANCE - 3);
	LeaveCriticalSection(&g_cs);
	while (1) {
		switch (InputHandle::getKeyInput())
		{
		case InputHandle::KEY_UP:
			MusicUp(currBankMusic, SCORE_START - DISTANCE - 3);
			break;
		case InputHandle::KEY_LEFT://上一页
			printPage(--bankPage, musicBank, DISTANCE);
			break;
		case InputHandle::KEY_DOWN:
			MusicDown(currBankMusic, SCORE_START - DISTANCE - 3);
			break;
		case InputHandle::KEY_RIGHT://下一页
			printPage(++bankPage, musicBank, DISTANCE);
			break;
		case InputHandle::ESC:
			draw::clearWidget(3, 6, 65, 33);
			printFunction();
			choiceHighlight(0);
			MusicHighlight(currMusic);
			return;
			break;
		case InputHandle::KEY_Q:

			break;
		case InputHandle::KEY_E:

			break;
		case InputHandle::KEY_W:

			break;
		case InputHandle::KEY_S:

			break;
		case InputHandle::KEY_ENTER://点歌
			clickMusic(currBankMusic + bankPage * musicNum);
			break;
		case InputHandle::CTRL_T://
			musicBank.setTmpState(false);
			currBankMusic = 0;
			bankPage = 0;
			printPage(bankPage, musicBank, DISTANCE);
			break;
		case InputHandle::CTRL_F://查找
			if (search()) {
				//currBankMusic = 0;
				bankPage = 0;
				printPage(bankPage, musicBank, DISTANCE);
			}
			else {
				draw::drawTitle(8, 8, "no found");
				Sleep(500);
			}
			draw::clearRow(6, 8, 20);
			draw::clearRow(7, 8, 20);
			draw::clearRow(8, 8, 20);
			break;
		default:
			break;
		}
	}
}

 void ClientLayout::vod() {
	musicBank.clear();
	thread t(&ClientLayout::tcpRequestInfo, this);
	t.detach();
	int bankPage = 0;
	int currBankMusic = 0;
	EnterCriticalSection(&g_cs);
	clearFuntion();
	for (int i = 0; i <= musicNum; i++) {
		draw::printRow(10 + i * 2, SCORE_START - DISTANCE, SINGER_END - SCORE_START + 1, '-');
	}
	draw::drawTitle(7, 40, "曲库点歌");
	draw::printCol(10, SCORE_START - DISTANCE, musicNum * 2 + 1, '|');
	draw::printCol(10, ID_START - DISTANCE, musicNum * 2 + 0, '|');
	draw::printCol(10, NAME_START - DISTANCE, musicNum * 2 + 0, '|');
	draw::printCol(10, SINGER_START - DISTANCE, musicNum * 2 + 0, '|');
	draw::printCol(10, SINGER_END - DISTANCE, musicNum * 2 + 0, '|');
	draw::drawTitle(4, 7, "查找:ctrl+F");
	//InputHandle::getString(5, 11);
	printPage(bankPage, musicBank, DISTANCE);
	MusicHide(currMusic);
	MusicHighlight(currBankMusic, SCORE_START - DISTANCE - 3);
	LeaveCriticalSection(&g_cs);
	while (1) {
		switch (InputHandle::getKeyInput())
		{
		case InputHandle::KEY_UP:
			MusicUp(currBankMusic, SCORE_START - DISTANCE - 3);
			break;
		case InputHandle::KEY_LEFT://上一页
			printPage(--bankPage, musicBank, DISTANCE);
			break;
		case InputHandle::KEY_DOWN:
			MusicDown(currBankMusic, SCORE_START - DISTANCE - 3);
			break;
		case InputHandle::KEY_RIGHT://下一页
			printPage(++bankPage, musicBank, DISTANCE);
			break;
		case InputHandle::ESC:
			draw::clearWidget(3, 6, 65, 33);
			printFunction();
			choiceHighlight(1);
			MusicHighlight(currMusic);
			return;
			break;
		case InputHandle::KEY_Q:

			break;
		case InputHandle::KEY_E:

			break;
		case InputHandle::KEY_W:

			break;
		case InputHandle::KEY_S:

			break;
		case InputHandle::KEY_ENTER://点歌
			clickMusic(currBankMusic + bankPage * musicNum);
			break;
		case InputHandle::CTRL_T://
			musicBank.setTmpState(false);
			currBankMusic = 0;
			bankPage = 0;
			printPage(bankPage, musicBank, DISTANCE);
			break;
		case InputHandle::CTRL_F://查找
			if (search()) {
				//currBankMusic = 0;
				bankPage = 0;
				printPage(bankPage, musicBank, DISTANCE);
			}
			else {
				draw::drawTitle(8, 8, "no found");
				Sleep(500);
			}
			draw::clearRow(6, 8, 20);
			draw::clearRow(7, 8, 20);
			draw::clearRow(8, 8, 20);
			break;
		default:
			break;
		}
	}
}

 void ClientLayout::clickMusic(int index) {
	playMusics.add(musicBank.getMusic(index));
	currPage = page;
	currMusic = 0;
	if (playEmpty) {
		if (playMusic > 0)
			playMusics.nextMusic(playMusic);
		printPlayMusic();
		playEmpty = false;
		//playMusics.nextMusic(playMusic);

	}
	printPage(currPage, playMusics, 0, playMusic + 1);

}

 int ClientLayout::search() {
	hHook2 = SetWindowsHookEx(WH_CBT, (HOOKPROC)CBTHookProc2, NULL, GetCurrentThreadId());
	int MB_RES = MessageBox(GetForegroundWindow(), "请选择查找方式", "选择", 3);
	if (MB_RES == 6) {//查找歌手
		draw::drawTitle(6, 8, "输入你要查找的歌手");
		return musicBank.singerSearch(InputHandle::getString(7, 8));
	}
	else if (MB_RES == 7) {//查找歌曲
		draw::drawTitle(6, 8, "输入你要查找的歌曲");
		return musicBank.musicSearch(InputHandle::getString(7, 8));
	}
	else {
		return 0;
	}

}

 void ClientLayout::nextMusic() {
	draw::clearRow(6, SCORE_START + 4, 20);
	draw::clearRow(7, SCORE_START + 4, 20);
	if (playMusic >= playMusics.musics_count() - 1) {
		playEmpty = true;
		return;
	}
	playMusics.nextMusic(playMusic);
	printPlayMusic();
	printPage(currPage, playMusics, 0, playMusic + 1);
}

 void ClientLayout::putOnTop() {
	int index = playMusic + 1 + currPage * musicNum + currMusic;
	Music m = playMusics.getMusic(index);
	if (m.ID == -1)return;
	if (MessageBox(NULL, string("要把歌曲：" + m.name + "  置顶吗").c_str(), "置顶", 1) == 2)return;
	playMusics.eraseMusic(index);
	playMusics.insert(m, playMusic + 1);
	refresh2();
}

 void ClientLayout::mark() {
	int index = playMusic + 1 + currPage * musicNum + currMusic;
	Music m = playMusics.getMusic(index);
	if (m.ID == -1)return;

	MusicHide(currMusic);
	draw::printMarkInfo(11 + currMusic * 2, SCORE_START - 16);
	int score = 0;
	int x_s = 11 + currMusic * 2;
	int y_s = SCORE_START - 15;
	while (1) {
		switch (InputHandle::getKeyInput())
		{
		case InputHandle::KEY_LEFT:
			if (score > 0) {
				draw::starHide(x_s, y_s + score * 2);
				score--;
			}

			break;
		case InputHandle::KEY_RIGHT:
			if (score < 5) {
				score++;
				draw::starHightlight(x_s, y_s + score * 2);
			}
			break;
		case InputHandle::ESC:
			draw::clearRow(10 + currMusic * 2, SCORE_START - 16, 16);
			draw::clearRow(11 + currMusic * 2, SCORE_START - 16, 16);
			return;
			break;
		case InputHandle::KEY_ENTER:
			tcpSendScore(playMusics.getMusic(index).ID, score);
			//playMusics.markMusic(index, score);				
			printPage(currPage, playMusics, 0, playMusic + 1);
			draw::clearRow(10 + currMusic * 2, SCORE_START - 16, 16);
			draw::clearRow(11 + currMusic * 2, SCORE_START - 16, 16);
			MusicHighlight(currMusic);
			return;
			break;
		default:
			break;
		}
	}
}

 void ClientLayout::refresh() {
	playMusics.clear();
	tcpThread = thread(&ClientLayout::tcpRequest, this);
	tcpThread.join();
	if (connected) {
		this->currPage = 0;
		printPage(this->currPage, musicBank);
	}
}

 void ClientLayout::refresh2() {
	currPage = 0;
	MusicHide(currMusic);
	currMusic = 0;
	printPage(currPage, playMusics, 0, playMusic + 1);
}

 void ClientLayout::get_IP() {
	system("cls");
	cout << "请输入服务器的ip（本机运行可输入127.0.0.1）:";
	ip = InputHandle::getString(2, 20);
}

 void ClientLayout::tcpRequest() {
	quit = false;
	while (!tcpClient.init(ip)) {
		connected = 0;
		Sleep(200);
		if (quit)return;
	}
	connected = 1;
	greetWord = tcpClient.getGreetWord();

}

 void ClientLayout::tcpRequestInfo() {
	if (!connected)return;
	tcpClient.request("info");
	Sleep(200);	
	string temp;
	temp = tcpClient.recvMessage();
	//musicBank.clear();
	while (temp != "EOF") {
		if (temp != "error") {
			musicBank.tcpAssist(temp);
		}
		temp = tcpClient.recvMessage();
	}
	int p = 0;
	EnterCriticalSection(&g_cs);
	printPage(p, musicBank, DISTANCE);
	LeaveCriticalSection(&g_cs);
}

 void ClientLayout::tcpRequestHotMusics() {
	if (!connected)return;
	tcpClient.request("hotmusic");
	Sleep(200);
	
	string temp;
	temp = tcpClient.recvMessage();
	//musicBank.clear();
	while (temp != "EOF") {
		if (temp != "error") {
			musicBank.tcpAssist(temp);
		}
		temp = tcpClient.recvMessage();
	}
	int p = 0;
	EnterCriticalSection(&g_cs);
	printPage(p, musicBank, DISTANCE);
	LeaveCriticalSection(&g_cs);
}

 void ClientLayout::tcpSendScore(int ID, int score) {
	if (!connected)return;
	tcpClient.request("mark");
	char s1[10], s2[3];
	_itoa(ID, s1, 10);
	_itoa(score, s2, 10);
	string s(s1);
	s.append(" ");
	s.append(s2);
	tcpClient.request(s);
}
