#include <iostream>
#include <sys/wait.h>
#include "core/responce.h"
#include "core/rest_server.h"
#include <handler/handler_statistic.h>
#include <handler/html_file_handler.h>
#include "statistic/statistic_server.h"

////class A : public RestHandler
////{
////public:
////	A(const std::list<std::string>& k) : RestHandler(k) {}
////	virtual ~A(){}
////	void receive() override
////	{
////		auto& responce = getResponce();
////		responce.setStatusCode(Responce::Ok);
////		responce.setBody("Tests");
////		responce.addHeader("Content-Type", "text/html");
////	}

////	std::unique_ptr<RestHandler> clone() override
////	{
////		return std::unique_ptr<RestHandler>(new A(getMethods()));
////	}
////};

////class B : public RestHandler
////{
////public:
////	B(const std::list<std::string>& k) : RestHandler(k) {}
////	virtual ~B(){}
////	void receive() override
////	{
////		auto& responce = getResponce();
////		responce.setStatusCode(Responce::Ok);
////		responce.setBody("Hellos");
////		responce.addHeader("Content-Type", "text/html");
////	}

//	std::unique_ptr<RestHandler> clone() override
//	{
//		return std::unique_ptr<RestHandler>(new B(getMethods()));
//	}
//};

static RestServer server;

__attribute__((noreturn)) void sigintCatcher(int signum)
{
	printf( "\nProccess Catched signal #%d\n", signum);
	server.stop();
	printf("Server stopped\n");
	exit(0);
}

int main()
{
	signal(SIGINT, sigintCatcher);
	signal(SIGABRT, sigintCatcher);
	constexpr uint64_t count_buffer = 5000;
	std::shared_ptr<Statistic> statistic(new StatisticServer(count_buffer));
	server.addRoute(
				std::make_unique<RestRoute>("/statistic", std::unique_ptr<RestHandler>(new HandlerStatistic(statistic))));
	server.addRoute(std::make_unique<RestRoute>("/html", std::unique_ptr<RestHandler>(new HtmlFileHandler(statistic))));

//	std::list<std::string> method = {"GET"};
//	server.addRoute(std::make_unique<RestRoute>("/tests", std::unique_ptr<RestHandler>(new A(method))));
//	server.addRoute(std::make_unique<RestRoute>("/hellos", std::unique_ptr<RestHandler>(new B(method))));
	server.run();
	return 0;
}
