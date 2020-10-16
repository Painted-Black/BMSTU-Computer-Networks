#include <iostream>
#include "core/rest_server.h"

class A : public RestHandler
{
public:
	A(const std::list<std::string>& k) : RestHandler(k) {}
	virtual ~A(){}

	std::unique_ptr<RestHandler> clone() override
	{
		return std::unique_ptr<RestHandler>(new A(getMethods()));
	}
};

int main()
{
	RestServer server;
	std::list<std::string> method = {"GET"};
	server.addRoute(std::make_unique<RestRoute>("/test", std::unique_ptr<RestHandler>(new A(method))));
	server.run();
//	TcpServer socket;
//	socket.setMaxConnection(5);
//	socket.setListenLocal(false);
//	socket.setPort(8888);
//	socket.setHandlerCallback([](const std::string& data)
//	{
//		std::cout << data << std::endl;
//	});
//	socket.listenSocket();
	return 0;
}
