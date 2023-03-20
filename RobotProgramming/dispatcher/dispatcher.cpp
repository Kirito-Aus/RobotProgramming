#include <iostream>
#include <algorithm>
#include <queue>
#include "WorkStation.h"
#include "Robot.h"
#include "Task.h"
using namespace std;

bool workStationLocation[101][101]={false};
priority_queue<Task> taskList;
queue<WorkStation> requestList[8]; //require for product 1-7
Robot robot[4];
WorkStation* workStationList;
bool* workStationUsed;

bool isWorkStationUsed(float x, float y) {
	int i = (199 - int(y * 4)) >> 1;
	int j = (int(x * 4) - 1) >> 1;
	//fprintf(stderr, "[debug] (%f, %f) -> [%d][%d]", x, y, i, j);
	return workStationLocation[i][j];
}

int main() {
	// get used work station from pre-process

	bool init = false;

	// input vars
	int frameSeq, coins;
	int nWorkStation;

	// input vars - workStation
	int wsType;
	float wsPosX, wsPosY;
	int wsRemainingTime, wsMaterialStatus, wsProductionStatus;

	// input vars - robot
	int rNearbyWs, rLoadType;
	float rTimeValue, rCollisionValue, rAngularSpeed, rLinearSpeedX, 
		rLinearSpeedY, rFacing, rPosX, rPosY;

	cin >> frameSeq;
	while (!cin.eof()) {
		cin >> coins;
		cin >> nWorkStation;
		if (!init) {
			workStationList = new WorkStation[nWorkStation];
			workStationUsed = new bool[nWorkStation];
			for (int i = 0; i < nWorkStation; i++) {
				cin >> wsType >> wsPosX >> wsPosY >> 
					wsRemainingTime >> wsMaterialStatus >> wsProductionStatus;
				workStationUsed[i] = isWorkStationUsed(wsPosX, wsPosY);
				if (workStationUsed[i]) {
					workStationList[i].id = i;
					workStationList[i].setPriority(0);
					workStationList[i].position.x = wsPosX;
					workStationList[i].position.y = wsPosY;
					workStationList[i].remainingProduceTime = wsRemainingTime;
					workStationList[i].materialStatus = wsMaterialStatus;
					workStationList[i].producionStatus = wsProductionStatus;
				}
			}
			for (int i = 0; i < 4; i++) {
				cin >> rNearbyWs >> rLoadType >> rTimeValue >> rCollisionValue >>
					rAngularSpeed >> rLinearSpeedX >> rLinearSpeedY >> rFacing >> 
					rPosX >> rPosY;
				robot[i].nearbyWorkStationId = rNearbyWs;
				robot[i].loadType = rLoadType;
				robot[i].timeValue = rTimeValue;
				robot[i].collisionValue = rCollisionValue;
				robot[i].angularSpeed = rAngularSpeed;
				robot[i].linearSpeed.x = rLinearSpeedX;
				robot[i].linearSpeed.y = rLinearSpeedY;
				robot[i].facing = rFacing;
				robot[i].position.x = rPosX;
				robot[i].position.y = rPosY;
			}
			init = true;
		}
		else {

		}
		cin >> frameSeq;
	}
	return 0;
}