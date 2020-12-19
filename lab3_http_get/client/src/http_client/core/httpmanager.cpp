#include <list>
#include <sstream>
#include <algorithm>
#include "httpmanager.h"

std::list<std::string> split(const std::string& text, const std::string& del)
{
	std::list<std::string> out_data;
	std::string::size_type indexer = 0;
	while (indexer != std::string::npos)
	{
		std::string::size_type last_indexer = text.find_first_of(del, indexer);
		out_data.push_back(text.substr(indexer, last_indexer - indexer));
		indexer = last_indexer == std::string::npos ? last_indexer : last_indexer + del.size();
	}

	return out_data;
}

/**
 * @brief Удаляет символы chars из string до первого символа, не входящего в char
 * @param string
 * @param chars
 * @return
 */
std::string trim(const std::string& string, const std::string& chars)
{
	std::string::size_type f_idx = string.find_first_not_of(chars);
	std::string::size_type s_idx = string.find_last_not_of(chars);
	return string.substr(f_idx, s_idx - f_idx + 1);
}

/**
 * @brief Отправка и получение пакетов
 * @param req
 * @return
 */
Responce HttpManager::httpGet(const Request & req)
{
	const auto& url = req.getUrl();
	std::string body = handler.sendBytea(fromRequest(req), url.getHost(), url.getPort());
	return toResponce(body);
}

/**
 * @brief Преобразование Request в пакет
 * @param req
 * @return
 */
std::string HttpManager::fromRequest(const Request & req)
{
	constexpr char Line_Delimer[] = "\r\n";
	constexpr char Header_Delimer[] = ": ";
	const Url& url = req.getUrl();

	std::stringstream sstream;
	sstream << "GET " << url.getPath() << url.getQuery() << ' ' << req.getHttpVersion() << Line_Delimer
			<< "Host: " << url.getHost() << Line_Delimer;

	auto write_header_responce = [&](const std::pair<std::string, std::string>& header)
	{
		sstream << header.first << Header_Delimer << header.second << Line_Delimer;
	};

	const auto& headers = req.getHeaders();
	std::for_each(headers.cbegin(), headers.cend(), write_header_responce);
	sstream << Line_Delimer;

	return sstream.str();
}

/**
 * @brief преобразование приходящего пакета в класс Responce
 * @param data
 * @return
 */
Responce HttpManager::toResponce(const std::string & data)
{

	enum PackagePart
	{
		None,
		RequestInfo,
		Header,
		Body
	};

	constexpr char TrimmedChars[] = "\t\r\n ";

	PackagePart current_part = None;
	std::string::size_type indexer = 0;
	Responce responce;
	std::string body;
	while (indexer != std::string::npos)
	{
		std::string::size_type last_indexer = data.find_first_of("\r\n", indexer);
		std::string::size_type diff_count =
				last_indexer == std::string::npos ? std::string::npos : last_indexer - indexer;

		std::string boff = data.substr(indexer, diff_count);
		if (current_part == None)
		{
			uint64_t status_code;
			std::list<std::string> data = split(boff, " ");
			auto iter = data.begin();
			responce.setHttpVersion(*(iter++));

			std::stringstream sstr(*(iter++));
			sstr >> status_code;
			responce.setStatusCode(status_code);
			current_part = RequestInfo;
		}
		else if (current_part == RequestInfo || current_part == Header)
		{
			if (boff.empty())
			{
				current_part = Body;
			}
			else
			{
				std::string::size_type idx_1 = boff.find(":");
				responce.addHeader(boff.substr(0, idx_1), trim(boff.substr(idx_1 + 1), TrimmedChars));
				current_part = Header;
			}
		}
		else
		{
			body = data.substr(indexer, data.size() - 2);
			break;
		}

		indexer = last_indexer == std::string::npos ? last_indexer : last_indexer + 2;
	}

	const auto& headers = responce.getHeaders();
	uint64_t count;
	std::stringstream stream(headers.at("Content-Length"));
	stream >> count;

	body.resize(count);
	responce.setBody(body);
	return responce;
}
