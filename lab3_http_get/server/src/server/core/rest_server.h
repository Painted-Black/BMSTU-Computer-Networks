#pragma once

#include <memory>
#include <string>
#include <map>
#include <list>
#include "rest_route.h"
#include "server.h"

class RestServer final
{
public:
	void addRoute(std::unique_ptr<RestRoute>);
	void run();
	void stop();

private:
	void receivePackage(const std::string&, int32_t);
	RestHandler::Request parsePackage(const std::string&);
	std::string writeResponce(const RestHandler::Responce&);
	std::list<std::string> split(const std::string&, const std::string&);
	std::string trim(const std::string&, const std::string&);

private:
	std::map<std::string, std::unique_ptr<RestRoute>> routes;
	TcpServer tcp_socket_server;
};

