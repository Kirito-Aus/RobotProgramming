#pragma once
#include "WorkStation.h"

class Request {
private:
	static int CUR_SEQ;

public:
	WorkStation* requester;
	int priority;
	int seq;

	Request(WorkStation* requester)
		:requester(requester) {
		priority = requester->requestTaskPriority;
		seq = CUR_SEQ++;
	}

	// determine which request is larger(more important)
	bool operator<(const Request& isLarger) const {
		return (isLarger.priority == priority ?
			isLarger.seq < seq :
			isLarger.priority < priority);
	}
};

int Request::CUR_SEQ = 0;