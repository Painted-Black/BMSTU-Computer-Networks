#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <limits.h>

#include "info.h"

int get_message(char* message, size_t msg_len);
void clear_stdin(void);

int main(void)
{
	int sockfd;
	int num_to_send;
	struct sockaddr_in addr;
	char* msg = NULL, *endptr;
	int cur_msg_len = 0;
	bool success = false;

	printf("UDP client\n");

	sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP); // create an endpoint for communication
	if (sockfd < 0)
	{
		printf("socket() failed: %d\n", errno);
		return -errno;
	}

	addr.sin_family = AF_INET; // IPv4 internet protocols
	addr.sin_port = htons(SOCK_PORT); // convert the unsigned short integer hostshort from
									  // host byte order to network byte order.
	addr.sin_addr.s_addr = htonl(SERVER_IP); // loopback interface address
			// htonl converts the unsigned integer hostlong  from  host
			// byte order to network byte order.

	msg = calloc(sizeof(char), MSG_LEN);
	if (msg == NULL)
	{
		printf("Unable to allocate memory with calloc()\n");
		return -ENOMEM;
	}

	while (success == false)
	{
		cur_msg_len = get_message(msg, MSG_LEN);
		if (cur_msg_len == 0)
		{
			printf("Incorrect input. Try again...\n");
		}
		else
		{
			num_to_send = strtol(msg, &endptr, 10);
			if (errno == ERANGE)
			{
				printf("Input nuber was too big or too small and was limited\n");
			}
			//printf("errno: %d\n", errno);
			if (strlen(endptr) != 0)
			{
				printf("Incorrect input. Try again...\n");
			}
			else
			{
				printf("Correct input\n");
				success = true;
			}
		}
	}

	printf("Sending message...\n", SERVER_IP);
	 // send a message on a socket
	if (sendto(sockfd, msg, strlen(msg), 0, (struct sockaddr*) &addr, sizeof (addr)) < 0)
	{
		printf("sendto() failed: %d\n", errno);
		return -errno;
	}

	free(msg);
	close(sockfd);
	return 0;
}

int get_message(char* message, size_t msg_len)
{
	int cres_len = 0;
	printf("Input integer in decimal notation: ");
	fgets(message, msg_len, stdin);

	cres_len = strlen(message);

	if (message[cres_len - 1] == '\n')
	{
		message[cres_len - 1] = '\0';
		cres_len--;
	}
	clear_stdin();
	return cres_len;
}

void clear_stdin(void)
{
	stdin->_IO_read_ptr = stdin->_IO_read_end;
}
