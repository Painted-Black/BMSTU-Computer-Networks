#include <map>
#include <mutex>
#include <utility>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <algorithm>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h> // inet_ntoa
#include "server.h"

class TcpServer::ConnectionList
{
public:
	using reverse_iterator = std::map<int32_t, std::string>::reverse_iterator;
public:
	void insert(int32_t val, const std::string&);
	void remove(int32_t val);
	reverse_iterator rbegin();
	reverse_iterator rend();
private:
	std::map<int32_t, std::string> connections;
	std::mutex mutex;
};

bool TcpServer::listenSocket()
{
	connections.reset(new ConnectionList());

	int nready, maxfd;
	struct sockaddr_in server_addr;
	fd_set fd_read;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
	{
		printf("socket() failed: %d\n", errno);
		perror("error: ");
		return false;
	}

	uint32_t type_connection = listen_local ? INADDR_LOOPBACK : INADDR_ANY;
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(type_connection);
	server_addr.sin_port = htons(static_cast<uint16_t>(port));
	if (bind(sockfd, reinterpret_cast<struct sockaddr *>(&server_addr), sizeof (server_addr)) < 0)
	{
		printf("bind() failed: %d\n", errno);
		perror("error ");
		return false;
	}

	if (listen(sockfd, max_connection) < 0)
	{
		printf("listen() failed: %d\n", errno);
		perror("error ");
		return false;
	}

	printf("Server is listening on port: %d.\n", port);
	printf("Waiting for connections...\n");

	while (true)
	{
		FD_ZERO(&fd_read); // clear a set
		FD_SET(sockfd, &fd_read); // add file descr to a set

		maxfd = sockfd;

//		for (auto& conn : connections)
//		{
//			if (conn != -1)
//			{
//				FD_SET(conn, &fd_read);
//			}
//			maxfd = (conn > maxfd) ? (conn) : (maxfd);
//		}

		nready = pselect(maxfd + 1, &fd_read, nullptr, nullptr, nullptr, nullptr);
		if (nready < 0)
		{
			printf("pselect() failed: %d\n", errno);
			perror("error ");
			return false;
		}

		if (FD_ISSET(sockfd, &fd_read))
		{
			if (newConnection(sockfd) == false)
			{
				printf("accept() or fcntl() failed: %d\n", errno);
				perror("error ");
			}
		}

		for (auto iter = connections->rbegin(); iter != connections->rend(); ++iter)
		{
			auto conn = *iter;
			bool is_close_connection = false;
			if (receive(conn.first, conn.second, is_close_connection) == false)
			{
				printf("recvfrom() failed: %d\n", errno);
				perror("error ");
				return false;
			}
			else if(is_close_connection)
			{
				connections->remove(conn.first);
			}
		}
	}
}

int32_t TcpServer::getPort() const
{
	return port;
}

uint16_t TcpServer::getMaxConnection() const
{
	return max_connection;
}

void TcpServer::setMaxConnection(const uint16_t &value)
{
	max_connection = value;
}

bool TcpServer::isListenLocal() const
{
	return listen_local;
}

bool TcpServer::newConnection(int32_t fd)
{
	struct sockaddr_in client_addr;
	socklen_t clilen = sizeof (client_addr);
	int connfd = accept(fd, reinterpret_cast<struct sockaddr *>(&client_addr), &clilen);
	bool is_failed = connfd < 0;
	if (is_failed)
	{
		printf("accept() failed: %d\n", errno);
		perror("error ");
	}
	else
	{
		std::string net = inet_ntoa(client_addr.sin_addr);
		connections->insert(connfd, net);
	}

	return (is_failed == false);
}

bool TcpServer::receive(int32_t conn, const std::string& addr, bool& is_close_conn)
{
	constexpr uint16_t BufferSize = 1024;
	is_close_conn = false;
	char msg_buffer[BufferSize] = { 0 };
	struct sockaddr_in client_addr;
	socklen_t clilen = sizeof (client_addr);

	auto bytes = recvfrom(conn, msg_buffer, BufferSize, 0,
						  reinterpret_cast<struct sockaddr *>(&client_addr), &clilen);
	if (bytes == -1)
	{
		printf("recvfrom() failed: %d\n", errno);
		perror("error");
		return false;
	}
	else if (bytes == 0)
	{
		printf("Client %s disconnected.\n", inet_ntoa(client_addr.sin_addr));
		closeConnection(conn);
		is_close_conn = true;
	}
	else
	{
		if (cb_handler != nullptr)
		{
			cb_handler(std::string(msg_buffer), addr, conn);
		}
//		printf("Got message from client %d: %s", serial, msg);
	}

	return true;
}

void TcpServer::setHandlerCallback(std::function<void (const std::string&, const std::string&, int32_t)> value)
{
	cb_handler = value;
}

void TcpServer::write(int32_t socket,const std::string& package)
{
	if (send(socket, package.data(), package.size(), 0) == -1)
	{
		perror("error:");
	}
}

void TcpServer::closeConnection(int32_t sock)
{
	connections->remove(sock);
	close(sock);
}

void TcpServer::stop()
{
	close(sockfd);
}

void TcpServer::setListenLocal(bool value)
{
	listen_local = value;
}

TcpServer::TcpServer() = default;

TcpServer::~TcpServer() = default;

void TcpServer::setPort(const int32_t &value)
{
	port = value;
}

void TcpServer::ConnectionList::insert(int32_t val, const std::string & ip)
{
	std::lock_guard<std::mutex> lock(mutex);
	connections[val] = ip;
}

void TcpServer::ConnectionList::remove(int32_t val)
{
	std::lock_guard<std::mutex> lock(mutex);
	connections.erase(val);
}

TcpServer::ConnectionList::reverse_iterator TcpServer::ConnectionList::rbegin()
{
	std::lock_guard<std::mutex> lock(mutex);
	return connections.rbegin();
}

TcpServer::ConnectionList::reverse_iterator TcpServer::ConnectionList::rend()
{
	std::lock_guard<std::mutex> lock(mutex);
	return connections.rend();
}
