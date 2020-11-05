#pragma once

#include <string>
#include <map>

class RestData
{

public:
	const std::string& getBody() const;
	void setBody(const std::string &);
	void setHttpVersion(const std::string&);
	void setHeaders(const std::map<std::string, std::string> &);
	void addHeader(const std::string&, const std::string&);

	const std::map<std::string, std::string>& getHeaders() const;
	const std::string& getHttpVersion() const;
	const std::string& getRoute() const;

private:
	std::string http_version = "HTTP1.1";
	std::string body;
	std::map<std::string, std::string> headers;
};

