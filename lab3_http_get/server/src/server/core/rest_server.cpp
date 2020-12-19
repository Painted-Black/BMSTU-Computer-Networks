#include <iostream>
#include <sstream>
#include <algorithm>
#include <sys/wait.h>
#include "responce.h"
#include "rest_server.h"
#include "request.h"

void RestServer::setStatistic(std::shared_ptr<Statistic> statistic)
{
	this->statistic = statistic;
}

void RestServer::addRoute(std::unique_ptr<RestRoute> route)
{
	routes[route->getRoute()] = std::move(route);
}

void RestServer::run()
{
	pool.reset(new ThreadPool(10));
	using namespace std::placeholders;
	tcp_socket_server.setMaxConnection(6);
	tcp_socket_server.setListenLocal(false);
	tcp_socket_server.setPort(8888);
//	std::function<void(const std::string&, const std::string&, int32_t)> socket_handler =
//			std::bind(&RestServer::receivePackage, this, _1, _2, _3);
	tcp_socket_server.setHandlerCallback([&](const std::string& data, const std::string& addr, int32_t socket)
	{
		// берем из пула свободный поток (если такого нет, встаем в очередь)
		pool->enqueue([&](std::string __data, std::string __addr, int32_t __socket)
		{
			std::cout << "Current thread id " << std::this_thread::get_id() << std::endl;
			receivePackage(__data, __addr, __socket);
		}, data, addr, socket);
	});
	tcp_socket_server.listenSocket();
}

void RestServer::stop()
{
	tcp_socket_server.stop();
}

void RestServer::receivePackage(const std::string& data, const std::string& addr, int32_t socket)
{
	auto request = parsePackage(data);
	request.setAddress(addr);
	if (routes.count(request.getRoute())) // если есть такой роут
	{
		auto handler_orig = routes[request.getRoute()]->getHandler().clone();
		const auto& methods = handler_orig->getMethods();
		const std::string& request_method = request.getMethod();
		bool is_find = std::find(methods.cbegin(), methods.cend(), request_method) != methods.cend();
		if (is_find)
		{
			RestHandler::Responce responce = handler_orig->receive(request);
			writeResponce(socket, responce);
			tcp_socket_server.closeConnection(socket);
		}
		else
		{
			RestHandler::Responce responce;
			responce.setStatusCode(RestHandler::Responce::MethodNotAllowed);
			writeResponce(socket, responce);
			tcp_socket_server.closeConnection(socket);
		}
	}
	else
	{
		RestHandler::Responce responce;
		responce.setStatusCode(RestHandler::Responce::NotFound);
		writeResponce(socket, responce);
		tcp_socket_server.closeConnection(socket);
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

		indexer = last_indexer == std::string::npos ? last_indexer : last_indexer + 2; // 2 потому что \r\n
	}
	request.setBody(bodybuffer.str());
	return request;
}

void RestServer::writeResponce(int32_t raw_socket, const RestHandler::Responce& responce)
{		
	constexpr char Line_Delimer[] = "\r\n";
	constexpr char Header_Delimer[] = ": ";
	constexpr char ContentLenghtHeader[] = "Content-Length";

	std::stringstream sstream;
	sstream << responce.getHttpVersion() << " " << responce.getStatus() << Line_Delimer;
	auto headers = responce.getHeaders();
	auto write_header_responce = [&](const std::pair<std::string, std::string>& header)
	{
		sstream << header.first << Header_Delimer << header.second << Line_Delimer;
	};

	const std::string& body_ref = responce.getBody();
	std::for_each(headers.cbegin(), headers.cend(), write_header_responce);
	if (headers.count(ContentLenghtHeader) == 0)
	{
		write_header_responce({ContentLenghtHeader, std::to_string(body_ref.size())});
	}
	sstream << Line_Delimer;
	tcp_socket_server.write(raw_socket, sstream.str());

	tcp_socket_server.write(raw_socket, body_ref);
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
