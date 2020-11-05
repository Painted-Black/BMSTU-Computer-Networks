#pragma once

#include <tuple>
#include "rest_data.h"

class Responce : public RestData
{
public:
	void setStatusCode(uint64_t);
	uint64_t getStatus() const;
private:
	uint64_t status_code;
};

