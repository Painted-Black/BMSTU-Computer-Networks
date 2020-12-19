#pragma once

#include <memory>
#include <string>
#include <list>

class RestHandler
{
public:
	class Request;
	class Responce;

	enum TypeLoading // unused
	{
		LoadMore,
		Stop
	};
public:
	RestHandler(const std::list<std::string>&);
	virtual ~RestHandler();
	const Responce &receive(const Request &);
	virtual TypeLoading loadingDataType() const;
	virtual std::unique_ptr<RestHandler> clone() = 0; // для unique_ptr, одно соединение -- один клон
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

