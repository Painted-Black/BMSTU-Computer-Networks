#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <limits.h>
#include <fcntl.h>

#include "info.h"

#define MAX_CONN 6

int make_socket_nonblocking(int sock);
void process(char* msg);
int print_num_in_notation(long num, int base);
int dec_to_base(long num, char** result, int base);
char num_to_char(int rem);
int my_strtev(char *str);
int convert_to_add_code(const char* str, char** result);
void invert(char** str);
void add_unit(char** str);

int main(void)
{
	printf("UDP server\n");
	int sockfd;
	struct sockaddr_in server_addr, client_addr;
	socklen_t clilen = sizeof (client_addr);
	int cur_msg_len;

	sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (sockfd < 0)
	{
		printf("socket() failed: %d\n", errno);
		return -errno;
	}

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(SOCK_PORT);
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(sockfd, (struct sockaddr*) &server_addr, sizeof (server_addr)) < 0)
	{
		printf("bind() failed: %d\n", errno);
		return -errno;
	}

	if (make_socket_nonblocking(sockfd) == -1)
	{
	   printf("fcntl() failed: %d\n", errno);
	   return EXIT_FAILURE;
	}

	printf("Server is running on %s:%d\n", inet_ntoa(server_addr.sin_addr),
		   ntohs(server_addr.sin_port));


	for (;;)
	{
		char msg[MSG_LEN] = { '\0' };
		cur_msg_len = recvfrom(sockfd, msg, MSG_LEN, 0, (struct sockaddr *) &client_addr, &clilen);
		if (cur_msg_len != -1)
		{
			msg[cur_msg_len] = '\0';
			printf("Recieved message from client %s:%d\n", inet_ntoa(client_addr.sin_addr),
				   ntohs(client_addr.sin_port));
			process(msg);
			printf("\n");
		}

	}
	return 0;
}

void process(char* msg)
{
	printf("Processing message...\n");
	char* endptr;
	long recv_num;
	int is_err;

	recv_num = strtol(msg, &endptr, 10);
	if (errno == ERANGE)
	{
		printf("Nuber was too big or too small and was limited\n");
	}
	if (strlen(endptr) != 0)
	{
		printf("Cannot convert message to number in %d notation\n", 10);
	}
	printf("Number in %d notation: %ld\n", 10, recv_num);

	is_err = print_num_in_notation(recv_num, 2);
	if (is_err != 0)
	{
		printf("Cannot convert message to number in %d notation\n", 2);
	}
	is_err = print_num_in_notation(recv_num, 8);
	if (is_err != 0)
	{
		printf("Cannot convert message to number in %d notation\n", 8);
	}
	is_err = print_num_in_notation(recv_num, 15);
	if (is_err != 0)
	{
		printf("Cannot convert message to number in %d notation\n", 15);
	}
	is_err = print_num_in_notation(recv_num, 16);
	if (is_err != 0)
	{
		printf("Cannot convert message to number in %d notation\n", 16);
	}

}

int convert_to_add_code(const char* str, char** result)
{
	//printf("\nNum to convert in add: %s\n", str);
	int l = strlen(str);
	if (l < 1)
	{
		return -1;
	}
	char* tmp = *result;
	int digits = (l / 8) * 8 + 8;
	//printf("Digits: %d\n", digits);

	tmp[0] = '1';
	int i;
	for (i = 1; i < digits - l + 1; ++i)
	{
		tmp[i] = '0';
	}
	for (int j = 1 ; i < digits; ++i, ++j)
	{
		tmp[i] = str[j];
	}
	invert(result);
	add_unit(result);
	return 0;
}

void add_unit(char** str)
{
	char* tmp = *str;
	int last_idx = strlen(tmp) - 1;
	if (last_idx < 0)
		return;
	for (int i = last_idx; i > 0; --i)
	{
		if (tmp[i] == '1')
			tmp[i] = '0';
		else if (tmp[i] == '0')
		{
			tmp[i] = '1';
			break;
		}
	}
}

void invert(char** str)
{
	char* tmp = *str;
	int strl = strlen(tmp);
	for (int i = 1; i < strl; ++i)
	{
		if (tmp[i] == '0')
			tmp[i] = '1';
		else if (tmp[i] == '1')
			tmp[i] = '0';
	}
}

int print_num_in_notation(long num, int base)
{
	int is_err;
//	size_t res_size = MSG_LEN;
	char* res = calloc(sizeof (char), MSG_LEN);
	int exit_code = 0;
	if (res == NULL)
	{
		printf("Unable to allocate memory\n");
		return -ENOMEM;
	}

	is_err = dec_to_base(num, &res, base);
	if (is_err != 0)
	{
		printf("Error occured while converting to %d notation\n", base);
		exit_code = -1;
	}
	else
	{
		printf("Number in %d notation: %s", base, res);
	}
	if (base == 2 && num < 0)
	{
		char* add_res = calloc(sizeof (char), MSG_LEN);
		if (add_res == NULL)
		{
			printf("Unable to allocate memory\n");
			exit_code = -ENOMEM;
		}
		else
		{
			is_err = convert_to_add_code(res, &add_res);
			if (is_err != 0)
			{
				printf("Error occured while converting to additional code\n");
				exit_code = EXIT_FAILURE;
			}
			else
			{
				printf(" (%s)", add_res);
			}
			free(add_res);
		}
	}
	printf("\n");
	free(res);

	return exit_code;
}

int dec_to_base(long num, char** result, int base)
{
	char* tmp = *result;
	size_t idx = 0;
	int rem;
	bool is_neg = false;

	if (num < 0)
	{
		num *= -1;
		is_neg = true;
	}

	while (num >= base)
	{
		rem = num % base;
		num /= base;
		tmp[idx] = num_to_char(rem);
		idx++;
	}
	tmp[idx] = num_to_char(num);

	if (is_neg == true)
	{
		tmp[idx + 1] = '-';
		tmp[idx + 2] = '\0';
	}
	else
	{
		tmp[idx + 1] = '\0';
	}

	my_strtev(tmp);

	return 0;
}

int my_strtev(char* str)
{
	int len = strlen(str);
	int i;
	for (i = 0; i < len/2; i++)
	{
		char temp = str[i];
		str[i] = str[len-i-1];
		str[len-i-1] = temp;
	}

	return 0;
}

char num_to_char(int rem)
{
	char res;
	if (rem == 10)
	{
		res = 'A';
	}
	else if (rem == 11)
	{
		res = 'B';
	}
	else if (rem == 12)
	{
		res = 'C';
	}
	else if (rem == 13)
	{
		res = 'D';
	}
	else if (rem == 14)
	{
		res = 'E';
	}
	else if (rem == 15)
	{
		res = 'F';
	}
	else
	{
		res = rem + '0';
	}
	return res;
}

int make_socket_nonblocking(int sock)
{
	int flags = fcntl(sock, F_GETFL, 0);
	if (flags == -1)
		return -1;
	return fcntl(sock, F_SETFL, flags | O_NONBLOCK);
}
