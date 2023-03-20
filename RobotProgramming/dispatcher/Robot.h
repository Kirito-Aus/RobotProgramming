#pragma once
#include "Task.h"
#include "LinearSpeed.h"

class Robot {
	// status
	int id;
	int nearbyWorkStationId;
	int load;
	float timeValue;
	float collisionValue;
	float angularSpeed;
	LinearSpeed linearSpeed;
	float facing;

	Position position;

	Task* assignedTask = &Task::nullTask;

	Robot(int id, Position& position)
		:id(id), position(position) {
		load = 0;
		nearbyWorkStationId = -1;
		timeValue = 0;
		collisionValue = 0;
		angularSpeed = 0;
		facing = 0;
	}
};