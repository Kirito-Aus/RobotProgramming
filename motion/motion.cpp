#include <iostream>
#include <algorithm>
#include <queue>
#include<cmath>
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

//output the motion of a robot
void motion(int robotNum, int workStationNum) {
	
	float distX = (robot[robotNum].position.x - workStationList[workStationNum].position.x);
	float distY = (robot[robotNum].position.y - workStationList[workStationNum].position.y);
	float dist = sqrt(distX*distX + distY*distY);//the ditance between robot and workStation
	float angle = atan2(distY, distX);
	if (dist <= 0.4) {
		if (robot[robotNum].loadType==0 && workStationList[workStationNum].producionStatus == 1) {
			cout << "buy " << robotNum << endl;
		}
		if (robot[robotNum].loadType != 0) {
			cout << "sell " << robotNum << endl;
		}
	}
	else {
		if (robot[robotNum].facing != angle)
			cout << "rotate " << robotNum << " " << angle - robot[robotNum].facing << endl;
		else {
			cout << "foward " << robotNum << " " << 6 << endl;
		}
	}
}