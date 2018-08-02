#include"FindStringInPi.h"
#include<iostream>
#include<sstream>
#include<chrono>
using namespace std;

int main(int argc,char** argv) {
	string pifilename;
	if (argc != 2) {
		cout << "must input the pi file name" << endl;
		return -1;
	}
	else {
		pifilename = argv[1];
	}
	cout << "\ninput threads: ";
	int threads;
	cin >> threads;
	cout << "\ninput the max length to find: ";
	int maxlength;
	cin >> maxlength;
	FindStringInPi my_finder(pifilename,threads,maxlength);
	cout << "imput the number you want to find" << endl;
	string patten;
	cin >> patten;
	auto timestart = std::chrono::steady_clock::now();
	auto ret=my_finder.run(patten);
	auto timeend = std::chrono::steady_clock::now();
	auto duration = timeend - timestart;
	cout << "cost " << std::chrono::duration_cast<std::chrono::minutes>(duration).count() << " minutes\n" << endl;
	if (ret == -1) {
		cout << "can‘t find " << patten << endl;
		return -1;

	}
	else {
		cout << "finded! the pos is " << ret << endl;
	}
	
	return 0;
}