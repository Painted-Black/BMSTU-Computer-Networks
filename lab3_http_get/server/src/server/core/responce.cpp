#include <assert.h>
#include "responce.h"

constexpr char MimeContentType_Default[] = "text/plain";

RestHandler::Responce::Responce()
	: status_code("500 Internal Server Error")
{
	setHttpVersion("HTTP/1.1");
	addHeader("Content-Type", MimeContentType_Default);
}

void RestHandler::Responce::setStatusCode(StatusCode code)
{
	switch (code)
	{
	case Ok:
		status_code = "200 OK"; break;
	case Created:
		status_code = "201 Created"; break;
	case NotFound:
		status_code = "404 Not Found"; break;
	case MethodNotAllowed:
		status_code = "405 Method Not Allowed"; break;
	default:
		assert(false); /* Unexpected status code */
	}
}

const std::string &RestHandler::Responce::getStatus() const
{
	return status_code;
}
