#pragma once

#include <tuple>
#include "rest_data.h"
#include "rest_handler.h"

class RestHandler::Responce : public RestData
{
public:
	void setStatusCode(const uint16_t &value);
	//void
#pragma message "TODO"
private:
	uint16_t status_code;
};

