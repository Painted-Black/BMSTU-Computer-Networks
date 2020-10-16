#pragma once

#include <memory>
#include <string>
#include <list>

class RestHandler
{
public:
	class Request;
	class Responce;

	enum TypeLoading
	{
		LoadMore,
		Stop
	};
public:
	RestHandler(const std::list<std::string>&);
	virtual ~RestHandler();
	const Responce &receive(const Request &);
	virtual TypeLoading loadingDataType() const;
	virtual std::unique_ptr<RestHandler> clone() = 0;
	const std::list<std::string>& getMethods() const;

protected:
	virtual void receive();
	const Request& getRequest() const;
	Responce& getResponce();

private:
	std::list<std::string> methods;
	std::unique_ptr<Request> request;
	std::unique_ptr<Responce> responce;
};

