#pragma once
#include "WorkStation.h"

enum TaskStatus{
	READY,
	TO_START_POINT,
	TO_END_POINT,
	DONE
};

class Task {
private:
	static int CUR_SEQ;

public:
	// empty task assigned to new robot
	static Task nullTask;
	WorkStation *const start, *const end;
	TaskStatus status;
	// sequnce number of task, used to compare
	int seq;
	// less the num, higher the priority
	int priority;

	Task(WorkStation* start, WorkStation* end, int priority, TaskStatus status = READY)
		:start(start), end(end), priority(priority), status(status) {
		this->seq = CUR_SEQ++;
	}

	// determine which task is larger(more important)
	bool operator<(const Task& target) const {
		return (target.priority == this->priority ?
			target.seq < this->seq :
			target.priority < this->priority);
	}
};

int Task::CUR_SEQ = 0;
Task Task::nullTask(nullptr, nullptr, 0, DONE);