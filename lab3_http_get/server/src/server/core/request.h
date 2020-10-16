#pragma once

#include <map>
#include "rest_data.h"
#include "rest_handler.h"

class RestHandler::Request : public RestData
{
public:
	struct QueryArg
	{
		std::string key;
		std::string val;
	};
public:
	const std::list<QueryArg>& getQueryArgs() const;
	void setRoute(const std::string&);
	const std::string &getRoute() const;
private:
	std::string route;
	std::list<QueryArg> input_arguments;
};

