#include "rest_handler.h"
#include "responce.h"
#include "request.h"

RestHandler::RestHandler(const std::list<std::string>& mt)
	: methods(mt)
{
}

RestHandler::~RestHandler()
{
}

const RestHandler::Responce& RestHandler::receive(const Request& data)
{
	responce.reset(new Responce());
	request.reset(new Request(data));
	receive();
	return *responce;
}

RestHandler::TypeLoading RestHandler::loadingDataType() const
{
	return RestHandler::LoadMore;
}

void RestHandler::receive()
{
}

const RestHandler::Request &RestHandler::getRequest() const
{
	return *request;
}

RestHandler::Responce &RestHandler::getResponce()
{
	return *responce;
}

const std::list<std::string> &RestHandler::getMethods() const
{
    return methods;
}
