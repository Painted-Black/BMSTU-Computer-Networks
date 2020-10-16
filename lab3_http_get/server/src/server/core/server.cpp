#include <utility>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h> // inet_ntoa
#include "server.h"

bool TcpServer::listenSocket()
{
	for (auto& conn : connections)
	{
		conn = -1;
	}

	int nready, maxfd;
	struct sockaddr_in server_addr;
	fd_set fd_read;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
	{
		printf("socket() failed: %d\n", errno);
		return false;
	}

	uint32_t type_connection = listen_local ? INADDR_LOOPBACK : INADDR_ANY;
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(type_connection);
	server_addr.sin_port = htons(port);
	if (bind(sockfd, (struct sockaddr *) &server_addr, sizeof (server_addr)) < 0)
	{
		printf("bind() failed: %d\n", errno);
		return false;
	}

	if (listen(sockfd, max_connection) < 0)
	{
		printf("listen() failed: %d\n", errno);
		return false;
	}

	printf("Server is listening on port: %d.\n", port);
	printf("Waiting for connections...\n");

	while (true)
	{
		FD_ZERO(&fd_read); // clear a set
		FD_SET(sockfd, &fd_read); // add file descr to a set

		maxfd = sockfd;

		for (auto& conn : connections)
		{
			if (conn != -1)
			{
				FD_SET(conn, &fd_read);
			}
			maxfd = (conn > maxfd) ? (conn) : (maxfd);
		}

		nready = pselect(maxfd + 1, &fd_read, NULL, NULL, NULL, NULL);
		if (nready < 0)
		{
			printf("pselect() failed: %d\n", errno);
			return false;
		}

		if (FD_ISSET(sockfd, &fd_read))
		{
			if (newConnection(sockfd) == false)
				printf("accept() or fcntl() failed: %d\n", errno);
		}

		for (auto iter = connections.rbegin(); iter != connections.rend(); ++iter)
		{
			auto conn = *iter;
			bool is_close_connection = false;
			if (receive(conn, is_close_connection) == false)
			{
				printf("recvfrom() failed: %d\n", errno);
				return false;
			}
			else if(is_close_connection)
			{
				connections.erase(iter.base());
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
	int connfd = accept(fd, (struct sockaddr *) &client_addr, &clilen);
	bool is_failed = connfd < 0;
	if (is_failed)
	{
		printf("accept() failed: %d\n", errno);
	}
	else
	{
		connections.push_back(connfd);
	}

	return (is_failed == false);
}

bool TcpServer::receive(int32_t conn, bool& is_close_conn)
{
	constexpr uint16_t BufferSize = 1024;
	is_close_conn = false;
	char msg_buffer[BufferSize] = { 0 };
	struct sockaddr_in client_addr;
	socklen_t clilen = sizeof (client_addr);

	int bytes = recvfrom(conn, msg_buffer, BufferSize, 0, (struct sockaddr *) &client_addr, &clilen);
	if (bytes == -1)
	{
		printf("recvfrom() failed: %d\n", errno);
		perror("Error");
		return false;
	}
	else if (bytes == 0)
	{
		printf("Client %s disconnected.\n", inet_ntoa(client_addr.sin_addr));
		close(conn);
		is_close_conn = true;
	}
	else
	{
		if (cb_handler != nullptr)
		{
			cb_handler(std::string(msg_buffer), conn);
		}
//		printf("Got message from client %d: %s", serial, msg);
	}

	return true;
}

void TcpServer::setHandlerCallback(std::function<void (const std::string&, int32_t)> value)
{
	cb_handler = value;
}

void TcpServer::write(int32_t socket,const std::string& package)
{
	if (send(socket, package.data(), package.size(), 0) == -1)
	{
		perror("error:");
	}

	close(socket);
}

void TcpServer::setListenLocal(bool value)
{
	listen_local = value;
}

void TcpServer::setPort(const int32_t &value)
{
	port = value;
}
