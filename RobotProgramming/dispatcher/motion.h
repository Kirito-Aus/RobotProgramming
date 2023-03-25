#pragma once
#include <iostream>
#include <algorithm>
#include <queue>
#include <set>
#include <cmath>
#include "Robot.h"
#define PI 3.1415926
using namespace std;

// calc angle
inline float calcAngle(Position src, Position tar) {
	float distX = (src.x - tar.x);
	float distY = (src.y - tar.y);
	return atan2(distY, distX);
}

// print robot to workstation motion
void printForwardMotion(Robot& robot, WorkStation& ws) {
	float angle = calcAngle(robot.position, ws.position);
	if (robot.facing != angle) {
		float deltaAngle = angle - robot.facing;
		if (abs(deltaAngle) <= PI / 2 * 0.02)
			cout << "rotate " << robot.id << " " << deltaAngle / 0.02 << endl;
		else
			cout << "rotate " << robot.id << " " << (deltaAngle > 0 ? PI / 2 : -PI / 2) << endl;
	}
	else {
		cout << "forward " << robot.id << " " << 6 << endl;
	}
}

// output the motion of a robot
void printMotion(Robot& robot, WorkStation* wsList) {
	switch (robot.assignedTask.status) {
	case TO_START_POINT: {
		WorkStation& targetWs = wsList[robot.assignedTask.start->id];
		if (robot.nearbyWorkStationId == targetWs.id) {
			if (robot.loadType != 0) {
				fprintf(stderr, "[ERROR] %s cannot buy since it's loaded\n", robot.toString().c_str());
				return;
			}
			if (targetWs.producionStatus == 1)	cout << "buy " << robot.id << endl;
			return;
		}
		printForwardMotion(robot, targetWs);
		return;
	}
	case TO_END_POINT: {
		WorkStation& targetWs = wsList[robot.assignedTask.end->id];
		if (robot.nearbyWorkStationId == targetWs.id) {
			if (robot.loadType == 0) {
				fprintf(stderr, "[ERROR] %s cannot sell since it's empty\n", robot.toString().c_str());
				return;
			}
			cout << "sell " << robot.id << endl;
			return;
		}
		printForwardMotion(robot, targetWs);
		return;
	}
	}
}