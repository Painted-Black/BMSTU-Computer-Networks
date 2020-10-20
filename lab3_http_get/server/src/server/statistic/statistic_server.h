#pragma once

#include "statistic.h"

class StatisticServer : public Statistic
{
public:
	void receiveEvent(const Event &) override;
};

