#pragma once

#include <string>

class Url final
{
public:
	Url(const std::string&);
	const std::string& getSchema() const;
	const std::string& getHost() const;
	const std::string& getPath() const;
	const std::string& getQuery() const;
	int16_t getPort() const;

	void setSchema(const std::string &value);
	void setHost(const std::string &value);
	void setPort(const int16_t &value);
	void setPath(const std::string &value);
	void setQuery(const std::string &value);

private:
	std::string schema;
	std::string host;
	int16_t port = -1;
	std::string path = "/";
	std::string query;
};

