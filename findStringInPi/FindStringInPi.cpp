#include "FindStringInPi.h"

#include<vector>
#include<future>
#include<thread>
#include<chrono>
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
	return source.find(patten, pos);
}
size_t FindStringInPi::run(const std::string& patten)
{

	auto length = patten.size();
	std::vector<std::string> strings;
	std::vector<std::string> str_mid;
	
	if (length > 1) {
		int *pos=new int[cores];
		for (unsigned i = 1; i < cores; i++) {
			pos[i-1] = i * lengthPerTask - length + 1;
			if (pos[i-1] < 0) pos[i-1] = 0;
			str_mid.emplace_back(pi.get(pos[i-1], length + length - 2));
		}
		unsigned index = 0;
		for (auto i : str_mid) {
			auto ret = i.find(patten);
			if (ret != std::string::npos) {
				return pos[index] + ret;
			}
			index++;
		}
	}
	if (length <= lengthPerTask) {
		for (unsigned i = 0; i < cores; i++) {
			strings.emplace_back(pi.get(i*lengthPerTask, lengthPerTask));
		}
	}
	else {
		return std::string::npos;
	}

	std::vector<std::future<size_t> > futures;
	if (length <= lengthPerTask) {
		for (unsigned i = 0; i < cores; i++) {
			futures.push_back(std::async(std::launch::async, find, patten, strings[i], 0));
		}
	}
	
	bool finded = false;
	int who = 0;
	std::vector<bool> mask(cores, false);//false代表未完成
	bool finished = false;
	size_t position;
	
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
				auto ret2 = futures[i].get();
				if (ret2 != std::string::npos) {
					finded = true;
					who = i;
					position = ret2;
					break;
				}
				else {
					mask[i] = true;
				}
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



