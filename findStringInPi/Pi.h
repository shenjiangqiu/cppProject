#pragma once
#include<string>
#include<fstream>

class Pi
{
public:
	Pi() = delete;
	Pi(const std::string& filename) : in(filename) {}
	~Pi() {
		in.close();
	}
	Pi(const Pi&) = delete;
	Pi(Pi&& pi) :in(std::move(pi.in)){

	}
	Pi& operator =(Pi&) = delete;
	Pi& operator =(Pi && pi) {
		in.close();
		in = std::move(pi.in);
	}

	std::string get(size_t offSet, size_t length);
private:
	std::ifstream in;
	//std::string fileName;
};

