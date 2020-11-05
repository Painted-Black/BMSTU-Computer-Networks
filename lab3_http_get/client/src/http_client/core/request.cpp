#include "request.h"

Request::Request(const Url &u)
	: url(u)
{
}

const Url &Request::getUrl() const
{
	return url;
}
