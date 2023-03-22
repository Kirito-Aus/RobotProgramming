#include <iostream>
#include <algorithm>
#include <queue>
#include <vector>
#include <string>
#include "WorkStation.h"
#include "Robot.h"
#include "Task.h"
#include "Request.h"
using namespace std;

bool workStationPriority[101][101]={false};
priority_queue<Task> taskList;
priority_queue<Request> requestList[8]; // request for product 1-7
vector<WorkStation*> resourceList[8];	// resource of product 1-7
Robot robot[4];
WorkStation* workStationList;

int getWorkStationPriority(float x, float y) {
	int i = (199 - int(y * 4)) >> 1;
	int j = (int(x * 4) - 1) >> 1;
	//fprintf(stderr, "[DEBUG] (%f, %f) -> [%d][%d]\n", x, y, i, j);
	return workStationPriority[i][j] ? 0 : 1;
}

void addRequest(WorkStation& ws) {
	if (ws.type < 4) return;
	fprintf(stderr, "[DEBUG] add %s into requestList\n", ws.toString().c_str());
	switch (ws.type)
	{
	case 4:
		requestList[1].push(Request(&ws));
		requestList[2].push(Request(&ws));
		return;
	case 5:
		requestList[1].push(Request(&ws));
		requestList[3].push(Request(&ws));
		return;
	case 6:
		requestList[2].push(Request(&ws));
		requestList[3].push(Request(&ws));
		return;
	case 7:
		requestList[4].push(Request(&ws));
		requestList[5].push(Request(&ws));
		requestList[6].push(Request(&ws));
		return;
	case 8:
	case 9:
		requestList[7].push(Request(&ws));
		return;
	}
}

void addResource(WorkStation& ws) {
	if (ws.remainingProduceTime != -1) {
		fprintf(stderr, "[DEBUG] add %s into resourceList\n", ws.toString().c_str());
		resourceList[ws.type].push_back(&ws);
	}
}

int main() {
	// TODO: get used work station from pre-process

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

	string strOK;

	// vars
	int preRemainingTime;

	cin >> frameSeq;
	while (!cin.eof()) {
		// input
		cin >> coins;
		cin >> nWorkStation;

		// update WorkStation
		if (!init) {
			workStationList = new WorkStation[nWorkStation];
			for (int i = 0; i < nWorkStation; i++) {
				cin >> wsType >> wsPosX >> wsPosY >> 
					wsRemainingTime >> wsMaterialStatus >> wsProductionStatus;
				workStationList[i].id = i;
				workStationList[i].type = wsType;
				workStationList[i].priority = getWorkStationPriority(wsPosX, wsPosY);
				workStationList[i].updateRequestPriority();
				workStationList[i].position.x = wsPosX;
				workStationList[i].position.y = wsPosY;
				workStationList[i].remainingProduceTime = wsRemainingTime;
				workStationList[i].materialStatus = wsMaterialStatus;
				workStationList[i].producionStatus = wsProductionStatus;
				addRequest(workStationList[i]);
				if (workStationList[i].type < 4) {
					addResource(workStationList[i]);
				}
			}
			init = true;
		}
		else {
			for (int i = 0; i < nWorkStation; i++) {
				// record prev status
				preRemainingTime = workStationList[i].remainingProduceTime;

				// update
				cin >> wsType >> wsPosX >> wsPosY >>
					wsRemainingTime >> wsMaterialStatus >> wsProductionStatus;
				workStationList[i].position.x = wsPosX;
				workStationList[i].position.y = wsPosY;
				workStationList[i].remainingProduceTime = wsRemainingTime;
				workStationList[i].materialStatus = wsMaterialStatus;
				workStationList[i].producionStatus = wsProductionStatus;

				// work station production start
				if (preRemainingTime == -1 && wsRemainingTime >= 0) {
					addRequest(workStationList[i]);
					addResource(workStationList[i]);
				}
			}
		}
		// update Robot
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

		// TODO: generate Task

		// TODO: assign Task to idle robot

		cin >> strOK;
		cin >> frameSeq;
	}
	delete[] workStationList;
	return 0;
}