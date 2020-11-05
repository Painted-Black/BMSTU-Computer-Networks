#include <sstream>
#include "url.h"

Url::Url(const std::string & url)
{
	std::stringstream stream(url);
	std::getline(stream, schema, ':');
	stream.ignore(2);
	if (stream.str().find(':') != std::string::npos)
	{
		std::getline(stream, host, ':');
		stream >> port;
	}
	else
	{
		std::getline(stream, host, '/');
	}
	std::getline(stream, path, '?');
	std::getline(stream, query);
}

const std::string &Url::getSchema() const
{
	return schema;
}

void Url::setSchema(const std::string &value)
{
	schema = value;
}

const std::string &Url::getHost() const
{
	return host;
}

void Url::setHost(const std::string &value)
{
	host = value;
}

int16_t Url::getPort() const
{
	return port;
}

void Url::setPort(const int16_t &value)
{
	port = value;
}

const std::string &Url::getPath() const
{
	return path;
}

void Url::setPath(const std::string &value)
{
	path = value;
}

const std::string &Url::getQuery() const
{
	return query;
}

void Url::setQuery(const std::string &value)
{
	query = "?" + value;
}
