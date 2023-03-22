#include<iostream>
#include<vector>
#include <tuple>
#include<fstream>
#include <string>
#include <cstdlib>
#include"Robot.h"
#include"WorkStation.h"
using namespace std;


// ������������
// �����±����λ�õĺ���
double getPosByIndex(int type, int index);
void readInData();
void calSecondLevelWorkStationCost();
// ��������㹤��̨�Ŀ���:7����ѡ�����7 ·��
int calThirdLevelWorkStationCostAndGetLowTask7();
// ��������7ID�ҵ�����·��
void getTotalPath(int task7ID);
// ���㵱ǰ�ڵ���¼��ڵ��б�������ĵ�
void getClosestLowerLevelStation(WorkStation& ws, vector<int> llSt);
// ����������֮��ľ���
double calDis(Position p1, Position p2);
// �ҵ�ĳһ�ڵ��һ��List�ڵ���������
void getCloestSellStation(WorkStation& ws, vector<int> sellWordStation8, vector<int> sellWordStation9);
// ����·��
void getPath();

// ȫ�ֱ�����������
// ����ID�����Robot�б�
 vector<Robot> robotVec;
// ����ID�����WorkStation�б�
 vector<WorkStation> workStationVec;
// �������ͷ��б��WorkStation�б�
 vector<int> stationList1, stationList2, stationList3, stationList4, stationList5, stationList6, stationList7, stationList8, stationList9;
 vector<vector<int>> stations = { stationList1, stationList2, stationList3, stationList4, stationList5, stationList6, stationList7, stationList8, stationList9 };

 vector<tuple<int, int>> path;


int main() {
	readInData();
	getPath();
	return 0;
}

void readInData() {
	ifstream infile;
	infile.open("C:/Users/N33/Desktop/WindowsRelease/WindowsRelease/maps/1.txt");
	if (!infile)
	{
		cout << "�޷����ļ���" << endl;
		exit(1);
	}

	int wordStationCount = 0;
	int robotCount = 0;
	int row = 0;
	string buff;
	while (getline(infile, buff))
	{
		// ��ʼ���ж�ȡ, ��line���д����ҵ�����Ϊ�����˻����ǹ���̨��Ԫ�أ����뵽������
		for (int col = 0; col < 100; col++) {
			// 
			if (buff[col] != '.') {
				if (buff[col] >= '0' && buff[col] <= '9') {
					// �ǹ���̨���鿴����̨����
					int type = buff[col] - '0';
					// ����洢����̶����б���
					WorkStation ws;
					ws.id = wordStationCount++;
					ws.type = type;
					// ���������������������ǰ��λ��
					double rowPos = getPosByIndex(0, row);
					double colPos = getPosByIndex(1, col);
					ws.pos = Position(colPos, rowPos);
					workStationVec.push_back(ws);
					stations[type - 1].push_back(ws.id);
				}
				else if (buff[col] == 'A') {
					// ����ǻ�����
					Robot rb;
					rb.id = robotCount++;
					double rowPos = getPosByIndex(0, row);
					double colPos = getPosByIndex(1, col);
					rb.pos = Position(colPos, rowPos);
					robotVec.push_back(rb);
				}
			}
		}

		row++;
	}

	infile.close();//�ر��ļ�

}

// �����±����λ�õĺ���
double getPosByIndex(int type, int index) {
	// ������к�
	if (type == 0) {
		double rowPos = 49.75 - (double)50 / 100 * index;
		return rowPos;
	}
	// �������
	else
	{
		double colPos = 0.25 + (double)50 / 100 * index;
		return colPos;
	}
}



// ����·��
void getPath() {
	calSecondLevelWorkStationCost();
	int task7ID = calThirdLevelWorkStationCostAndGetLowTask7();
	getTotalPath(task7ID);
}

// ����ڶ��㹤��̨�Ŀ���:4 5 6
void calSecondLevelWorkStationCost() {
	// ���ݳ�ʼ��
	vector<WorkStation> curWSVec;
	int* secondLevelWorkStation = new int[3]{4, 5, 6};
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
				getClosestLowerLevelStation(curSt, stations[0]);
				getClosestLowerLevelStation(curSt, stations[1]);
				break;
			}
			case 5: {
				// ��Ҫ1��3
				getClosestLowerLevelStation(curSt, stations[0]);
				getClosestLowerLevelStation(curSt, stations[2]);
				break;
			}
			case 6: {
				// ��Ҫ2��3
				getClosestLowerLevelStation(curSt, stations[1]);
				getClosestLowerLevelStation(curSt, stations[2]);
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
int calThirdLevelWorkStationCostAndGetLowTask7() {
	// ��������
	double lowestTotalCost = DBL_MAX;
	int lowestTotalCostTask7ID;
	// �����е�7Level�ģ�����Χ��������Ľڵ�
	for (int stP = 0; stP < stations[6].size(); stP++) {
		// ��Ҫ 4 5 6
		WorkStation& curSt = workStationVec[stations[6][stP]];
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
void getTotalPath(int task7ID) {
	// ������������һ�㼶���񣬽��Ŷ���һ��������һ��
	WorkStation& ws7 = workStationVec[task7ID];
	for (int l2TaskIDP = 0; l2TaskIDP < ws7.preWorkStationID.size(); l2TaskIDP++) {
		// �ҵ���������
		WorkStation& l2Task = workStationVec[ws7.preWorkStationID[l2TaskIDP]];
		// �ҵ����ĸ������ε�����
		WorkStation& l1Task1 = workStationVec[l2Task.preWorkStationID[0]];
		WorkStation& l1Task2 = workStationVec[l2Task.preWorkStationID[1]];
		// ������ӵ� level2Stations�� Path��
		path.push_back(make_tuple(l2Task.id, task7ID));
		path.push_back(make_tuple(l1Task1.id, l2Task.id));
		path.push_back(make_tuple(l1Task2.id, l2Task.id));
	}
	// ����sellWorkStation������Ҳ����
	path.push_back(make_tuple(task7ID, ws7.sellWorkStationID));
}


// ���㵱ǰ�ڵ���¼��ڵ��б�������ĵ�
void getClosestLowerLevelStation(WorkStation& ws, vector<int> llSt) {
	Position stPostion = ws.pos;
	Position tmpPostion;
	int closestPreWorkStationID;
	double minDis = INT_MAX;
	double curDis;
	// ����llSt�е�����Ԫ�أ��ҵ������
	for (int i = 0; i < llSt.size(); i++) {
		curDis = calDis(ws.pos, workStationVec[llSt[i]].pos);
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
void getCloestSellStation(WorkStation& ws, vector<int> sellWordStation8, vector<int> sellWordStation9) {
	Position stPostion = ws.pos;
	Position tmpPostion;
	int closestSellWorkStationID;
	double minDisWithSellStation = INT_MAX;
	double curDis;
	// ����sellWordStation8�е�����Ԫ�أ��ҵ������
	for (int i = 0; i < sellWordStation8.size(); i++) {
		curDis = calDis(ws.pos, workStationVec[sellWordStation8[i]].pos);
		if (curDis < minDisWithSellStation) {
			minDisWithSellStation = curDis;
			closestSellWorkStationID = workStationVec[sellWordStation8[i]].id;
		}
	}

	// ����sellWordStation9�е�����Ԫ�أ��ҵ������
	for (int i = 0; i < sellWordStation9.size(); i++) {
		curDis = calDis(ws.pos, workStationVec[sellWordStation9[i]].pos);
		if (curDis < minDisWithSellStation) {
			minDisWithSellStation = curDis;
			closestSellWorkStationID = workStationVec[sellWordStation9[i]].id;
		}
	}

	// ��������
	ws.cost += minDisWithSellStation;
	ws.sellWorkStationID = closestSellWorkStationID;
}

