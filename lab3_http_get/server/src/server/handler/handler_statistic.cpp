#include "handler_statistic.h"

HandlerStatistic::HandlerStatistic(std::shared_ptr<Statistic> stat)
	: RestHandler({"GET"}), statistic(stat)
{
}

/**
 * @brief Формирует ответ, который автоматически отправляет его при выходе из функции
 */
void HandlerStatistic::receive()
{
	auto& resp = getResponce();
	statistic->build();
	resp.setStatusCode(Responce::Ok);
	resp.setBody(statistic->getBuildedData());
	resp.addHeader("Content-Type", "application/json");
}

std::unique_ptr<RestHandler> HandlerStatistic::clone()
{
	return std::unique_ptr<RestHandler>(new HandlerStatistic(statistic));
}
