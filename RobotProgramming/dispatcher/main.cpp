#include <iostream>
#include <algorithm>
#include <queue>
#include <set>
#include <string>
#include <tuple>
#include "Request.h"
#include "motion.h"
#include "Pretreatment.h"
using namespace std;

bool workStationPriority[101][101]={false};
priority_queue<Task> taskList;
priority_queue<Request> requestList[8]; // request for product 1-7
multiset<WorkStation*> resourceList[8];	// resource of product 1-7
Robot robot[4];
WorkStation* workStationList;

/**Pretreatment处的全局变量**/
// 根据ID排序的Robot列表
vector<Robot> robotVec;
// 根据ID排序的WorkStation列表
vector<WorkStation> workStationVec;
// 按照类型分列表的WorkStation列表
vector<int> stationList1, stationList2, stationList3, stationList4, stationList5, stationList6, stationList7, stationList8, stationList9;
vector<vector<int>> stations = { stationList1, stationList2, stationList3, stationList4, stationList5, stationList6, stationList7, stationList8, stationList9 };
vector<tuple<int, int>> path;

int getWorkStationPriority(float x, float y) {
	int i = (199 - int(y * 4)) >> 1;
	int j = (int(x * 4) - 1) >> 1;
	//fprintf(stderr, "[DEBUG] (%f, %f) -> [%d][%d]\n", x, y, i, j);
	return workStationPriority[i][j] ? 0 : 1;
}

void addRequest(WorkStation& ws) {
	if (ws.type < 4) return;
	if (ws.priority == 0) fprintf(stderr, "[DEBUG] add %s into requestList\n", ws.toString().c_str());
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
	if (ws.type > 7) return;
	if (!ws.resourcePushed && ws.remainingProduceTime >= 0 && ws.remainingProduceTime <= 100) {
		resourceList[ws.type].insert(&ws);
		ws.resourcePushed = true;
		if (ws.priority == 0) fprintf(stderr, "[DEBUG] add %s into resourceList\n", ws.toString().c_str());
	}
}

inline void updateRobotTaskStatus(Robot& r, int preLoadType) {
	if (r.loadType == preLoadType) return;
	Task& task = r.assignedTask;
	switch (task.status) {
	case DONE:
		fprintf(stderr, "[ERROR] %s load change with assigned task done(preLoad=%d)\n", r.toString().c_str(), preLoadType);
		return;
	case TO_START_POINT:
		if (r.loadType != task.start->type) {
			fprintf(stderr, "[ERROR] %s load change doesn't fit assigned task(preLoad=%d)\n", r.toString().c_str(), preLoadType);
			return;
		}
		task.status = TO_END_POINT;
		fprintf(stderr, "[DEBUG] %s load change(preLoad=%d)\n", r.toString().c_str(), preLoadType);
		return;
	case TO_END_POINT:
		if (r.loadType != 0) {
			fprintf(stderr, "[ERROR] %s load change doesn't fit assigned task(preLoad=%d)\n", r.toString().c_str(), preLoadType);
			return;
		}
		task.status = DONE;
		fprintf(stderr, "[DEBUG] %s load change(preLoad=%d)\n", r.toString().c_str(), preLoadType);

		// type 8 and 9 request
		if (r.assignedTask.end->type > 7) {
			addRequest(*r.assignedTask.end);
		}

		return;
	default:
		fprintf(stderr, "[ERROR] %s load change with improper assigned task(preLoad=%d)\n", r.toString().c_str(), preLoadType);
		return;
	}
}

float calcSquareDistance(Position p1, Position p2) {
	float intervalX = p1.x - p2.x;
	float intervalY = p1.y - p2.y;
	return intervalX * intervalX + intervalY * intervalY;
}

int main() {

	fprintf(stderr, "[DEBUG] Start Pretreatment\n");

	// TODO: get used work station from pre-process
	/***进行开始时的地图交互***/
	readInStartData(robotVec, workStationVec, stations);
	vector<int> taskStations;
	getPath(stations, workStationVec, path, taskStations);
	// 设置bool workStationPriority[101][101]
	for (int i = 0; i < taskStations.size(); i++) {
		CoordinatePos curCPos = workStationVec[taskStations[i]].cPos;
		// 将它对应的位置设置为true
		//cerr << curCPos.x << " " << curCPos.y << endl;
		workStationPriority[99-curCPos.x][curCPos.y] = true;
	}
	fprintf(stderr, "[DEBUG] end Pretreatment\n");

	// 输出结果
	puts("OK");
	fflush(stdout);

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
	int preLoadType;

	float minDistance = 10000, dis;

	set<WorkStation*>::iterator wsIter;
	WorkStation* nearestWs;

	set<Robot*> idleRobot;
	Robot* nearestRobot;
	set<Robot*>::iterator robotIter;

	//for (int i = 0; i < 100; i++) {
	//	for (int j = 0; j < 100; j++) {
	//		cerr << (workStationPriority[i][j] ? 0 : 1);
	//	}
	//	cerr << endl;
	//}

	// robot id assignment
	for (int i = 0; i < 4; i++) {
		robot[i].id = i;
	}

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
				if (workStationList[i].type > 7) workStationList[i].priority = 0;
				//if (workStationList[i].priority == 0) {
				//	cerr << workStationList[i].toString() << endl;
				//}
				workStationList[i].updateRequestPriority();
				workStationList[i].position.x = wsPosX;
				workStationList[i].position.y = wsPosY;
				workStationList[i].remainingProduceTime = wsRemainingTime;
				workStationList[i].materialStatus = wsMaterialStatus;
				workStationList[i].producionStatus = wsProductionStatus;
				addRequest(workStationList[i]);
				addResource(workStationList[i]);
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
				if (preRemainingTime <= 0 && wsRemainingTime > 0) {
					addRequest(workStationList[i]);
					workStationList[i].resourcePushed = false;
				}
				addResource(workStationList[i]);
			}
		}
		
		// update Robot
		for (int i = 0; i < 4; i++) {
			// record prev status
			preLoadType = robot[i].loadType;

			// update
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

			// load type change
			updateRobotTaskStatus(robot[i], preLoadType);
		}
		cin >> strOK;

		//if (frameSeq % 50 == 0) {
		//	for (int i = 0; i < 4; i++) {
		//		if (robot[i].assignedTask.status == TO_START_POINT)
		//			fprintf(stderr, "[DEBUG] %s facing %.2f target %.2f\n", robot[i].toString().c_str(), robot[i].facing, calcAngle(robot[i].position, robot[i].assignedTask.start->position));
		//		else if (robot[i].assignedTask.status == TO_END_POINT)
		//			fprintf(stderr, "[DEBUG] %s facing %.2f target %.2f\n", robot[i].toString().c_str(), robot[i].facing, calcAngle(robot[i].position, robot[i].assignedTask.end->position));
		//	}
		//}

		// TODO: generate Task
		for (int i = 1; i <= 7; i++) {
			while (!resourceList[i].empty() && !requestList[i].empty()) {
				// new Task
				const Request& request = requestList[i].top();

				wsIter = resourceList[i].begin();
				nearestWs = *wsIter;
				minDistance = calcSquareDistance(nearestWs->position, request.requester->position);
				wsIter++;

				for (; wsIter != resourceList[i].end(); wsIter++) {
					dis = calcSquareDistance((*wsIter)->position, request.requester->position);
					if (dis < minDistance) {
						minDistance = dis;
						nearestWs = *wsIter;
					}
				}

				Task newTask(nearestWs, request.requester, request.priority);
				taskList.push(newTask);
				if (newTask.end->priority == 0) cerr << "[DEBUG] new " << newTask.toString() << endl;
				resourceList[i].erase(nearestWs);
				requestList[i].pop();
			}
		}

		// TODO: assign Task to idle robot
		idleRobot.clear();
		for (int i = 0; i < 4; i++) {
			if (robot[i].assignedTask.status == DONE) idleRobot.insert(&robot[i]);
			//fprintf(stderr, "[DEBUG] %s\n", robot[i].toString().c_str());
		}
		while (!idleRobot.empty() && !taskList.empty()) {
			const Task& task = taskList.top();

			robotIter = idleRobot.begin();
			nearestRobot = *robotIter;
			minDistance = calcSquareDistance(nearestRobot->position, task.start->position);
			robotIter++;

			for (; robotIter != idleRobot.end(); robotIter++) {
				dis = calcSquareDistance((*robotIter)->position, task.start->position);
				if (dis < minDistance) {
					minDistance = dis;
					nearestRobot = *robotIter;
				}
			}

			nearestRobot->assignedTask = task;
			nearestRobot->assignedTask.status = TO_START_POINT;
			fprintf(stderr, "[DEBUG] new Task assigned to %s\n", nearestRobot->toString().c_str());
			taskList.pop();
			idleRobot.erase(nearestRobot);
		}

		cout << frameSeq << endl;
		// TODO: generate robot code for output
		for (int i = 0; i < 4; i++) {
			printMotion(robot[i], workStationList);
		}

		cout << "OK\n";
		cin >> frameSeq;
	}
	delete[] workStationList;
	return 0;
}