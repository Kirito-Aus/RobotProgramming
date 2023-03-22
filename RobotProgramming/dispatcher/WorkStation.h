#pragma once
#include "Position.h"

class WorkStation {
private:
	// get the tier of workstation on production tree
	static int getTier(int type)
	{
		if (type >= 8) return 0;
		if (type == 7) return 1;
		return 2;
	}

public:
	static WorkStation nullWorkStation;

	// status
	int id;
	int type;
	Position position;
	int remainingProduceTime;
	int materialStatus;
	int producionStatus;

	int priority;
	// the priority of task which this workstation request from others
	int requestTaskPriority;

	WorkStation(){}

	void updateRequestPriority() {
		requestTaskPriority = priority * 3 + getTier(type);
	}
};

WorkStation WorkStation::nullWorkStation;