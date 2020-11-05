#pragma once

#include <map>
#include "rest_data.h"
#include "url.h"

class Request : public RestData
{
public:
	Request(const Url& url);
	const Url& getUrl() const;

private:
	Url url;
};

