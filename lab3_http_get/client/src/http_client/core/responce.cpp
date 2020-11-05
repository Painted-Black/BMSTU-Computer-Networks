#include <assert.h>
#include "responce.h"

void Responce::setStatusCode(uint64_t code)
{
	status_code = code;
}

uint64_t Responce::getStatus() const
{
	return status_code;
}
