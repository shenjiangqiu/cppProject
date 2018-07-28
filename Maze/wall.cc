// wall.cpp: 定义控制台应用程序的入口点。
//

//#include "stdafx.h"
#include<map>
#include<string>
#include<fstream>
#include<iostream>
#include<sstream>
#include<stdexcept>
#include<utility>
#include<vector>
using namespace std;


using position = vector<int>;
using connect = vector<position>;
using connectMap = map<connect, bool>;


istream & operator>>(istream & in, position& pos) {
	in >> pos[0];
	char a;
	in >> a;
	in >> pos[1];
	return in;
}

istream & operator >> (istream & in, connect& cn) {
	in >> cn[0];
	in >> cn[1];
	return in;
}

//pure(确保0,0 0,1 和 0,1 0,0等价)
connect pure(connect c) {
	if (c[0] > c[1]) {
		return connect({ c[1],c[0] });
	}
	else {
		return c;
	}
}
using namespace std;
//class 迷宫
class Maze {
public:
	Maze(string command) {
		if (!check(command)) throw runtime_error("invalid command");
		stringstream a(command);
		a >> width;
		a >> height;
		connect con({ { 0,0 }, { 0,0 } });
		while (a >> con) {
			myMap[pure(con)] = true;
			char temp;
			a >> temp;
		}
	}
	ostream& print(ostream &out) const{

		//首先输出第一行
		out << "[W]";
		for (int i = 0; i < width * 2; i++)
			out << string(" [W]");
		out << endl;

		//书中中间2*height-1行
		for (int i = 0; i < height*2-1; i++) {
			//每一行第一个是墙壁
			out << "[W]";

			//对于偶数行
			if (i % 2 == 0) {
				//输出中间2 * width - 1列
				for (int j = 0; j < 2 * width - 1; j++) {
					if (j % 2 == 0) {
						//迷宫房间中心
						out << " [R]";
					}
					else {
						//迷宫房间边缘，判断和右边的连通性
						out << ((myMap.find(pure(connect({ {i / 2,j / 2},{i / 2,j / 2 + 1} }))) != myMap.end()) ? " [R]" : " [W]");
					}
				}
			}//对于奇数行
			else {
				for (int j = 0; j < 2 * width - 1; j++) {
					//对于偶数列，判断此房间和下面房间的关系
					if (j % 2 == 0) {
						out << ((myMap.find(pure(connect({ { i / 2,j / 2 },{ i / 2+1,j / 2 } }))) != myMap.end()) ? " [R]" : " [W]");
					}//墙壁
					else {
						out << " [W]";
					}
				}
			}
			out << " [W]" << endl;
		}
		out << "[W]";
		for (int i = 0; i < width * 2; i++)
			out << string(" [W]");

		return out;
		
	}
	friend  ostream & operator <<(ostream &out, const Maze& mz);

private:
	bool check(string cmd) { return true; }//检查是否正确
	int width;
	int height;
	connectMap myMap;
};
ostream &operator <<(ostream &out, const Maze& mz) {
	return mz.print(out);
	
}
int main()
{
	ifstream in("command.txt");
	char* buffer = new char[1000];
	string cmd;
	while (in.read(buffer, 1000))
	{
		cmd += buffer;
	}
	cmd = "3 3\n0,0 0,1;1,1 1,2";
	Maze mMaze(cmd);
	cout<<mMaze<< endl;
    return 0;
}

