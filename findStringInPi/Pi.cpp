#include "Pi.h"




std::string Pi::get( size_t offSet,size_t length)
{
	if ((int)offSet < 0) {
		offSet = 0;
	}
	in.seekg(offSet);
	char *buffer = new char[length + 1];
	std::fill_n(buffer, length + 1, 0);
	in.read(buffer, length);
	std::string ret(buffer);
	delete[] buffer;
	return ret;
}
