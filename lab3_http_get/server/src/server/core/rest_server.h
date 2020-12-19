#pragma once

#include <memory>
#include <string>
#include <map>
#include <list>
#include "rest_route.h"
#include "server.h"
#include "statistic/statistic.h"
#include "thread_pool.h"

class ThreadPool;

class RestServer final
{
public:
	void setStatistic(std::shared_ptr<Statistic>); // unused
	void addRoute(std::unique_ptr<RestRoute>);
	void run();
	void stop();

private:
	void receivePackage(const std::string&, const std::string &addr, int32_t);
	RestHandler::Request parsePackage(const std::string&);
	void writeResponce(int32_t, const RestHandler::Responce&);
	std::list<std::string> split(const std::string&, const std::string&);
	std::string trim(const std::string&, const std::string&);

private:
	std::map<std::string, std::unique_ptr<RestRoute>> routes;
	TcpServer tcp_socket_server;
	std::unique_ptr<ThreadPool> pool;
	std::shared_ptr<Statistic> statistic;
};

