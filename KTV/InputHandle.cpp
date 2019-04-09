#include "InputHandle.h"

//±£´æ

 int InputHandle::getKeyInput() {
	int ch = _getch();
	if (ch == 224) {
		ch = _getch();
		switch (ch)
		{
		case 72:return KEY_UP;
		case 80:return KEY_DOWN;
		case 75:return KEY_LEFT;
		case 77:return KEY_RIGHT;
		}
	}
	else {
		//Debug:
		_getch();
		//release:

		switch (ch)
		{
		case 'w':return KEY_W;
		case 's':return KEY_S;
		case 'q':return KEY_Q;
		case 'e':return KEY_E;
		case '\r':return KEY_ENTER;
		case 27:return ESC;
		case 26:return CTRL_Z;
		case 20:return CTRL_T;
		case 19:return CTRL_S;
		case 14:return CTRL_N;
		case 6:return CTRL_F;
		case 4:return CTRL_D;
		}
	}
}

 string InputHandle::getString(int x, int y) {
	draw::gotoxy(x, y);
	char s[100];
	int i;
	for (i = -1; s[i++] != '\n';) {
		s[i] = getchar();
	}
	s[i - 1] = 0;
	draw::gotoxy(0, 0);
	return string(s);
}
