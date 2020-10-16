#include <sstream>
#include <algorithm>
#include <sys/wait.h>
#include "rest_server.h"
#include "request.h"

void RestServer::addRoute(std::unique_ptr<RestRoute> route)
{
	routes[route->getRoute()] = std::move(route);
}

void RestServer::run()
{
	using namespace std::placeholders;
	tcp_socket_server.setMaxConnection(6);
	tcp_socket_server.setListenLocal(false);
	tcp_socket_server.setPort(8889);
	std::function<void(const std::string&, int32_t)> socket_handler =
			std::bind(&RestServer::receivePackage, this, _1, _2);
	tcp_socket_server.setHandlerCallback(socket_handler);
	tcp_socket_server.listenSocket();
}

void RestServer::stop()
{
	tcp_socket_server.closeSock();
}

void RestServer::receivePackage(const std::string& data, int32_t socket)
{
	auto request = parsePackage(data);
	request.setRawDescriptor(socket);
	if (routes.count(request.getRoute()))
	{
		auto& handler_orig = routes[request.getRoute()]->getHandler();
		const auto& methods = handler_orig.getMethods();
		const std::string& request_method = request.getMethod();
		bool is_find = std::find(methods.cbegin(), methods.cend(), request_method) != methods.cend();
		if (is_find)
		{
		}
		else
		{
//			405 Method not allowed
		}
//		auto handler =
	}
	else
	{
		tcp_socket_server.write(socket, std::to_string(404));
		/*404 Not found*/
	}
}

RestHandler::Request RestServer::parsePackage(const std::string& data)
{
	enum PackagePart
	{
		None,
		RequestType,
		Header,
		Body
	};

	constexpr char TrimmedChars[] = "\t\r\n ";

	PackagePart current_part = None;
	std::string::size_type indexer = 0;
	RestHandler::Request request;
	std::stringstream bodybuffer;
	while (indexer != std::string::npos)
	{
		std::string::size_type last_indexer = data.find_first_of("\r\n", indexer);
		std::string boff = data.substr(indexer, last_indexer - indexer);
		if (current_part == None)
		{
			std::list<std::string> data = split(boff, " ");
			auto iter = data.begin();
			request.setMethod(*(iter++));
			request.setRoute(*(iter++));
			request.setHttpVersion(*(iter++));
			current_part = RequestType;
		}
		else if (current_part == RequestType || current_part == Header)
		{
			if (boff.empty())
			{
				current_part = Body;
			}
			else
			{
				std::string::size_type idx_1 = boff.find(":");
				request.addHeader(boff.substr(0, idx_1), trim(boff.substr(idx_1 + 1), TrimmedChars));
				current_part = Header;
			}
		}
		else
		{
			bodybuffer << boff;
		}

		indexer = last_indexer == std::string::npos ? last_indexer : last_indexer + 2;
	}
	request.setBody(bodybuffer.str());
	return request;
}

std::string RestServer::writeResponce(const RestHandler::Responce& responce)
{

}

std::list<std::string> RestServer::split(const std::string& text, const std::string& del)
{
	std::list<std::string> out_data;
	std::string::size_type indexer = 0;
	while (indexer != std::string::npos)
	{
		std::string::size_type last_indexer = text.find_first_of(del, indexer);
		out_data.push_back(text.substr(indexer, last_indexer - indexer));
		indexer = last_indexer == std::string::npos ? last_indexer : last_indexer + del.size();
	}

	return out_data;
}

std::string RestServer::trim(const std::string& string, const std::string& chars)
{
	std::string::size_type f_idx = string.find_first_not_of(chars);
	std::string::size_type s_idx = string.find_last_not_of(chars);
	return string.substr(f_idx, s_idx - f_idx + 1);
}
