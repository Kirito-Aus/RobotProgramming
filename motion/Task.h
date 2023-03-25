#pragma once
#include <string>
#include <sstream>
#include "WorkStation.h"
using std::string;
using std::stringstream;

enum TaskStatus{
	READY,
	TO_START_POINT,
	TO_END_POINT,
	DONE
};

const char* strTaskStatus[4] = {
	"READY",
	"TO_START_POINT",
	"TO_END_POINT",
	"DONE"
};

class Task {
private:
	static int CUR_SEQ;

public:
	// empty task assigned to new robot
	static Task nullTask;

	// these pointers shouldn't be changed after construction by user
	WorkStation *start, *end;
	TaskStatus status;
	// sequnce number of task, used to compare
	int seq;
	// less the num, higher the priority
	int priority;

	Task(WorkStation* start, WorkStation* end, int priority, TaskStatus status = READY)
		:start(start), end(end), priority(priority), status(status) {
		seq = CUR_SEQ++;
		fprintf(stderr, "[DEBUG] new %s\n", toString().c_str());
	}

	// determine which task is larger(more important)
	bool operator<(const Task& isLarger) const {
		return (isLarger.priority == priority ?
			isLarger.seq < seq :
			isLarger.priority < priority);
	}

	string toString() {
		stringstream buffer;
		buffer << "Task{start=" << start->toString() << ", end=" << end->toString() << ", priority=" << priority << ", seq=" << seq << ", status=" << strTaskStatus[status] << "}";
		return buffer.str();
	}
};

int Task::CUR_SEQ = 0;
Task Task::nullTask(&WorkStation::nullWorkStation, &WorkStation::nullWorkStation , 0, DONE);