#pragma once

#include "request.h"
#include "responce.h"
#include "tcp_socket.h"

class HttpManager
{
public:
	Responce httpGet(const Request& request);

private:
	std::string fromRequest(const Request&);
	Responce toResponce(const std::string&);

private:
	SenderHandler handler;
};

