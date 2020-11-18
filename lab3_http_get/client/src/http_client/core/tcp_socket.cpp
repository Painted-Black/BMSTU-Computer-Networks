#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h> // inet_ntoa
#include "tcp_socket.h"

std::string SenderHandler::sendBytea(const std::string & bytea, const std::string& ip, uint16_t port)
{
	struct sockaddr_in server_addr;

	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
	{
		printf("socket() failed: %d\n", errno);
		perror("error: ");
		return std::string();
	}

	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr(ip.c_str());
	server_addr.sin_port = htons(port);
	if (connect(sockfd, reinterpret_cast<struct sockaddr *>(&server_addr), sizeof (server_addr)) < 0)
	{
		printf("connect() failed: %d\n", errno);
		perror("error: ");
		return std::string();
	}

	if (sendto(sockfd, static_cast<const void*>(bytea.c_str()), bytea.size(), MSG_DONTROUTE | MSG_WAITALL, nullptr, 0) < 0)
	{
		printf("sendto() failed: %d\n", errno);
		return std::string();
	}

	std::string result;
	char buf[1024] = { '\0' };
	ssize_t read_count = 0;
	while ((read_count = recv(sockfd, buf, 1024, MSG_WAITALL)) > 0)
	{
		result.append(buf, static_cast<std::string::size_type>(read_count));
	}

	close(sockfd);
	return result;
}
