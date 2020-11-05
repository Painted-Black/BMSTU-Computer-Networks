#include <chrono>
#include <sstream>
#include <fstream>
#include "statistic_server.h"

using namespace std::chrono;

StatisticServer::StatisticServer(uint64_t size)
	: Statistic(size)
{

}

void StatisticServer::buildEvent(const Statistic::Event & ev)
{
	time_t now = time(nullptr);
	tm* current_date = localtime(&now);
	tm* event_date = ev.getCreateDate();
	auto sec = difftime(mktime(current_date), mktime(event_date));
	auto hour = duration_cast<hours>(seconds(static_cast<uint64_t>(sec))).count();

	const auto& host = ev.getUserData("user");
	const auto& route = ev.getUserData("route");

	if (hour == 0)
	{
		auto& data = event_data.host_data[host];
		++std::get<0>(data);
		std::get<1>(data).insert(route);
	}
}

void StatisticServer::buildStart()
{
	event_data.host_data.clear();
}

void StatisticServer::buildFinish()
{
	std::stringstream json_stream;
	json_stream << "{ \"last_hour\":{ \"hosts\":[";

	bool is_first = true;
	for (const auto& data : event_data.host_data)
	{
		if (is_first)
		{
			is_first = false;
		}
		else
		{
			json_stream << ',';
		}

		auto count = std::get<0>(data.second);
		json_stream
				<< "{ \"host\": \"" << data.first
					<< "\", \"count\":" << count << ", \"routes\": [";
		auto routes = std::get<1>(data.second);
		bool is_first_local = true;
		for (const auto& route : routes)
		{
			if (is_first_local)
			{
				is_first_local = false;
			}
			else
			{
				json_stream << ',';
			}

			json_stream << '"' << route << '"';
		}
		json_stream << "]}";
	}
	json_stream << "]}}";

	setBuildData(json_stream.str());
}
