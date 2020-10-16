#pragma once

#include <memory>
#include <string>
#include "rest_handler.h"

class RestRoute final
{
public:
	RestRoute(const std::string&, std::unique_ptr<RestHandler>);
	const std::string& getRoute() const;
	RestHandler& getHandler();
private:
	std::string route;
	std::unique_ptr<RestHandler> handler;
};

