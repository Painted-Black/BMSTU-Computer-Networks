#pragma once

#include <list>
#include <memory>
#include <stdint.h>
#include <functional>
#include "connection_handler.h"

class TcpServer
{
public:
	void setPort(const int32_t &value);
	void setListenLocal(bool value);
	void setMaxConnection(const uint16_t &value);

	bool listenSocket();

	int32_t getPort() const;
	uint16_t getMaxConnection() const;
	bool isListenLocal() const;
	void setHandlerCallback(std::function<void(const std::string&, int32_t)> value);
	void write(int32_t, const std::string&);
	void closeConnection(int32_t);
	void stop();

private:
	inline bool newConnection(int32_t);
	inline bool receive(int32_t, bool &is_close_conn);

private:
	int sockfd;
	int32_t port = -1;
	uint16_t max_connection = 10;
	bool listen_local = false;
	std::list<int32_t> connections;
	std::function<void(const std::string&, int32_t)> cb_handler;
};
