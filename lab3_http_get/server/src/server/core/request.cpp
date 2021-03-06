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

const std::string& RestHandler::Request::getAddress() const
{
    return address;
}

void RestHandler::Request::setAddress(const std::string &value)
{
    address = value;
}
