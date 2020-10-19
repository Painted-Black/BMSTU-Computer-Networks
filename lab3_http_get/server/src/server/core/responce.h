#pragma once

#include <tuple>
#include "rest_data.h"
#include "rest_handler.h"

class RestHandler::Responce : public RestData
{
public:
	enum StatusCode
	{
		Ok = 200,
		Created = 201,
		NotFound = 404,
		MethodNotAllowed = 405
	};
public:
	Responce();
	void setStatusCode(StatusCode);
	const std::string& getStatus() const;
private:
	std::string status_code;
};

