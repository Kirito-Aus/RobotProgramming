#include <iostream>
#include <algorithm>
#include <queue>
#include <set>
#include <cmath>
#include "WorkStation.h"
#include "Robot.h"
#include "Task.h"
#include "Request.h"
#define PI 3.1415926
using namespace std;

bool workStationPriority[101][101] = { false };
priority_queue<Task> taskList;
priority_queue<Request> requestList[8]; // request for product 1-7
set<WorkStation*> resourceList[8];	// resource of product 1-7
Robot robot[4];
WorkStation* workStationList;

// calc angle
inline float calcAngle(Position src, Position tar) {
	float distX = (src.x - tar.x);
	float distY = (src.y - tar.y);
	float angle = atan2(distY, distX);
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
void printMotion(Robot& robot) {
	switch (robot.assignedTask.status) {
	case TO_START_POINT:
		WorkStation& targetWs = workStationList[robot.assignedTask.start->id];
		if (robot.nearbyWorkStationId == targetWs.id) {
			if (robot.loadType != 0) {
				fprintf(stderr, "[ERROR] %s cannot buy since it's loaded\n", robot.toString().c_str());
				return;
			}
			if (targetWs.producionStatus == 1)	cout << "buy " << robot.id << endl;
			return;
		}
		printForwardMotion(robot, targetWs);
	case TO_END_POINT:
		WorkStation& targetWs = workStationList[robot.assignedTask.end->id];
		if (robot.nearbyWorkStationId == targetWs.id) {
			if (robot.loadType == 0) {
				fprintf(stderr, "[ERROR] %s cannot sell since it's empty\n", robot.toString().c_str());
				return;
			}
			cout << "sell " << robot.id << endl;
			return;
		}
		printForwardMotion(robot, targetWs);
	}

	//float distX = (robot[robotNum].position.x - workStationList[workStationNum].position.x);
	//float distY = (robot[robotNum].position.y - workStationList[workStationNum].position.y);
	//float dist = sqrt(distX * distX + distY * distY);//the ditance between robot and workStation
	//float angle = atan2(distY, distX);
	//if (dist <= 0.4) {
	//	if (robot[robotNum].loadType==0 && workStationList[workStationNum].producionStatus == 1) {
	//		cout << "buy " << robotNum << endl;
	//	}
	//	if (robot[robotNum].loadType != 0) {
	//		cout << "sell " << robotNum << endl;
	//	}
	//}
	//else {
	//	if (robot[robotNum].facing != angle) {
	//		float deltaAngle = angle - robot[robotNum].facing;
	//		if (abs(deltaAngle) <= PI / 2 * 0.02) 
	//			cout << "rotate " << robotNum << " " << deltaAngle /0.02 << endl;
	//		else
	//			cout << "rotate " << robotNum << " " << (deltaAngle > 0 ? PI/2: -PI/2 )<< endl;
	//	}	
	//	else {
	//			cout << "forward " << robotNum << " " << 6 << endl;
	//	}
	//}
}

int main()
{
	float facing = atan2(1,1);
	float angle = -PI / 4;
	float deltaAngle = (facing - angle) >= -PI / 2 && (facing - angle <= PI / 2) ?
		facing - angle : angle - facing;

	cout << facing << endl;
	return 0;
}