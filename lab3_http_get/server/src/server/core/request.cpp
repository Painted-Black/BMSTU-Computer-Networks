#include "request.h"

const std::list<RestHandler::Request::QueryArg> &RestHandler::Request::getQueryArgs() const
{
	return input_arguments;
}

void RestHandler::Request::setRoute(const std::string &value)
{
    route = value;
}


const std::string& RestHandler::Request::getRoute() const
{
	return route;
}
