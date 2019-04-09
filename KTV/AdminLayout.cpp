#include "AdminLayout.h"
HHOOK hHook;
LRESULT __stdcall CBTHookProc(long nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode == HCBT_ACTIVATE)
	{
		SetDlgItemText((HWND)wParam, IDYES, "����");
		SetDlgItemText((HWND)wParam, IDNO, "���Ϊ");
		UnhookWindowsHookEx(hHook);
	}
	return 0;
}
 AdminLayout::AdminLayout() {
	info.loadFile("music.txt");
	saved = true;
	tcpServe();
	checkThread = thread(&AdminLayout::checkMusicMark, this);
}

int AdminLayout::exec() {
	render();
	isRunning = true;
	detailWidgetInit();
	int choice = 0;
	currMusic = 0;
	MusicHighlight(currMusic);
	choiceHighlight(choice);
	checkThread.detach();
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
		case InputHandle::KEY_Q://��һҳ
			printPage(--currPage);

			break;
		case InputHandle::KEY_E://��һҳ
			printPage(++currPage);

			break;
		case InputHandle::KEY_W:
			MusicUp(currMusic);

			break;
		case InputHandle::KEY_S:
			MusicDown(currMusic);

			break;
		case InputHandle::ESC:  //���淵��
			if (!saved) {
				if (MessageBox(NULL, " �Ƿ񱣴棿", "δ����", 1) == 1) {
					info.saveInfo("music.txt");
				}
			}
			tcpServer.quit();
			isRunning = false;
			return -1;
			break;
		case InputHandle::CTRL_D:  //ɾ��
			deleteMusic();
			break;
		case InputHandle::CTRL_F:  //����
			search();
			break;
		case InputHandle::CTRL_T:  //ˢ��
			info.setTmpState(false);
			refresh();
			drawFunctionWidget();
			break;
		case InputHandle::CTRL_S:  //����
			if (!saved) {
				info.saveInfo("music.txt");
				MessageBox(NULL, "����ɹ�", "����", MB_OK);
			}
			break;
		default:
			break;
		}
	}
	return 1;
}

 AdminLayout::~AdminLayout() {
	Sleep(1000);
	info.~Info();
	tcpServer.~TcpServer();
	tcpThread.~thread();
	checkThread.~thread();
}

 void AdminLayout::render() {
	system("cls");
	system("color 0B");
	draw::printRect(2, 5, 140, 35);
	drawFunctionWidget();
	drawWidget();
	printFunction();
	currPage = 0;
	printPage(currPage);
}

 void AdminLayout::drawWidget()
{
	draw::gotoxy(3, 48);
	cout << "KTV����";
	draw::printRow(4, 44, 18);
	draw::drawTitle(5, SCORE_START + 1, "����");
	draw::drawTitle(5, ID_START + 1, "���");
	draw::drawTitle(5, NAME_START + 8, "������");
	draw::drawTitle(5, SINGER_START + 2, "����");

	for (int i = 0; i < 15; i++) {
		draw::printRow(6 + i * 2, 6, SINGER_END - SCORE_START + 1, '-');
	}

	draw::printCol(5, SCORE_START, 30, '|');
	draw::printCol(5, ID_START, 29, '|');
	draw::printCol(5, NAME_START, 29, '|');
	draw::printCol(5, SINGER_START, 29, '|');
	draw::printCol(5, SINGER_END, 29, '|');
}

 void AdminLayout::printFunction() {
	draw::drawFunction(10, FUNCTION_Y, "��������");
	draw::drawFunction(14, FUNCTION_Y, "��������");
	draw::drawFunction(18, FUNCTION_Y, "��������");
	draw::drawFunction(22, FUNCTION_Y, "���");
	draw::drawFunction(26, FUNCTION_Y, "�޸�");
	draw::drawFunction(30, FUNCTION_Y, "����");
	draw::drawFunction(34, FUNCTION_Y, "����");
}

 void AdminLayout::choiceUp(int & choice) {
	choiceHide(choice);
	choice--;
	if (choice < 0)choice += choiceNum;
	choiceHighlight(choice);
}

 void AdminLayout::choiceDown(int & choice) {
	choiceHide(choice);
	choice++;
	if (choice >= choiceNum)choice = 0;
	choiceHighlight(choice);
}

 void AdminLayout::choiceHighlight(int choice) {
	draw::gotoxy(10 + choice * 4, FUNCTION_Y - 3);
	cout << '#';
}

 void AdminLayout::choiceHide(int choice) {
	draw::gotoxy(10 + choice * 4, FUNCTION_Y - 3);
	cout << ' ';
}

/*******************���ֹ���ƶ�*********************/

 void AdminLayout::MusicUp(int & choice) {
	MusicHide(choice);
	choice--;
	if (choice < 0)choice += 14;
	MusicHighlight(choice);
}

 void AdminLayout::MusicDown(int & choice) {
	MusicHide(choice);
	choice++;
	if (choice > 13)choice = 0;
	MusicHighlight(choice);
}

 void AdminLayout::MusicHighlight(int choice) {
	draw::gotoxy(7 + choice * 2, 2);
	cout << '@';
}

 void AdminLayout::MusicHide(int choice) {
	draw::gotoxy(7 + choice * 2, 2);
	cout << ' ';
}

 void AdminLayout::MusicJump(int index) {
	if (index < currPage * 14 || index >= (currPage + 1) * 14) {
		currPage = index / 14;
		//printPage(currPage);
	}
	MusicHide(currMusic);
	currMusic = index % 14;
	MusicHighlight(currMusic);
}

/*****************************************/
//CRITICAL_SECTION g_cs;

 void AdminLayout::printPage(int & currPage) {
	draw::printClientInfo(tcpServer.getClientAddr());
	this->page = (info.musics_count() - 1) / 14;
	if (currPage < 0)currPage = page;
	if (currPage > this->page)currPage = 0;

	//eraseAll();
	printMusics(14 * currPage, 14);
}

 void AdminLayout::printMusics(int begin, int count) {
	//vector<thread> printThreads;
	//InitializeCriticalSection(&g_cs);
	for (int i = 0; i < count; i++) {
		if (i + begin < info.musics_count()) {
			//printThreads.push_back(thread(&AdminLayout::printMusic, this, 7 + i * 2, i + begin));				
			printMusic(7 + i * 2, i + begin);
		}
		else {
			//printThreads.push_back(thread(&AdminLayout::erase, this, 7 + i * 2));
			erase(7 + i * 2);
		}
	}
	/*
	for (auto &t : printThreads) {
	t.join();
	}*/
}

 void AdminLayout::printMusic(int x, int index) {
	//EnterCriticalSection(&g_cs);
	Music m = info.getMusic(index);
	draw::gotoxy(x, SCORE_START + 1);
	printf("%-*.1f", ID_START - SCORE_START - 1, m.score);
	//cout << std::left << setw(ID_START - SCORE_START - 1) << m.score;
	draw::gotoxy(x, ID_START + 1);
	printf("%-*d", NAME_START - ID_START - 1, m.ID);
	//cout << std::left << setw(NAME_START - ID_START - 1) << m.ID;
	draw::gotoxy(x, NAME_START + 1);
	printf("%-*s", SINGER_START - NAME_START - 1, m.name.c_str());
	//cout << std::left << setw(SINGER_START - NAME_START - 1) << m.name;
	draw::gotoxy(x, SINGER_START + 1);
	printf("%-*s", SINGER_END - SINGER_START - 1, m.singer.c_str());
	//LeaveCriticalSection(&g_cs);
	//cout << std::left << setw(SINGER_END - SINGER_START - 1) << m.singer;
}

 void AdminLayout::erase(int x) {
	//EnterCriticalSection(&g_cs);
	draw::gotoxy(x, SCORE_START + 1);
	printf("%-*c", ID_START - SCORE_START - 1, ' ');
	//cout << setfill(' ') << setw(ID_START - SCORE_START - 1) << ' ';
	draw::gotoxy(x, ID_START + 1);
	printf("%-*c", NAME_START - ID_START - 1, ' ');
	//cout << setw(NAME_START - ID_START - 1) << ' ';
	draw::gotoxy(x, NAME_START + 1);
	printf("%-*c", SINGER_START - NAME_START - 1, ' ');
	//cout << setw(SINGER_START - NAME_START - 1) << ' ';
	draw::gotoxy(x, SINGER_START + 1);
	printf("%-*c", SINGER_END - SINGER_START - 1, ' ');
	//cout << setw(SINGER_END - SINGER_START - 1) << ' ';
	//LeaveCriticalSection(&g_cs);
}

 void AdminLayout::chooseFunction(int choice) {
	switch (choice)
	{
	case 0://��������
		loadMore();
		break;
	case 1://��������
		rank();
		break;
	case 2://��������
		pushHotMusic();
		break;
	case 3://���
		add();
		printPage(currPage);//ˢ��
		break;
	case 4://�޸�
		modify();
		break;
	case 5://����
		search();
		break;
	case 6://����
		save();
		break;

	default:
		break;
	}
}

/********************��Ҫ���ܵ�ʵ��*******************************/

 void AdminLayout::rank() {
	info.rankMusics();
	refresh();
}

 void AdminLayout::pushHotMusic() {
	MessageBox(NULL, "������", "���Ÿ���", 0);
}

 void AdminLayout::add() {
	draw::clearWidget(FunctionWidget_X,
		FunctionWidget_Y,
		FunctionWidget_WIDTH,
		FunctionWidget_HEIGHT);
	draw::printAddIntro(FunctionWidget_X, FunctionWidget_Y);
	string name, singer, abb;
	//_getch();
	getchar();
	while (1) {
		name = InputHandle::getString(FunctionWidget_X + 2, FunctionWidget_Y + 13);
		if (name.length() > 1 && name.length() < 20) {
			break;
		}
		else {
			draw::cerrWidgetBottom(FunctionWidget_X,
				FunctionWidget_Y,
				FunctionWidget_WIDTH,
				FunctionWidget_HEIGHT,
				"���������벻�Ϸ�");
			draw::clearRow(FunctionWidget_X + 2, FunctionWidget_Y + 13, 30);
		}
	}
	while (1) {
		singer = InputHandle::getString(FunctionWidget_X + 3, FunctionWidget_Y + 13);
		if (singer.length() > 1 && singer.length() < 20) {
			break;
		}
		else {
			draw::cerrWidgetBottom(FunctionWidget_X,
				FunctionWidget_Y,
				FunctionWidget_WIDTH,
				FunctionWidget_HEIGHT,
				"���������벻�Ϸ�");
			draw::clearRow(FunctionWidget_X + 3, FunctionWidget_Y + 13, 30);
		}
	}
	while (1) {
		abb = InputHandle::getString(FunctionWidget_X + 4, FunctionWidget_Y + 13);
		if (abb.length() > 1 && abb.length() < 20) {
			break;
		}
		else {
			draw::cerrWidgetBottom(FunctionWidget_X,
				FunctionWidget_Y,
				FunctionWidget_WIDTH,
				FunctionWidget_HEIGHT,
				"������д���벻�Ϸ�");
			draw::clearRow(FunctionWidget_X + 4, FunctionWidget_Y + 13, 30);
		}
	}
	if (info.add(info.createMusic(name, singer, abb))) {//������bug
		draw::cerrWidgetBottom(FunctionWidget_X,
			FunctionWidget_Y,
			FunctionWidget_WIDTH,
			FunctionWidget_HEIGHT,
			"������ӳɹ�");
	}
	else {
		draw::cerrWidgetBottom(FunctionWidget_X,
			FunctionWidget_Y,
			FunctionWidget_WIDTH,
			FunctionWidget_HEIGHT,
			"�ø����Ѵ���");
	}
	Sleep(300);
	draw::clearWidget(FunctionWidget_X,
		FunctionWidget_Y,
		FunctionWidget_WIDTH,
		FunctionWidget_HEIGHT);

	saved = false;
}

 void AdminLayout::modify() {
	draw::clearWidget(FunctionWidget_X,
		FunctionWidget_Y,
		FunctionWidget_WIDTH,
		FunctionWidget_HEIGHT);
	Music m(info.getMusic(currMusic + currPage * 14));
	draw::printModifyInfo(FunctionWidget_X, FunctionWidget_Y, m);
	string name = InputHandle::getString(FunctionWidget_X + 2, FunctionWidget_Y + m.name.length() + 6);
	string singer = InputHandle::getString(FunctionWidget_X + 3, FunctionWidget_Y + m.singer.length() + 6);
	string abb = InputHandle::getString(FunctionWidget_X + 4, FunctionWidget_Y + m.abb.length() + 6);
	if (!name.empty())m.name = name;
	if (!singer.empty())m.singer = singer;
	if (!abb.empty())m.abb = abb;
	info.eraseMusic(currMusic + currPage * 14);
	info.insert(m, currMusic + currPage * 14);
	saved = false;
	draw::cerrWidgetBottom(FunctionWidget_X,
		FunctionWidget_Y,
		FunctionWidget_WIDTH,
		FunctionWidget_HEIGHT,
		"�޸ĳɹ�");
	Sleep(500);

	draw::clearWidget(FunctionWidget_X,
		FunctionWidget_Y,
		FunctionWidget_WIDTH,
		FunctionWidget_HEIGHT);
	printPage(currPage);
}

 void AdminLayout::search() {
	draw::clearWidget(FunctionWidget_X,
		FunctionWidget_Y,
		FunctionWidget_WIDTH,
		FunctionWidget_HEIGHT);
	int searchChoice = 0;
	draw::printSearchIntro(FunctionWidget_X, FunctionWidget_Y);
	searchChoiceHighlight(searchChoice);
	while (1) {
		switch (InputHandle::getKeyInput())
		{
		case InputHandle::KEY_UP:
			searchChoiceUp(searchChoice);
			break;
		case InputHandle::KEY_DOWN:
			searchChoiceDown(searchChoice);
			break;
		case InputHandle::KEY_ENTER:
			getchar();
			if (searchChoice == 0)search_by_name();
			if (searchChoice == 1)search_by_singer();
			draw::clearWidget(FunctionWidget_X,
				FunctionWidget_Y,
				FunctionWidget_WIDTH,
				FunctionWidget_HEIGHT);
			return;
			break;

		}
	}
}

 void AdminLayout::searchChoiceUp(int & choice) {
	searchChoiceHide(choice);
	choice--;
	if (choice < 0)choice += 2;
	searchChoiceHighlight(choice);
}

 void AdminLayout::searchChoiceDown(int & choice) {
	searchChoiceHide(choice);
	choice++;
	if (choice >= 2)choice = 0;
	searchChoiceHighlight(choice);
}

 void AdminLayout::searchChoiceHighlight(int choice) {
	draw::gotoxy(FunctionWidget_X + 3 + choice, FunctionWidget_Y + 3);
	cout << '*';
}

 void AdminLayout::searchChoiceHide(int choice) {
	draw::gotoxy(FunctionWidget_X + 3 + choice, FunctionWidget_Y + 3);
	cout << ' ';
}

 int AdminLayout::search_by_singer() {
	draw::clearWidget(FunctionWidget_X,
		FunctionWidget_Y,
		FunctionWidget_WIDTH,
		FunctionWidget_HEIGHT);
	draw::drawTitle(FunctionWidget_X + 1, FunctionWidget_Y + 1, "������Ҫ���ҵĸ�������");
	string singer = InputHandle::getString(FunctionWidget_X + 2, FunctionWidget_Y + 14);

	if (info.singerSearch(singer) == 0) {
		MessageBox(NULL, "û�иø���", "����", 0);
		info.setTmpState(false);
	}
	refresh();
	return 1;
}

 int AdminLayout::search_by_name() {
	draw::clearWidget(FunctionWidget_X,
		FunctionWidget_Y,
		FunctionWidget_WIDTH,
		FunctionWidget_HEIGHT);
	draw::drawTitle(FunctionWidget_X + 1, FunctionWidget_Y + 1, "������Ҫ���ҵĸ�����(����д)��");
	string name = InputHandle::getString(FunctionWidget_X + 2, FunctionWidget_Y + 14);

	if (info.musicSearch(name) == 0) {
		MessageBox(NULL, "û�иø���", "����", 0);
		info.setTmpState(false);
	}
	refresh();
	return 1;
}

 void AdminLayout::loadMore() {
	OPENFILENAME ofn = { 0 };
	TCHAR strFilename[MAX_PATH] = { 0 };//���ڽ����ļ���
	ofn.lStructSize = sizeof(OPENFILENAME);//�ṹ���С
	ofn.hwndOwner = NULL;//ӵ���Ŵ��ھ����ΪNULL��ʾ�Ի����Ƿ�ģ̬�ģ�ʵ��Ӧ����һ�㶼Ҫ��������
	ofn.lpstrFilter = TEXT("�����ļ�\0*.*\0C/C++ Flie\0*.cpp;*.c;*.h\0\0");//���ù���
	ofn.nFilterIndex = 1;//����������
	ofn.lpstrFile = strFilename;//���շ��ص��ļ�����ע���һ���ַ���ҪΪNULL
	ofn.nMaxFile = sizeof(strFilename);//����������
	ofn.lpstrInitialDir = NULL;//��ʼĿ¼ΪĬ��
	ofn.lpstrTitle = TEXT("��ѡ��һ���ļ�");//ʹ��ϵͳĬ�ϱ������ռ���
	ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY;//�ļ���Ŀ¼������ڣ�����ֻ��ѡ��
	if (GetOpenFileName(&ofn))
	{

		string s;
		char num[5];
		s += "�ɹ�����";
		int res = info.loadFile(strFilename);
		if (res == -1) {
			MessageBox(NULL, "�ļ����ʹ���", TEXT("����ʧ��"), 0);
			return;
		}
		_itoa(res, num, 5);
		s += num;
		s += "�׸���";
		MessageBox(NULL, s.c_str(), TEXT("����ɹ�"), 0);
		saved = false;
	}
	else {

	}
}

 void AdminLayout::save() {
	hHook = SetWindowsHookEx(WH_CBT, (HOOKPROC)CBTHookProc, NULL, GetCurrentThreadId());
	int MB_RES = MessageBox(NULL, "ѡ�񱣴淽ʽ", "����", 3);
	if (MB_RES == 6) {
		info.saveInfo("music.txt");
		MessageBox(NULL, TEXT("����ɹ�"), TEXT("����"), 0);
		saved = true;
	}
	else if (MB_RES == 7) {
		OPENFILENAME ofn = { 0 };
		TCHAR strFilename[MAX_PATH] = "music.txt";//���ڽ����ļ���
		ofn.lStructSize = sizeof(OPENFILENAME);//�ṹ���С
		ofn.hwndOwner = NULL;//ӵ���Ŵ��ھ����ΪNULL��ʾ�Ի����Ƿ�ģ̬�ģ�ʵ��Ӧ����һ�㶼Ҫ��������
		ofn.lpstrFilter = TEXT("�����ļ�\0*.*\0C/�ı��ĵ�\0*.txt\0\0");//���ù���
		ofn.nFilterIndex = 1;//����������
		ofn.lpstrFile = strFilename;//���շ��ص��ļ�����ע���һ���ַ���ҪΪNULL
		ofn.nMaxFile = sizeof(strFilename);//����������
		ofn.lpstrInitialDir = NULL;//��ʼĿ¼ΪĬ��
		ofn.lpstrTitle = TEXT("");//ʹ��ϵͳĬ�ϱ������ռ���
		ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY;//�ļ���Ŀ¼������ڣ�����ֻ��ѡ��
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;//Ŀ¼������ڣ������ļ�ǰ��������
		ofn.lpstrTitle = TEXT("���浽");//ʹ��ϵͳĬ�ϱ������ռ���
		ofn.lpstrDefExt = TEXT("cpp");//Ĭ��׷�ӵ���չ��
		if (GetSaveFileName(&ofn))
		{
			info.saveInfo(strFilename);
			MessageBox(NULL, TEXT("����ɹ�"), TEXT("����"), 0);
			//saved = true;
		}
		else {
			//ȡ�����Ϊ
		}
	}
	else {

	}


}

 void AdminLayout::deleteMusic() {
	int sure;
	string cont = "����ɾ��:" + info.getMusic(currMusic + currPage * 14).name;
	if (cont.empty()) {
		MessageBox(GetForegroundWindow(), " û�иø���", "", 1);
		return;
	}
	sure = MessageBox(GetForegroundWindow(), cont.c_str(), "ȷ��ɾ���ø�����", 1);
	if (sure == 1) {
		if (info.deleteMusic(currMusic + currPage * 14)) {
			printPage(currPage);
			saved = false;
		}
		else {
			MessageBox(GetForegroundWindow(), " û�иø���", "", 1);
		}
	}
}

 void AdminLayout::refresh() {
	currPage = 0;
	MusicHide(currMusic);
	currMusic = 0;
	MusicHighlight(currMusic);
	printPage(currPage);
}

 void AdminLayout::checkMusicMark() {
	while (1) {
		if (!isRunning)break;
		while (tcpServer.count >= 0) {
			info.markMusic(tcpServer.musicMark[tcpServer.count][0], tcpServer.musicMark[tcpServer.count][1]);
			--tcpServer.count;
			saved = false;
		}
		Sleep(1000);//ÿ1����һ�θ���
	}

}

/************************�����û�С��************************************/

 void AdminLayout::detailWidgetInit() {
	draw::printRect(4, 85, 55, 10);
	draw::drawTitle(5, 90, "�������û�");
}

 void AdminLayout::drawFunctionWidget() {
	draw::printRect(FunctionWidget_X, FunctionWidget_Y, FunctionWidget_WIDTH, FunctionWidget_HEIGHT);
	draw::drawTitle(18, 86, "����˵����");
	draw::drawTitle(19, 88, "������һҳ��q ��һҳ��e");
	draw::drawTitle(20, 88, "��������ƶ���w��s��");
	draw::drawTitle(21, 88, "���ܹ���ƶ������¼�ͷ");
	draw::drawTitle(22, 88, "�س�ȷ��");
	draw::drawTitle(23, 88, "��ݼ���");
	draw::drawTitle(24, 90, "ctrl+F -> ����");
	draw::drawTitle(25, 90, "ctrl+D -> ɾ��");
	draw::drawTitle(26, 90, "ctrl+T -> ˢ��");
	draw::drawTitle(27, 90, "ctrl+S -> ����");
	draw::drawTitle(28, 88, "�˳� ESC");
}

 void AdminLayout::tcpServe() {
	tcpServer.init();
	tcpThread = thread(&TcpServer::tcpServerMain, std::ref(tcpServer));
	tcpThread.detach();
}
