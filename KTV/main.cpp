#include<iostream>
#include"AdminLayout.h"
#include"ClientLayout.h"
#include"LoginLayout.h"

#pragma comment(lib,"ws2_32.lib")
using namespace std;
int main()
{
	system("mode 150,40");
	LoginLayout l;
	AdminLayout *admin;
	ClientLayout *client;
	int exec_result;
	while (1) {	
		exec_result = l.exec();
		if (exec_result == 0) {
			admin = new AdminLayout;
			if (admin->exec() == 1)
				break;
			delete admin;
		}
		else if (exec_result == 1) {
			client = new ClientLayout;
			if (client->exec() == 1)
				break;
			delete client;
		}
		system("cls");
	}
	return 0;
}