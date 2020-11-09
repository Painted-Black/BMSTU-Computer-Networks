#include <fstream>
#include "core/request.h"
#include "html_file_handler.h"

constexpr char FILENAME[] = "test.html";

HtmlFileHandler::HtmlFileHandler(std::shared_ptr<Statistic> stat)
	: RestHandler({"GET"}), statistic(stat)
{

}

void HtmlFileHandler::receive()
{
	const auto& req = getRequest();
	auto& res = getResponce();
	Statistic::Event ev("HtmlFileHandler");
	ev.setUserData("route", req.getRoute());
	ev.setUserData("user", req.getAddress());
	statistic->receiveEvent(ev);

	std::fstream file;
	file.open(FILENAME, std::ios_base::in);
	if (file.is_open())
	{
		std::string body;
		body.assign((std::istreambuf_iterator<char>(file)),
					std::istreambuf_iterator<char>());

		res.setBody(body);
		res.setStatusCode(Responce::Ok);
		res.addHeader("Content-Type", "text/html");
		file.close();
	}
	else
	{
		res.setStatusCode(Responce::NotFound);
	}
}

std::unique_ptr<RestHandler> HtmlFileHandler::clone()
{
	return std::unique_ptr<RestHandler>(new HtmlFileHandler(statistic));
}
