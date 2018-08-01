// future.cpp: 定义控制台应用程序的入口点。
//

//#include "stdafx.h"

#include<future>
#include<iostream>
#include<random>
#include<chrono>
#include<memory>
#include<numeric>
using namespace std;
template<typename RandonIt>
unsigned long long add(RandonIt t1, RandonIt t2) {
	unsigned long long ret = 0;
	for (; t1 != t2; t1++) {
		ret += *t1;
	}
	return ret;
}
template<typename RandonIt>
unsigned long long  pallAdd(RandonIt t1, RandonIt t2,const unsigned long long  all) {
	auto gap = t2 - t1;
	if (gap<=all/4) {
		return add(t1, t2);
	}
	else {
		auto mid = t1 + (t2 - t1) / 2;
		auto ft = async(launch::async, pallAdd<RandonIt>, t1, mid,all);
		return pallAdd(mid, t2,all) + ft.get();
	}
}

int main(int argc, char const *argv[])
{
	const unsigned long long N = 0x7fffffff;
	auto engin = default_random_engine(chrono::system_clock::now().time_since_epoch().count());
	uniform_int_distribution<int> eg(1, 10);
	char *buffer = new char[N];
	for (int i = 0; i<(N); i++) {
		buffer[i] = eg(engin);
	}
	auto start = chrono::system_clock::now();
	auto anser1 = add(buffer, buffer + (N));
	auto end = chrono::system_clock::now();
	auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);
	cout << "ret = " << anser1 << ", time= " << duration.count() << " miliseconds" << endl;

	start = chrono::system_clock::now();
	auto anser2 = pallAdd(buffer, buffer + (N),N);
	end = chrono::system_clock::now();
	duration = chrono::duration_cast<chrono::milliseconds>(end - start);
	cout << "ret = " << anser2 << ", time= " << duration.count() << " miliseconds" << endl;

	start = chrono::system_clock::now();
	anser2 = pallAdd(buffer, buffer + (N),N);
	end = chrono::system_clock::now();
	duration = chrono::duration_cast<chrono::milliseconds>(end - start);
	cout << "ret = " << anser2 << ", time= " << duration.count() << " miliseconds" << endl;

	return 0;
}

