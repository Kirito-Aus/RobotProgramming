#pragma once
#include"Position.h"

class WorkStation {
private:
	// the priority of task which this workstation request from others
	int requestTaskPriority;
	// get the tier of workstation on production tree
	static int getTier(int type)
	{
		if (type >= 8) return 0;
		if (type == 7) return 1;
		return 2;
	}

public:
	// status
	const int id;
	const int type;
	const Position position;
	int remainingProduceTime;
	int materialStatus;
	int producionStatus;

	const int priority;

	WorkStation(int id, int priority, int type, Position& position)
		:position(position), id(id), priority(priority), type(type) {
		requestTaskPriority = priority * 3 + getTier(type);
		remainingProduceTime = -1;
		materialStatus = 0;
		producionStatus = 0;
	}

	int getRequestTaskPriority()
	{
		return requestTaskPriority;
	}
};