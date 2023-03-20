
/*
#include<iostream>
#include<vector>
#include <tuple>
#include"WorkStation.h"
#include"Position.h"
#include"Robot.h"
using namespace std;


// ��������㹤��̨�Ŀ���:7����ѡ�����7 ·��
double calThirdLevelWorkStationCostAndGetLowTask7();
// ��������7ID�ҵ�����·��
vector<tuple<int, int>> getTotalPath(int task7ID);
// ���㵱ǰ�ڵ���¼��ڵ��б�������ĵ�
void getClosestLowerLevelStation(WorkStation ws, vector<WorkStation> llSt);
// ����������֮��ľ���
double calDis(Position p1, Position p2);
// �ҵ�ĳһ�ڵ��һ��List�ڵ���������
void getCloestSellStation(WorkStation ws, vector<WorkStation> sellWordStation8, vector<WorkStation> sellWordStation9);


// ����ID�����Robot�б�
extern vector<Robot> robotVec;
// ����ID�����WorkStation�б�
extern vector<WorkStation> workStationVec;
// �������ͷ��б��WorkStation�б�
extern vector<WorkStation> stationList1, stationList2, stationList3, stationList4, stationList5, stationList6, stationList7, stationList8, stationList9;
extern vector<vector<WorkStation>> stations = { stationList1, stationList2, stationList3, stationList4, stationList5, stationList6, stationList7, stationList8, stationList9 };


// ����ڶ��㹤��̨�Ŀ���:4 5 6
void calSecondLevelWorkStationCost() {



	// ���ݳ�ʼ��
	vector<WorkStation> curWSVec;
	WorkStation curSt;
	int* secondLevelWorkStation = new int[3] {4, 5, 6};
	// ����ÿһ������
	for (int t = 0; t < sizeof(secondLevelWorkStation); t++) {
		int type = secondLevelWorkStation[t];
		// ��������͵�ÿһ������̨
		curWSVec = stations[type - 1];
		for (int stP = 0; stP < sizeof(curWSVec); stP++) {
			// ����ù���̨���ӹ���̨�ľ��룬���ղ�ͬ���͵Ĺ���̨���ͣ��Ҳ�ͬ��������
			// ��ȡ��ǰ�Ľڵ�
			curSt = curWSVec[stP];
			switch (type){
				case 4: {
					// ��Ҫ1��2
					getClosestLowerLevelStation(curSt, stations[0]);
					getClosestLowerLevelStation(curSt, stations[1]);
				}
				case 5: {
					// ��Ҫ1��3
					getClosestLowerLevelStation(curSt, stations[0]);
					getClosestLowerLevelStation(curSt, stations[2]);
				}
				case 6: {
					// ��Ҫ2��3
					getClosestLowerLevelStation(curSt, stations[1]);
					getClosestLowerLevelStation(curSt, stations[2]);
				}
				default:
					break;
			}
			// ���ڵ�ǰ�ڵ㣬�ҵ���cost��С���ӽڵ㣬�洢������
		}
	}
}

// ��������㹤��̨�Ŀ���:7����ѡ�����7 ·��
double calThirdLevelWorkStationCostAndGetLowTask7() {
	// ��������
	double lowestTotalCost = DBL_MAX;
	double lowestTotalCostTask7ID;
	WorkStation curSt;
	// �����е�7Level�ģ�����Χ��������Ľڵ�
	for (int stP = 0; stP < stations[6].size(); stP++) {
		// ��Ҫ 4 5 6
		curSt = stations[6][stP];
		getClosestLowerLevelStation(curSt, stations[4 - 1]);
		getClosestLowerLevelStation(curSt, stations[5 - 1]);
		getClosestLowerLevelStation(curSt, stations[6 - 1]);
		// �����������8��9
		getCloestSellStation(curSt, stations[8 - 1], stations[9 - 1]);
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
vector<tuple<int, int>> getTotalPath(int task7ID) {
	vector<tuple<int, int>> path;
	// ������������һ�㼶���񣬽��Ŷ���һ��������һ��
	WorkStation ws7 = workStationVec[task7ID];
	for (int l2TaskIDP = 0; l2TaskIDP < ws7.preWorkStationID.size(); l2TaskIDP++) {
		// �ҵ���������
		WorkStation l2Task = workStationVec[l2TaskIDP];
		// �ҵ����ĸ������ε�����
		WorkStation l1Task1 = workStationVec[l2Task.preWorkStationID[0]];
		WorkStation l1Task2 = workStationVec[l2Task.preWorkStationID[1]];
		// ������ӵ� level2Stations�� Path��
		path.push_back(make_tuple(l2Task.id, task7ID));
		path.push_back(make_tuple(l1Task1.id, l2Task.id));
		path.push_back(make_tuple(l1Task2.id, l2Task.id));
	}
	// ����sellWorkStation������Ҳ����
	path.push_back(make_tuple(task7ID, ws7.sellWorkStationID));
	return path;
}


// ���㵱ǰ�ڵ���¼��ڵ��б�������ĵ�
void getClosestLowerLevelStation(WorkStation ws, vector<WorkStation> llSt) {
	Position stPostion = ws.pos;
	Position tmpPostion;
	int closestPreWorkStationID;
	double minDis = INT_MAX;
	double curDis;
	// ����llSt�е�����Ԫ�أ��ҵ������
	for (int i = 0; i < llSt.size(); i++) {
		curDis = calDis(ws.pos, llSt[i].pos);
		if (curDis < minDis) {
			minDis = curDis;
			closestPreWorkStationID = llSt[i].id;
		}
	}
	// ���µ�ǰ����ڵ������ӹ���̨�����Ĵ���
	ws.cost += minDis;
	ws.preWorkStationID.push_back(closestPreWorkStationID);
}

double calDis(Position p1, Position p2) {
	return sqrt((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y)*(p1.y - p2.y));
}

// �ҵ�ĳһ�ڵ��һ��List�ڵ���������
void getCloestSellStation(WorkStation ws, vector<WorkStation> sellWordStation8, vector<WorkStation> sellWordStation9) {
	Position stPostion = ws.pos;
	Position tmpPostion;
	int closestSellWorkStationID;
	double minDisWithSellStation = INT_MAX;
	double curDis;
	// ����sellWordStation8�е�����Ԫ�أ��ҵ������
	for (int i = 0; i < sellWordStation8.size(); i++) {
		curDis = calDis(ws.pos, sellWordStation8[i].pos);
		if (curDis < minDisWithSellStation) {
			minDisWithSellStation = curDis;
			closestSellWorkStationID = sellWordStation8[i].id;
		}
	}
	
	// ����sellWordStation9�е�����Ԫ�أ��ҵ������
	for (int i = 0; i < sellWordStation9.size(); i++) {
		curDis = calDis(ws.pos, sellWordStation9[i].pos);
		if (curDis < minDisWithSellStation) {
			minDisWithSellStation = curDis;
			closestSellWorkStationID = sellWordStation9[i].id;
		}
	}

	// ��������
	ws.cost += minDisWithSellStation;
	ws.sellWorkStationID = closestSellWorkStationID;
}



*/