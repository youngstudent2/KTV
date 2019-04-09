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
	string name, singer, abb;//歌曲名 歌手名 缩写
	bool playstate;//播放状态
	int ID;//编号
	float score;//评分
	int markNum;//评分人数
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
	int add(Music m);//添加歌曲
	int musicSearch(string MusicName);//按歌名搜索
	int singerSearch(string singerName);//按歌手搜索
	int insert(Music m, int index);//插入歌曲(用于置顶
	Music createMusic(string name, string singer, string abb);
	int deleteMusic(int index);//有检查删除歌曲
	Music getMusic(int index);
	int musics_count();
	void tcpAssist(string musicInfo);//辅助解析
	void clear();//清空曲库
	void setTmpState(bool state);//设置临时状态（用于显示搜索结果或排序结果
	void rankMusics();//歌曲按评分排序
	void nextMusic(int &playMusic);//切歌
	void setMusicState(int index, int state);//设置歌曲播放状态
	int eraseMusic(int index);//无检查删除歌曲
	void markMusic(int ID, double score);//根据ID进行歌曲评分
private:
	vector<Music> musics;
	vector<Music> tmpRes;
	bool tmpState;
};

#endif // ! INFO_H

