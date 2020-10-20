#pragma once

#include <map>
#include <list>
#include <string>
#include <ctime>

class Statistic
{
public:
	class Event;
public:
	Statistic();
	virtual ~Statistic() noexcept;
	void build();
protected:
	virtual void receiveEvent(const Event&) = 0;
private:
	std::list<Event> events;
};

class Statistic::Event
{
public:
	Event(const std::string &name);
	void setUserData(const std::string&, const std::string&);
	const std::string &getUserData(const std::string&) const;
	std::list<std::string> getUserKeys() const;
private:
	std::tm* create_date;
	std::string name;
	std::map<std::string, std::string> user_data;
};
