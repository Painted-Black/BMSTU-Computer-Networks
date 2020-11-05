#pragma once

#include <memory>
#include "core/responce.h"
#include "core/rest_handler.h"
#include "statistic/statistic_server.h"

class HtmlFileHandler final : public RestHandler
{
public:
	HtmlFileHandler(std::shared_ptr<Statistic> stat);
	void receive() override;
	std::unique_ptr<RestHandler> clone() override;

private:
	std::shared_ptr<Statistic> statistic;
};

