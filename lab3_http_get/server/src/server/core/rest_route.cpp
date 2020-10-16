#include "rest_route.h"

RestRoute::RestRoute(const std::string& rt, std::unique_ptr<RestHandler> hd)
	: route(rt), handler(std::move(hd))
{
}

const std::string &RestRoute::getRoute() const
{
	return route;
}

RestHandler &RestRoute::getHandler()
{
	return *handler;
}
