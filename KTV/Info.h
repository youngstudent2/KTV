#ifndef  INFO_H
#define INFO_H
#include<fstream>
#include<string>
#include<vector>
#include<iomanip>
#include<algorithm>
#include<sstream>
using namespace std;
struct Music {
	string name, singer, abb;//������ ������ ��д
	bool playstate;//����״̬
	int ID;//���
	float score;//����
	int markNum;//��������
	Music(string n, string s, string a, int ID);
	Music();
	bool operator ==(Music m) {
		return m.name == name && m.singer == singer;
	}
};

class Info {
public:
	Info();
	int loadFile(string fileName);
	int saveInfo(string fileName);
	int add(Music m);//��Ӹ���
	int musicSearch(string MusicName);//����������
	int singerSearch(string singerName);//����������
	int insert(Music m, int index);//�������(�����ö�
	Music createMusic(string name, string singer, string abb);
	int deleteMusic(int index);//�м��ɾ������
	Music getMusic(int index);
	int musics_count();
	void tcpAssist(string musicInfo);//��������
	void clear();//�������
	void setTmpState(bool state);//������ʱ״̬��������ʾ���������������
	void rankMusics();//��������������
	void nextMusic(int &playMusic);//�и�
	void setMusicState(int index, int state);//���ø�������״̬
	int eraseMusic(int index);//�޼��ɾ������
	void markMusic(int ID, double score);//����ID���и�������
private:
	vector<Music> musics;
	vector<Music> tmpRes;
	bool tmpState;
};

#endif // ! INFO_H

