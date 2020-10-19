#include "server_stat.h"

#include <utility>

ServerStat::ServerStat()
{

}

void ServerStat::update(std::string user, std::string page)
{
	if (mStat.count(user) == 0)
	{
		std::vector<std::string> vec;
		vec.push_back(page);
		mStat[user] = vec;
	}
}

int ServerStat::dump(std::string filename)
{

}
