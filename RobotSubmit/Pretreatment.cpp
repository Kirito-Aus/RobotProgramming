#include "Pretreatment.h"
#include<iostream>
#include<vector>
#include <tuple>
#include<fstream>
#include <string>
#include <cstdlib>
#include<vector>
#include"Robot.h"
#include"WorkStation.h"
#include"Pretreatment.h"
using namespace std;

double calDis(Position p1, Position p2);
void getClosestLowerLevelStation(WorkStation& ws, vector<int> &llSt, vector<WorkStation> &workStationVec);
void getTotalPath(int task7ID, vector<WorkStation> &workStationVec, vector<tuple<int, int>> &path);
void getCloestSellStation(WorkStation& ws, vector<int> &sellWordStation8, vector<int> &sellWordStation9, vector<WorkStation> &workStationVec);
void calSecondLevelWorkStationCost(vector<vector<int>> &stations, vector<WorkStation> &workStationVec);
int calThirdLevelWorkStationCostAndGetLowTask7(vector<vector<int>> &stations, vector<WorkStation> &workStationVec);

bool readInStartData(vector<Robot> &robotVec, vector<WorkStation> &workStationVec, vector<vector<int>> &stations) {
	int wordStationCount = 0;
	int row = 0;
	int robotCount = 0;
	char line[1024];
	while (fgets(line, sizeof line, stdin)) {
		if (line[0] == 'O' && line[1] == 'K') {
			return true;
		}
		// 处理地图数据
		// 开始逐行读取, 对line进行处理，找到里面为机器人或者是工作台的元素，放入到集合中
		for (int col = 0; col < 100; col++) {
			// 
			if (line[col] != '.') {
				if (line[col] >= '0' && line[col] <= '9') {
					// 是工作台，查看工作台类型
					int type = line[col] - '0';
					// 将其存储进入固定的列表中
					WorkStation ws;
					ws.id = wordStationCount++;
					ws.type = type;
					// 根据行数和列数计算出当前的位置
					double rowPos = getPosByIndex(0, row);
					double colPos = getPosByIndex(1, col);
					ws.position = Position(colPos, rowPos);
					workStationVec.push_back(ws);
					stations[type - 1].push_back(ws.id);
				}
				else if (line[col] == 'A') {
					// 如果是机器人
					Robot rb;
					rb.id = robotCount++;
					double rowPos = getPosByIndex(0, row);
					double colPos = getPosByIndex(1, col);
					rb.position = Position(colPos, rowPos);
					robotVec.push_back(rb);
				}
			}
		}

		row++;
	}
	return false;
}


// 根据下标计算位置的函数
double getPosByIndex(int type, int index) {
	// 如果是行号
	if (type == 0) {
		double rowPos = 49.75 - (double)50 / 100 * index;
		return rowPos;
	}
	// 如果是列
	else
	{
		double colPos = 0.25 + (double)50 / 100 * index;
		return colPos;
	}
}



/****计算路径*****/
// 计算路径
void getPath(vector<vector<int>> &stations, vector<WorkStation> &workStationVec, vector<tuple<int, int>> &path) {
	calSecondLevelWorkStationCost(stations, workStationVec);
	int task7ID = calThirdLevelWorkStationCostAndGetLowTask7(stations, workStationVec);
	getTotalPath(task7ID,workStationVec,path);
}

// 计算第二层工作台的开销:4 5 6
void calSecondLevelWorkStationCost(vector<vector<int>> &stations, vector<WorkStation> &workStationVec) {
	// 数据初始化
	vector<WorkStation> curWSVec;
	int* secondLevelWorkStation = new int[3] {4, 5, 6};
	// 对于每一种类型
	for (int t = 0; t < 3; t++) {
		int type = secondLevelWorkStation[t];
		// 处理该类型的每一个工作台
		for (int stP = 0; stP < stations[type - 1].size(); stP++) {
			// 计算该工作台和子工作台的距离，按照不同类型的工作台类型，找不同的子任务
			// 获取当前的节点
			WorkStation& curSt = workStationVec[stations[type - 1][stP]];
			switch (type) {
			case 4: {
				// 需要1、2
				getClosestLowerLevelStation(curSt, stations[0], workStationVec);
				getClosestLowerLevelStation(curSt, stations[1], workStationVec);
				break;
			}
			case 5: {
				// 需要1、3
				getClosestLowerLevelStation(curSt, stations[0], workStationVec);
				getClosestLowerLevelStation(curSt, stations[2], workStationVec);
				break;
			}
			case 6: {
				// 需要2、3
				getClosestLowerLevelStation(curSt, stations[1], workStationVec);
				getClosestLowerLevelStation(curSt, stations[2], workStationVec);
				break;
			}
			default:
				break;
			}
			// 对于当前节点，找到了cost最小的子节点，存储在里面
		}
	}
}

// 计算第三层工作台的开销:7，并选择具体7 路线
int calThirdLevelWorkStationCostAndGetLowTask7(vector<vector<int>> &stations, vector<WorkStation> &workStationVec) {
	// 参数定义
	double lowestTotalCost = DBL_MAX;
	int lowestTotalCostTask7ID;
	// 对所有的7Level的，找周围的子任务的节点
	for (int stP = 0; stP < stations[6].size(); stP++) {
		// 需要 4 5 6
		WorkStation& curSt = workStationVec[stations[6][stP]];
		getClosestLowerLevelStation(curSt, stations[4 - 1], workStationVec);
		getClosestLowerLevelStation(curSt, stations[5 - 1], workStationVec);
		getClosestLowerLevelStation(curSt, stations[6 - 1], workStationVec);
		// 找离它最近的8、9
		getCloestSellStation(curSt, stations[8 - 1], stations[9 - 1], workStationVec);
		// 判断是否为最小代价7
		if (curSt.cost < lowestTotalCost) {
			lowestTotalCost = curSt.cost;
			lowestTotalCostTask7ID = curSt.id;
		}
	}
	// 最后在所有的7中找代价最小的返回ID
	return lowestTotalCostTask7ID;
}

// 根据任务7ID找到完整路线
void getTotalPath(int task7ID, vector<WorkStation> &workStationVec, vector<tuple<int, int>> &path) {
	// 首先找它的下一层级任务，接着对下一层再找下一层
	WorkStation& ws7 = workStationVec[task7ID];
	for (int l2TaskIDP = 0; l2TaskIDP < ws7.preWorkStationID.size(); l2TaskIDP++) {
		// 找到下游任务
		WorkStation& l2Task = workStationVec[ws7.preWorkStationID[l2TaskIDP]];
		// 找到它的更加下游的任务
		WorkStation& l1Task1 = workStationVec[l2Task.preWorkStationID[0]];
		WorkStation& l1Task2 = workStationVec[l2Task.preWorkStationID[1]];
		// 将其添加到 level2Stations、 Path中
		path.push_back(make_tuple(l2Task.id, task7ID));
		path.push_back(make_tuple(l1Task1.id, l2Task.id));
		path.push_back(make_tuple(l1Task2.id, l2Task.id));
	}
	// 将到sellWorkStation的内容也加入
	path.push_back(make_tuple(task7ID, ws7.sellWorkStationID));
}


// 计算当前节点和下级节点列表中最近的点
void getClosestLowerLevelStation(WorkStation& ws, vector<int> &llSt, vector<WorkStation> &workStationVec) {
	Position stPostion = ws.position;
	Position tmpPostion;
	int closestPreWorkStationID;
	double minDis = INT_MAX;
	double curDis;
	// 遍历llSt中的所有元素，找到最近的
	for (int i = 0; i < llSt.size(); i++) {
		curDis = calDis(ws.position, workStationVec[llSt[i]].position);
		if (curDis < minDis) {
			minDis = curDis;
			closestPreWorkStationID = workStationVec[llSt[i]].id;
		}
	}
	// 更新当前计算节点的最近子工作台和它的代价
	ws.cost += minDis;
	ws.preWorkStationID.push_back(closestPreWorkStationID);
}

double calDis(Position p1, Position p2) {
	return (double)sqrt((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y));
}

// 找到某一节点和一个List节点的最近距离
void getCloestSellStation(WorkStation& ws, vector<int> &sellWordStation8, vector<int> &sellWordStation9, vector<WorkStation> &workStationVec) {
	Position stPostion = ws.position;
	Position tmpPostion;
	int closestSellWorkStationID = -1;
	double minDisWithSellStation = INT_MAX;
	double curDis;
	// 遍历sellWordStation8中的所有元素，找到最近的
	for (int i = 0; i < sellWordStation8.size(); i++) {
		curDis = calDis(ws.position, workStationVec[sellWordStation8[i]].position);
		if (curDis < minDisWithSellStation) {
			minDisWithSellStation = curDis;
			closestSellWorkStationID = workStationVec[sellWordStation8[i]].id;
		}
	}

	// 遍历sellWordStation9中的所有元素，找到最近的
	for (int i = 0; i < sellWordStation9.size(); i++) {
		curDis = calDis(ws.position, workStationVec[sellWordStation9[i]].position);
		if (curDis < minDisWithSellStation) {
			minDisWithSellStation = curDis;
			closestSellWorkStationID = workStationVec[sellWordStation9[i]].id;
		}
	}

	// 更新数据
	ws.cost += minDisWithSellStation;
	ws.sellWorkStationID = closestSellWorkStationID;
}


/****读取每一帧的内容*****/
bool getFrameInput(int &frameId, int &price) {
	char line[1024];
	// 先获取第一行，第一行输入 2 个整数，表示帧序号（从 1 开始递增）、当前金钱数
	cin >> frameId >> price;
	// 再获取第二行,表示场上工作台的数量 K
	int k;
	cin >> k;
	// 紧接着 K 行数据，每一行表示一个工作台
	int type, leftFrame, initialState, ProState;
	float x, y;
	
	for (int i = 0; i < k; i++) {
		cin >> type >> x >> y >> leftFrame >> initialState >> ProState;
	}
	// 接下来的 4 行数据，每一行表示一个机器人
	int workStationId, withThingType;
	float timeValue, collideValue, angleRate, linerRate, toward, rb_x, rb_y;
	for (int i = 0; i < 4; i++) {
		cin >> workStationId >> withThingType >> timeValue >> collideValue >> angleRate >> linerRate >> toward >> rb_x >> rb_y;
	}

	string okStr;
	cin >> okStr;
	if (okStr.compare("OK")) {
		// 如果最后输出的是ok，则认为正确
		return true;
	}
	return false;
}