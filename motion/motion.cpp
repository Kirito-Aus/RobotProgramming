#include <iostream>
#include <algorithm>
#include <queue>
#include<cmath>
#include "WorkStation.h"
#include "Robot.h"
#include "Task.h"
#define PI 3.1415926
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
		if (robot[robotNum].facing != angle) {
			float deltaAngle = angle - robot[robotNum].facing;
			if (abs(deltaAngle) <= PI / 2 * 0.02) 
				cout << "rotate " << robotNum << " " << deltaAngle /0.02 << endl;
			else
				cout << "rotate " << robotNum << " " << (deltaAngle > 0 ? PI/2: -PI/2 )<< endl;
		}	
		else {
				cout << "forward " << robotNum << " " << 6 << endl;
		}
	}
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