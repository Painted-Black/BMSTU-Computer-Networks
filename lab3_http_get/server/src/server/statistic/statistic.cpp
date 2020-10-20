#include "statistic.h"

Statistic::Statistic()
{

}

Statistic::~Statistic() noexcept
{
}

void Statistic::build()
{
	for (const auto& event : events)
	{
		receiveEvent(event);
	}
}

Statistic::Event::Event(const std::string& ev_name)
{
	time_t now = time(nullptr);
	create_date = localtime(&now);
	name = ev_name;
}

void Statistic::Event::setUserData(const std::string & key, const std::string & value)
{
	user_data[key] = value;
}

const std::string& Statistic::Event::getUserData(const std::string & key) const
{
	return user_data.at(key);
}

std::list<std::string> Statistic::Event::getUserKeys() const
{
	std::list<std::string> out_keys;
	for (auto iter = user_data.begin(); iter != user_data.end(); ++iter)
	{
		out_keys.push_back(iter->first);
	}
	return out_keys;
}
