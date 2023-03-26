#pragma once
#include <string>
#include <sstream>
#include "WorkStation.h"
using std::string;
using std::stringstream;

class Request {
private:
	static int CUR_SEQ;

public:
	// this pointer shouldn't be changed after construction by user
	WorkStation* requester;
	int priority;
	int seq;

	Request(WorkStation* requester)
		:requester(requester) {
		priority = requester->requestTaskPriority;
		seq = CUR_SEQ++;
		//fprintf(stderr, "[DEBUG] new %s\n", toString().c_str());
	}

	// determine which request is larger(more important)
	bool operator<(const Request& isLarger) const {
		return (isLarger.priority == priority ?
			isLarger.seq < seq :
			isLarger.priority < priority);
	}

	string toString() {
		stringstream buffer;
		buffer << "Request{requester=" << requester->toString() << ", priority=" << priority << ", seq=" << seq << "}";
		return buffer.str();
	}
};

int Request::CUR_SEQ = 0;