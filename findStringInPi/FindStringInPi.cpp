#include "FindStringInPi.h"

#include<vector>
#include<future>
#include<thread>
#include<chrono>
#include<iostream>
FindStringInPi::FindStringInPi(std::string fileNameOfPi, unsigned cores, size_t maxLength) :
	pi(fileNameOfPi),
	cores(cores>=1?cores:1),
	lengthPerTask(maxLength / this->cores)
{
}

FindStringInPi::~FindStringInPi()
{
}
size_t find(const std::string& patten, const std::string& source, size_t pos) {
	std::cout<<"start to find the patten ,which source length is "<<source.size()<<\
	"\n and the patten's length is "<<patten.size()<<std::endl;
	auto ret= source.find(patten, pos);
	std::cout<<"end find "<<std::endl;
	return ret;
}
size_t FindStringInPi::run(const std::string& patten)
{
	std::cout<<"start to run()"<<std::endl;
	std::cout<<"the cores is "<<cores<<" and the task length is "<<lengthPerTask<<std::endl;

	auto length = patten.size();
	std::vector<std::string> strings;
	std::vector<std::string> str_mid;
	
	if (length > 1) {
		std::cout<<"start to build limb str"<<std::endl;
		int *pos=new int[cores];
		for (unsigned i = 1; i < cores; i++) {
			pos[i-1] = i * lengthPerTask - length + 1;
			if (pos[i-1] < 0) pos[i-1] = 0;
			str_mid.emplace_back(pi.get(pos[i-1], length + length - 2));
		}
		std::cout<<"end build limb str"<<std::endl;
		unsigned index = 0;
		std::cout<<"start to figure limb"<<std::endl;
		for (auto i : str_mid) {
			auto ret = i.find(patten);
			if (ret != std::string::npos) {
				return pos[index] + ret;
			}
			index++;
		}
		std::cout<<"end figure limb"<<std::endl;
	}
	
	if (length <= lengthPerTask) {
		std::cout<<"start to build muti task str"<<std::endl;
		for (unsigned i = 0; i < cores; i++) {
			strings.emplace_back(pi.get(i*lengthPerTask, lengthPerTask));
		}
		std::cout<<"end build muti task "<<std::endl;
	}
	else {
		std::cout<<"the str is too long"<<std::endl;
		return std::string::npos;
	}

	std::vector<std::future<size_t> > futures;
	std::cout<<"start to build futures"<<std::endl;
	if (length <= lengthPerTask) {
		for (unsigned i = 0; i < cores; i++) {
			futures.push_back(std::async(std::launch::async, find, patten, strings[i], 0));
		}
	}
	std::cout<<"end build futures"<<std::endl;
	bool finded = false;
	int who = 0;
	std::vector<bool> mask(cores, false);//false����δ���
	bool finished = false;
	size_t position;
	using std::cout;
	using std::endl;
	cout<<"start to wait for anser"<<endl;
	while (!finded) {
		finished = true;
		for (unsigned i = 0; i < cores; i++) {
			if (mask[i]==true) {
				continue;
			}
			else {
				finished = false;
			}
			auto ret1=futures[i].wait_for(std::chrono::seconds(0));
			if (ret1 == std::future_status::ready) {
				cout<<"the thread "<<i<<" completed"<<endl;
				auto ret2 = futures[i].get();
				if (ret2 != std::string::npos) {
					cout<<"find !!"<<endl;
					finded = true;
					who = i;
					position = ret2;
					break;
				}
				else {
					cout<<"cant find,and set mask to true"<<endl;
					mask[i] = true;
				}
			}else{
				cout<<"the thread "<<i<<" are not ready"<<endl;
			}
			
		}
		if (finished == true) {
			break;
		}
		std::this_thread::sleep_for(std::chrono::seconds(2));
	}
	if (finded == true) {
		return who * lengthPerTask + position;
	}
	else {
		return -1;
	}

}



