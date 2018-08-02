#pragma once
#include"Pi.h"
class FindStringInPi
{
public:
	FindStringInPi(std::string fileNameOfPi,unsigned cores=4,size_t maxLenght=100000000u);
	~FindStringInPi();
	size_t run(const std::string & patten);
	
private:
	
	Pi pi;
	unsigned cores;
	size_t lengthPerTask;
};

