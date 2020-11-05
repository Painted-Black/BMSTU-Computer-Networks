#pragma once

#include <mutex>
#include <map>
#include <list>
#include <string>
#include <ctime>

class Statistic
{
public:
	class Event;
public:
	Statistic(uint64_t size);
	virtual ~Statistic() noexcept;
	void receiveEvent(const Event&);
	void build();
	const std::string& getBuildedData() const;
protected:
	virtual void buildStart() = 0;
	virtual void buildEvent(const Event&) = 0;
	virtual void buildFinish() = 0;
	void setBuildData(const std::string&);
private:
	std::mutex mutex;
	std::list<Event> events;
	std::string build_data;
	uint64_t max_count;
};

class Statistic::Event
{
public:
	Event(const std::string &name);
	void setUserData(const std::string&, const std::string&);
	const std::string &getUserData(const std::string&) const;
	std::list<std::string> getUserKeys() const;
	std::tm* getCreateDate() const;
	const std::string& getName() const;

private:
	std::tm* create_date;
	std::string name;
	std::map<std::string, std::string> user_data;
};
