#include "rest_data.h"

const std::string &RestData::getBody() const
{
	return body;
}

void RestData::setBody(const std::string &value)
{
	body = value;
}

const std::map<std::string, std::string> &RestData::getHeaders() const
{
	return headers;
}

void RestData::setHeaders(const std::map<std::string, std::string> &value)
{
	headers = value;
}

void RestData::addHeader(const std::string& header, const std::string & value)
{
	headers[header] = value;
}


const std::string &RestData::getMethod() const
{
	return method;
}

const std::string &RestData::getHttpVersion() const
{
	return http_version;
}

void RestData::setHttpVersion(const std::string &value)
{
	http_version = value;
}

void RestData::setMethod(const std::string & mt)
{
	method = mt;
}
