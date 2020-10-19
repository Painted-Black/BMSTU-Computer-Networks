#pragma once

#include <iostream>
#include <map>
#include <vector>

class ServerStat
{
public:
	ServerStat();
	void update(std::string user, std::string page);
	int dump(std::string filename);
private:
	std::map<std::string, std::vector<std::string>> mStat;
};

