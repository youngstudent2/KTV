#include "Draw.h"

void draw::gotoxy(int x, int y) {
	COORD pos;
	HANDLE hOutput;
	pos.X = y;
	pos.Y = x;
	hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hOutput, pos);
}

 void draw::printRow(int x, int y, int width, char ch) {
	gotoxy(x, y);
	string s(width, ch);//提高打印速度
	printf("%s", s.c_str());
	/*
	for (int i = 0; i < width; i++) {
	printf("%c", ch);
	}*/
}

 void draw::printCol(int x, int y, int height, char ch) {
	for (int i = 0; i < height; i++) {
		gotoxy(x + i, y);
		printf("%c", ch);
	}
}

 void draw::printRect(int x, int y, int width, int height, char ch1, char ch2) {
	printRow(x, y, width, ch1);
	printRow(x + height - 1, y, width, ch1);
	printCol(x + 1, y, height - 2, ch2);
	printCol(x + 1, y + width - 1, height - 2, ch2);
}

 void draw::drawFunction(int x, int y, const string functionName) {
	printRect(x - 1, y - 2, functionName.length() + 4, 3, '-', '|');
	gotoxy(x, y);
	printf("%s", functionName.c_str());
}

 void draw::drawTitle(int x, int y, const string title) {
	gotoxy(x, y);
	printf("%s", title.c_str());
}

 void draw::clearWidget(int x, int y, int width, int height) {
	for (int i = x + 1; i < x + height - 1; i++) {
		gotoxy(i, y + 1);
		printf("%*c", width - 2, ' ');
	}
}

 void draw::printAddIntro(int x, int y) {
	drawTitle(x + 1, y + 1, "请输入要添加的歌曲信息:");
	drawTitle(x + 2, y + 2, "歌曲名    :");
	drawTitle(x + 3, y + 2, "歌手      :");
	drawTitle(x + 4, y + 2, "歌曲名缩写:");
}

 void draw::printSearchIntro(int x, int y) {
	drawTitle(x + 1, y + 1, "请选择查找方式");
	drawTitle(x + 3, y + 5, "按歌曲查找");
	drawTitle(x + 4, y + 5, "按歌手查找");
}

 void draw::printDeleteIntro(int x, int y) {

}

 void draw::clearRow(int x, int y, int width) {
	draw::gotoxy(x, y);
	printf("%*c", width, ' ');
}

 void draw::cerrWidgetBottom(int x, int y, int width, int height, string errInfo) {
	int start_x = x + height - 2;
	int start_y = y + width / 2 - 8;
	clearRow(start_x, start_y, 24);
	drawTitle(start_x, start_y, errInfo);
}

 void draw::printModifyInfo(int x, int y, Music & m) {
	drawTitle(x + 1, y + 1, "请修改歌曲信息:");
	gotoxy(x + 2, y + 2);
	printf("%s ->", m.name.c_str());
	gotoxy(x + 3, y + 2);
	printf("%s ->", m.singer.c_str());
	gotoxy(x + 4, y + 2);
	printf("%s ->", m.abb.c_str());
}

 void draw::printMarkInfo(int x, int y) {
	drawTitle(x - 1, y, "请给该歌曲打分:");
	drawTitle(x, y + 3, "☆☆☆☆☆");
}

 void draw::starHightlight(int x, int y) {
	gotoxy(x, y);
	printf("★");
}

 void draw::starHide(int x, int y) {
	gotoxy(x, y);
	printf("☆");
}

 void draw::printClientInfo(vector<string> adds) {
	for (int i = 0; i < 6; i++) {
		clearRow(i + 6, 86, 28);
	}
	int i = 0;
	for (auto& a : adds) {
		gotoxy(i + 6, 86);
		printf("用户%2d:%s", ++i, a.c_str());
	}
}
