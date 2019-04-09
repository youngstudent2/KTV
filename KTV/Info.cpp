#include"Info.h"
Info::Info()
{
	tmpState = false;
}
int Info::loadFile(string fileName) {
	if (fileName.find(".txt") == string::npos)return -1;
	fstream file(fileName, ios::in);
	if (!file.is_open())return 0;
	Music music;
	bool exist;
	int count = 0;
	while (file >> music.ID >> music.name >> music.singer >> music.abb >> music.playstate >> music.score >> music.markNum) {
		exist = false;
		for (auto& m : musics) {
			if (m == music) {
				exist = true;//有重复
				break;
			}
		}
		if (exist)continue;
		music.ID = musics.size() + 1;
		count++;
		musics.push_back(music);
	}
	file.close();
	return count;
}

int Info::saveInfo(string fileName) {
	if (fileName.find('.') == string::npos)return 0;
	fstream file(fileName, ios::out);
	if (!file.is_open())return 0;
	for (auto &music : musics) {
		file << music.ID << '\t'
			<< music.name << '\t'
			<< music.singer << '\t'
			<< music.abb << '\t'
			<< music.playstate << '\t'
			<< music.score << '\t'
			<< music.markNum << '\n';
	}
	file.close();
	return 1;
}

int Info::add(Music m)
{
	if (m.ID == -1)return 0;
	for (auto& music : musics) {
		if (m == music && music.playstate)return 0;
	}
	musics.push_back(m);
	return 1;
}

int Info::musicSearch(string musicName)
{
	if (musicName.empty())return 0;
	tmpRes.clear();
	setTmpState(true);
	for (int i = 0; i < musics.size(); i++) {
		if (musics.at(i).name.find(musicName)!=string::npos || musics.at(i).abb.find(musicName) != string::npos) {
			tmpRes.push_back(musics.at(i));
		}
	}
	return tmpRes.size(); 
}

int Info::singerSearch(string singerName)
{
	if (singerName.empty())return 0;
	tmpRes.clear();
	setTmpState(true);
	for (int i = 0; i < musics.size(); i++) {
		if (musics.at(i).singer.find(singerName) != string::npos) {
			tmpRes.push_back(musics.at(i));
		}
	}
	return tmpRes.size();
}

int Info::insert(Music m, int index)
{
	musics.insert(musics.begin() + index, m);
	return 0;
}

Music Info::createMusic(string name, string singer, string abb)
{
	if (name.empty() || singer.empty() || abb.empty())return Music();
	int id = musics.size() + 1;
	return Music(name, singer, abb, id);
}

int Info::deleteMusic(int index)
{
	if (tmpState) {
		if (index<0 || index>tmpRes.size())return 0;
		int tmpID = tmpRes.at(index).ID;
		tmpRes.erase(tmpRes.begin() + index);
		for (auto& t : tmpRes) {
			if (t.ID > tmpID)t.ID--;
		}
		musics.erase(musics.begin() + tmpID - 1);
		for (vector<Music>::iterator i = musics.begin() + tmpID - 1; i != musics.end();i++) {
			i->ID--;			
		}
	}
	else {
		if (index<0 || index>=musics.size())return 0;
		for (vector<Music>::iterator i = musics.begin() + index + 1; i != musics.end(); i++) {
			i->ID--;
		}
		musics.erase(musics.begin() + index);
	}
	
	return 1;
}

int Info::eraseMusic(int index) {
	if (index < 0 || index >= musics.size())return 0;
	musics.erase(musics.begin() + index);
	return 1;
}

void Info::markMusic(int ID, double score)
{
	for (auto& m : musics) {
		if (m.ID == ID) {
			m.score =(double)(m.score*m.markNum + score) / (1 + m.markNum);
			m.markNum++;
			break;
		}
	}
}

Music Info::getMusic(int index)
{
	if (index<0 || index>musics_count() - 1) {
		Music m;
		return m;
	}		
	return tmpState ? tmpRes.at(index) : musics.at(index);
}

int Info::musics_count()
{
	return tmpState ? tmpRes.size() : musics.size();
}

void Info::tcpAssist(string MusicInfo)
{
	istringstream s(MusicInfo);
	Music m;
	while (s >> m.ID >> m.name >> m.singer >> m.abb >> m.playstate >> m.score >> m.markNum) {
		musics.push_back(m);
	}
}

void Info::clear()
{
	musics.clear();
}

void Info::setTmpState(bool state)
{
	tmpState = state;
}

void Info::rankMusics()
{
	tmpRes.clear();
	setTmpState(true);
	tmpRes.assign(musics.begin(), musics.end());
	sort(tmpRes.begin(), tmpRes.end(), [](const Music &m, const Music &n)->bool { return m.score > n.score; });
}

void Info::nextMusic(int & playMusic)
{
	//musics[playMusic].playstate = 0;
	musics[++playMusic].playstate = 0;
}

void Info::setMusicState(int index, int state)
{
	musics[index].playstate = state;
}

//评分人数

inline Music::Music(string n, string s, string a, int ID) {
	name = n;
	singer = s;
	abb = a;
	playstate = 1;
	this->ID = ID;
	score = 0.0;
	markNum = 0;
}

inline Music::Music() {
	ID = -1;
}
