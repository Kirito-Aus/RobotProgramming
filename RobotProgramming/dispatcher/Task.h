#pragma once
#include "WorkStation.h"

enum TaskStatus{
	READY,
	TO_START_POINT,
	TO_END_POINT,
	DONE
};

// use pointer to pass Task objects!
class Task {
private:
	static int CUR_SEQ;

public:
	// empty task assigned to new robot
	static Task nullTask;

	WorkStation &start, &end;
	TaskStatus status;
	// sequnce number of task, used to compare
	int seq;
	// less the num, higher the priority
	int priority;

	Task(WorkStation& start, WorkStation& end, int priority, TaskStatus status = READY)
		:start(start), end(end), priority(priority), status(status) {
		this->seq = CUR_SEQ++;
	}

	// determine which task is larger(more important)
	bool operator<(const Task& isLarger) const {
		return (isLarger.priority == priority ?
			isLarger.seq < seq :
			isLarger.priority < priority);
	}
};

int Task::CUR_SEQ = 0;
Task Task::nullTask(WorkStation::nullWorkStation, WorkStation::nullWorkStation , 0, DONE);