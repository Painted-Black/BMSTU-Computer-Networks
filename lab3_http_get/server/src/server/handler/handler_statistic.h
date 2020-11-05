#pragma once

#include <memory>
#include "core/responce.h"
#include "core/rest_handler.h"
#include "statistic/statistic_server.h"

class HandlerStatistic final : public RestHandler
{
public:
	HandlerStatistic(std::shared_ptr<Statistic> stat);
	virtual ~HandlerStatistic() override {}
	void receive() override;

	std::unique_ptr<RestHandler> clone() override;

private:
	std::shared_ptr<Statistic> statistic;
};

