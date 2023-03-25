#include "Pretreatment.h"
#include<iostream>
#include<vector>
#include <tuple>
#include<fstream>
#include <string>
#include <cstdlib>
#include<vector>
using namespace std;

double calDis(Position p1, Position p2);
void getClosestLowerLevelStation(WorkStation& ws, vector<int>& llSt, vector<WorkStation>& workStationVec);
void getTotalPath(int task7ID, vector<WorkStation>& workStationVec, vector<tuple<int, int>>& path, vector<int>& taskStations);
void getCloestSellStation(WorkStation& ws, vector<int>& sellWordStation8, vector<int>& sellWordStation9, vector<WorkStation>& workStationVec);
void calSecondLevelWorkStationCost(vector<vector<int>>& stations, vector<WorkStation>& workStationVec);
int calThirdLevelWorkStationCostAndGetLowTask7(vector<vector<int>>& stations, vector<WorkStation>& workStationVec);
float getPosByIndex(int index);

bool readInStartData(vector<Robot>& robotVec, vector<WorkStation>& workStationVec, vector<vector<int>>& stations) {
	fprintf(stderr, "[DEBUG] Start Input Map");
	int wordStationCount = 0;
	int row = 0;
	int robotCount = 0;
	char line[1024];
	while (fgets(line, sizeof line, stdin)) {
		if (line[0] == 'O' && line[1] == 'K') {
			return true;
		}
		// �����ͼ����
		// ��ʼ���ж�ȡ, ��line���д����ҵ�����Ϊ�����˻����ǹ���̨��Ԫ�أ����뵽������
		for (int col = 0; col < 100; col++) {
			// 
			if (line[col] != '.') {
				if (line[col] >= '0' && line[col] <= '9') {
					// �ǹ���̨���鿴����̨����
					int type = line[col] - '0';
					// ����洢����̶����б���
					WorkStation ws;
					ws.id = wordStationCount++;
					ws.type = type;
					CoordinatePos cPos(99 - row, col);
					ws.cPos = cPos;
					// ���������������������ǰ��λ��
					float rowPos = getPosByIndex(99-row);
					float colPos = getPosByIndex(col);
					ws.position = Position(colPos, rowPos);
					workStationVec.push_back(ws);
					stations[type - 1].push_back(ws.id);
					fprintf(stderr, "[DEBUG] pretreat add %s into workStationVec\n", ws.pretreatToString().c_str());
				}
				else if (line[col] == 'A') {
					// ����ǻ�����
					Robot rb;
					rb.id = robotCount++;
					float rowPos = getPosByIndex(0, 99 - row);
					float colPos = getPosByIndex(1, col);
					rb.position = Position(colPos, rowPos);
					robotVec.push_back(rb);
					fprintf(stderr, "[DEBUG] pretreat add %s into robotVec\n", rb.toString().c_str());
				}
			}
		}


		row++;
	}
	return false;
}

float getPosByIndex(int index) {
	float pos = (float)0.25 + (float)50 / 100 * index;
	return pos;
}

// �����±����λ�õĺ���
float getPosByIndex(int type, int index) {
	// ������к�
	if (type == 0) {
		float rowPos = (float)49.75 - (float)50 / 100 * index;
		return rowPos;
	}
	// �������
	else
	{
		float colPos = (float)0.25 + (float)50 / 100 * index;
		return colPos;
	}
}




/****����·��*****/
// ����·��
void getPath(vector<vector<int>>& stations, vector<WorkStation>& workStationVec, vector<tuple<int, int>>& path, vector<int>& taskStations) {
	fprintf(stderr, "[DEBUG] Start Calculation Path\n");
	calSecondLevelWorkStationCost(stations, workStationVec);
	int task7ID = calThirdLevelWorkStationCostAndGetLowTask7(stations, workStationVec);
	getTotalPath(task7ID, workStationVec, path, taskStations);
}

// ����ڶ��㹤��̨�Ŀ���:4 5 6
void calSecondLevelWorkStationCost(vector<vector<int>>& stations, vector<WorkStation>& workStationVec) {
	// ���ݳ�ʼ��
	vector<WorkStation> curWSVec;
	int* secondLevelWorkStation = new int[3] {4, 5, 6};
	// ����ÿһ������
	for (int t = 0; t < 3; t++) {
		int type = secondLevelWorkStation[t];
		// ��������͵�ÿһ������̨
		for (int stP = 0; stP < stations[type - 1].size(); stP++) {
			// ����ù���̨���ӹ���̨�ľ��룬���ղ�ͬ���͵Ĺ���̨���ͣ��Ҳ�ͬ��������
			// ��ȡ��ǰ�Ľڵ�
			WorkStation& curSt = workStationVec[stations[type - 1][stP]];
			switch (type) {
			case 4: {
				// ��Ҫ1��2
				getClosestLowerLevelStation(curSt, stations[0], workStationVec);
				getClosestLowerLevelStation(curSt, stations[1], workStationVec);
				break;
			}
			case 5: {
				// ��Ҫ1��3
				getClosestLowerLevelStation(curSt, stations[0], workStationVec);
				getClosestLowerLevelStation(curSt, stations[2], workStationVec);
				break;
			}
			case 6: {
				// ��Ҫ2��3
				getClosestLowerLevelStation(curSt, stations[1], workStationVec);
				getClosestLowerLevelStation(curSt, stations[2], workStationVec);
				break;
			}
			default:
				break;
			}
			// ���ڵ�ǰ�ڵ㣬�ҵ���cost��С���ӽڵ㣬�洢������
		}
	}
}

// ��������㹤��̨�Ŀ���:7����ѡ�����7 ·��
int calThirdLevelWorkStationCostAndGetLowTask7(vector<vector<int>>& stations, vector<WorkStation>& workStationVec) {
	// ��������
	double lowestTotalCost = DBL_MAX;
	int lowestTotalCostTask7ID = -1;
	// �����е�7Level�ģ�����Χ��������Ľڵ�
	for (int stP = 0; stP < stations[6].size(); stP++) {
		// ��Ҫ 4 5 6
		WorkStation& curSt = workStationVec[stations[6][stP]];
		getClosestLowerLevelStation(curSt, stations[4 - 1], workStationVec);
		getClosestLowerLevelStation(curSt, stations[5 - 1], workStationVec);
		getClosestLowerLevelStation(curSt, stations[6 - 1], workStationVec);
		// �����������8��9
		getCloestSellStation(curSt, stations[8 - 1], stations[9 - 1], workStationVec);
		// �ж��Ƿ�Ϊ��С����7
		if (curSt.cost < lowestTotalCost) {
			lowestTotalCost = curSt.cost;
			lowestTotalCostTask7ID = curSt.id;
		}
	}
	// ��������е�7���Ҵ�����С�ķ���ID
	return lowestTotalCostTask7ID;
}

// ��������7ID�ҵ�����·��
void getTotalPath(int task7ID, vector<WorkStation>& workStationVec, vector<tuple<int, int>>& path, vector<int>& taskStations) {
	// ������������һ�㼶���񣬽��Ŷ���һ��������һ��
	WorkStation& ws7 = workStationVec[task7ID];
	for (int l2TaskIDP = 0; l2TaskIDP < ws7.preWorkStationID.size(); l2TaskIDP++) {
		// �ҵ���������
		WorkStation& l2Task = workStationVec[ws7.preWorkStationID[l2TaskIDP]];
		// �ҵ����ĸ������ε�����
		WorkStation& l1Task1 = workStationVec[l2Task.preWorkStationID[0]];
		WorkStation& l1Task2 = workStationVec[l2Task.preWorkStationID[1]];
		taskStations.push_back(l2Task.id);
		taskStations.push_back(l1Task1.id);
		taskStations.push_back(l1Task2.id);
		// ������ӵ� level2Stations�� Path��
		path.push_back(make_tuple(l2Task.id, task7ID));
		path.push_back(make_tuple(l1Task1.id, l2Task.id));
		path.push_back(make_tuple(l1Task2.id, l2Task.id));
		fprintf(stderr, "[DEBUG] Add Pretreatment Level2 Task %s\n", l2Task.toString().c_str());
		fprintf(stderr, "[DEBUG] Add Pretreatment Level1_1 Task %s\n", l1Task1.toString().c_str());
		fprintf(stderr, "[DEBUG] Add Pretreatment Level1_2 Task %s\n", l1Task2.toString().c_str());
	}
	// ����sellWorkStation������Ҳ����
	taskStations.push_back(ws7.id);
	path.push_back(make_tuple(task7ID, ws7.sellWorkStationID));
	fprintf(stderr, "[DEBUG] Add Pretreatment Level3 Task %s\n", ws7.toString().c_str());
}


// ���㵱ǰ�ڵ���¼��ڵ��б�������ĵ�
void getClosestLowerLevelStation(WorkStation& ws, vector<int>& llSt, vector<WorkStation>& workStationVec) {
	Position stPostion = ws.position;
	Position tmpPostion;
	int closestPreWorkStationID;
	double minDis = INT_MAX;
	double curDis;
	// ����llSt�е�����Ԫ�أ��ҵ������
	for (int i = 0; i < llSt.size(); i++) {
		curDis = calDis(ws.position, workStationVec[llSt[i]].position);
		if (curDis < minDis) {
			minDis = curDis;
			closestPreWorkStationID = workStationVec[llSt[i]].id;
		}
	}
	// ���µ�ǰ����ڵ������ӹ���̨�����Ĵ���
	ws.cost += minDis;
	ws.preWorkStationID.push_back(closestPreWorkStationID);
}

double calDis(Position p1, Position p2) {
	return (double)sqrt((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y));
}

// �ҵ�ĳһ�ڵ��һ��List�ڵ���������
void getCloestSellStation(WorkStation& ws, vector<int>& sellWordStation8, vector<int>& sellWordStation9, vector<WorkStation>& workStationVec) {
	Position stPostion = ws.position;
	Position tmpPostion;
	int closestSellWorkStationID = -1;
	double minDisWithSellStation = INT_MAX;
	double curDis;
	// ����sellWordStation8�е�����Ԫ�أ��ҵ������
	for (int i = 0; i < sellWordStation8.size(); i++) {
		curDis = calDis(ws.position, workStationVec[sellWordStation8[i]].position);
		if (curDis < minDisWithSellStation) {
			minDisWithSellStation = curDis;
			closestSellWorkStationID = workStationVec[sellWordStation8[i]].id;
		}
	}

	// ����sellWordStation9�е�����Ԫ�أ��ҵ������
	for (int i = 0; i < sellWordStation9.size(); i++) {
		curDis = calDis(ws.position, workStationVec[sellWordStation9[i]].position);
		if (curDis < minDisWithSellStation) {
			minDisWithSellStation = curDis;
			closestSellWorkStationID = workStationVec[sellWordStation9[i]].id;
		}
	}

	// ��������
	ws.cost += minDisWithSellStation;
	ws.sellWorkStationID = closestSellWorkStationID;
}


/****��ȡÿһ֡������*****/
bool getFrameInput(int& frameId, int& price) {
	// �Ȼ�ȡ��һ�У���һ������ 2 ����������ʾ֡��ţ��� 1 ��ʼ����������ǰ��Ǯ��
	cin >> frameId >> price;
	// �ٻ�ȡ�ڶ���,��ʾ���Ϲ���̨������ K
	int k;
	cin >> k;
	// ������ K �����ݣ�ÿһ�б�ʾһ������̨
	int type, leftFrame, initialState, ProState;
	float x, y;

	for (int i = 0; i < k; i++) {
		cin >> type >> x >> y >> leftFrame >> initialState >> ProState;
	}
	// �������� 4 �����ݣ�ÿһ�б�ʾһ��������
	int workStationId, withThingType;
	float timeValue, collideValue, angleRate, linerRate, toward, rb_x, rb_y;
	for (int i = 0; i < 4; i++) {
		cin >> workStationId >> withThingType >> timeValue >> collideValue >> angleRate >> linerRate >> toward >> rb_x >> rb_y;
	}

	string okStr;
	cin >> okStr;
	if (okStr.compare("OK")) {
		// �������������ok������Ϊ��ȷ
		return true;
	}
	return false;
}