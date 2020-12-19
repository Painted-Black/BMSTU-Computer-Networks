#include <iostream>
#include <sys/wait.h>
#include "core/responce.h"
#include "core/rest_server.h"
#include <handler/handler_statistic.h>
#include <handler/html_file_handler.h>
#include "statistic/statistic_server.h"

static RestServer server;

__attribute__((noreturn)) void sigintCatcher(int signum)
{
	printf( "\nProccess Catched signal #%d\n", signum);
	server.stop();
	printf("Server stopped\n");
	exit(0);
}

// for i in {1..100}; do curl localhost:8888/html; done
// Проверка, подряд 100 соединений
int main()
{
	signal(SIGINT, sigintCatcher);
	signal(SIGABRT, sigintCatcher);
	constexpr uint64_t count_buffer = 5000;
	std::shared_ptr<Statistic> statistic(new StatisticServer(count_buffer));
	server.addRoute(
				std::make_unique<RestRoute>("/statistic", std::unique_ptr<RestHandler>(new HandlerStatistic(statistic))));
	server.addRoute(std::make_unique<RestRoute>("/html", std::unique_ptr<RestHandler>(new HtmlFileHandler(statistic))));

	server.run();
	return 0;
}
