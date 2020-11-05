#pragma once

#include <map>
#include <set>
#include "statistic.h"

class StatisticServer : public Statistic
{
private:
	struct Data
	{
		/* host { count : <>, routes : [] }*/
		std::map<std::string, std::tuple<uint64_t, std::set<std::string>>> host_data;
	};

	Data event_data;
public:
	StatisticServer(uint64_t);
private:
	void buildEvent(const Event &) override;
	void buildStart() override;
	void buildFinish() override;
};

