#include "LoginLayout.h"

 int LoginLayout::exec() {
	render();
	int choice = 0;
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
			return chooseFuntion(choice);
			break;

		default:
			break;
		}

	}
}

 int LoginLayout::chooseFuntion(int choice) {
	 int try_times;
	switch (choice)
	{
	case 0:
		try_times = 3;
		while (try_times-- > 0) {
			if (adminLayout())return 0;
		}
		return -1;	
		break;
	case 1:
		if (userLayout())return 1;
		break;
	case 2:
		changeKey();
		break;
	case 3:
		exit(0);
		break;
	default:
		break;
	}
}

 void LoginLayout::choiceUp(int & choice) {
	choiceHide(choice);
	choice--;
	if (choice < 0)choice += 4;
	choiceHighlight(choice);
}

 void LoginLayout::choiceDown(int & choice) {
	choiceHide(choice);
	choice++;
	if (choice > 3)choice = 0;
	choiceHighlight(choice);
}

 void LoginLayout::choiceHighlight(int choice) {
	draw::gotoxy(12 + choice * 4, 30);
	cout << '#';
}

 void LoginLayout::choiceHide(int choice) {
	draw::gotoxy(12 + choice * 4, 30);
	cout << ' ';
}

 boolean LoginLayout::adminLayout() {
	system("cls");
	system("color 0B");
	draw::printRect(2, 5, 100, 35);//绘制外边框

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
	draw::gotoxy(10, 24);
	cout << "请输入用户名:";
	draw::gotoxy(15, 24);
	cout << "请输入密码:";
	draw::gotoxy(30, 26);
	cout << "#回车登录#";
	string name, key;
	name = InputHandle::getString(10, 40);
	draw::gotoxy(15, 35);
	key = InputHandle::getString(15, 40);
	fstream file("adminKey.txt", ios::in);
	string trueKey, trueName;
	file >> trueName >> trueKey;
	file.close();
	if (name == trueName && key == trueKey)
		return true;
	return false;
}

 boolean LoginLayout::userLayout() {
	return true;
}

 void LoginLayout::render() {
	system("cls");
	system("color 0B");
	draw::printRect(2, 5, 140, 35);//绘制外边框

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
	draw::gotoxy(12, 30);
	cout << '#';
	draw::gotoxy(12, 32);
	cout << "管理员登录";
	draw::gotoxy(16, 32);
	cout << "普通用户登录";
	draw::gotoxy(20, 32);
	cout << "修改管理员密码";
	draw::gotoxy(24, 32);
	cout << "退出";
}

 void LoginLayout::changeKey()
 {
	 system("cls");
	 system("color 0B");
	 draw::printRect(2, 5, 100, 35);//绘制外边框

	 SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
	 draw::gotoxy(10, 24);
	 cout << "请输入用户名:";
	 draw::gotoxy(15, 24);
	 cout << "请输入原密码:";
	 draw::gotoxy(20, 24);
	 cout << "请输入新密码:";
	// draw::gotoxy(30, 26);
	 //cout << "#回车登录#";
	 string name, key;
	 name = InputHandle::getString(10, 40);
	 draw::gotoxy(15, 35);
	 key = InputHandle::getString(15, 40);
	 fstream file("adminKey.txt", ios::in);
	 string trueKey, trueName;
	 file >> trueName >> trueKey;
	 file.close();
	 if (trueName == name && trueKey == key) {
		 string newKey = InputHandle::getString(20, 40);
		 while (newKey.length() < 3) {
			 draw::clearRow(20, 40, 30);
			 newKey = InputHandle::getString(20, 40);
		 }
		 file.open("adminKey.txt", ios::out);
		 file << name << ' ' << newKey;
		 file.close();
		 draw::clearRow(10, 24, 30);
		 draw::clearRow(15, 24, 30);
		 draw::clearRow(20, 24, 30);
		 draw::drawFunction(20, 40, "修改成功");
		 Sleep(1000);
		 
	 }
	 else {

	 }
	 render();
 }
