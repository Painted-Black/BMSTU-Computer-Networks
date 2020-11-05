#include "statistic.h"

Statistic::Statistic(uint64_t size)
	: max_count(size)
{
}

Statistic::~Statistic() noexcept
{
}

void Statistic::receiveEvent(const Statistic::Event & ev)
{
	std::lock_guard<std::mutex> lock(mutex);
	if (events.size() == max_count)
	{
		events.pop_front();
	}

	events.push_back(ev);
}

void Statistic::build()
{
	std::lock_guard<std::mutex> lock(mutex);
	buildStart();
	for (const auto& event : events)
	{
		buildEvent(event);
	}
	buildFinish();
}

const std::string &Statistic::getBuildedData() const
{
	return build_data;
}

void Statistic::setBuildData(const std::string & d)
{
	build_data = d;
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

const std::string& Statistic::Event::getName() const
{
    return name;
}

std::tm *Statistic::Event::getCreateDate() const
{
    return create_date;
}
