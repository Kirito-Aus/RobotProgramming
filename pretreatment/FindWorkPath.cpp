
/*
#include<iostream>
#include<vector>
#include <tuple>
#include"WorkStation.h"
#include"Position.h"
#include"Robot.h"
using namespace std;


// 计算第三层工作台的开销:7，并选择具体7 路线
double calThirdLevelWorkStationCostAndGetLowTask7();
// 根据任务7ID找到完整路线
vector<tuple<int, int>> getTotalPath(int task7ID);
// 计算当前节点和下级节点列表中最近的点
void getClosestLowerLevelStation(WorkStation ws, vector<WorkStation> llSt);
// 计算两个点之间的距离
double calDis(Position p1, Position p2);
// 找到某一节点和一个List节点的最近距离
void getCloestSellStation(WorkStation ws, vector<WorkStation> sellWordStation8, vector<WorkStation> sellWordStation9);


// 根据ID排序的Robot列表
extern vector<Robot> robotVec;
// 根据ID排序的WorkStation列表
extern vector<WorkStation> workStationVec;
// 按照类型分列表的WorkStation列表
extern vector<WorkStation> stationList1, stationList2, stationList3, stationList4, stationList5, stationList6, stationList7, stationList8, stationList9;
extern vector<vector<WorkStation>> stations = { stationList1, stationList2, stationList3, stationList4, stationList5, stationList6, stationList7, stationList8, stationList9 };


// 计算第二层工作台的开销:4 5 6
void calSecondLevelWorkStationCost() {



	// 数据初始化
	vector<WorkStation> curWSVec;
	WorkStation curSt;
	int* secondLevelWorkStation = new int[3] {4, 5, 6};
	// 对于每一种类型
	for (int t = 0; t < sizeof(secondLevelWorkStation); t++) {
		int type = secondLevelWorkStation[t];
		// 处理该类型的每一个工作台
		curWSVec = stations[type - 1];
		for (int stP = 0; stP < sizeof(curWSVec); stP++) {
			// 计算该工作台和子工作台的距离，按照不同类型的工作台类型，找不同的子任务
			// 获取当前的节点
			curSt = curWSVec[stP];
			switch (type){
				case 4: {
					// 需要1、2
					getClosestLowerLevelStation(curSt, stations[0]);
					getClosestLowerLevelStation(curSt, stations[1]);
				}
				case 5: {
					// 需要1、3
					getClosestLowerLevelStation(curSt, stations[0]);
					getClosestLowerLevelStation(curSt, stations[2]);
				}
				case 6: {
					// 需要2、3
					getClosestLowerLevelStation(curSt, stations[1]);
					getClosestLowerLevelStation(curSt, stations[2]);
				}
				default:
					break;
			}
			// 对于当前节点，找到了cost最小的子节点，存储在里面
		}
	}
}

// 计算第三层工作台的开销:7，并选择具体7 路线
double calThirdLevelWorkStationCostAndGetLowTask7() {
	// 参数定义
	double lowestTotalCost = DBL_MAX;
	double lowestTotalCostTask7ID;
	WorkStation curSt;
	// 对所有的7Level的，找周围的子任务的节点
	for (int stP = 0; stP < stations[6].size(); stP++) {
		// 需要 4 5 6
		curSt = stations[6][stP];
		getClosestLowerLevelStation(curSt, stations[4 - 1]);
		getClosestLowerLevelStation(curSt, stations[5 - 1]);
		getClosestLowerLevelStation(curSt, stations[6 - 1]);
		// 找离它最近的8、9
		getCloestSellStation(curSt, stations[8 - 1], stations[9 - 1]);
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
vector<tuple<int, int>> getTotalPath(int task7ID) {
	vector<tuple<int, int>> path;
	// 首先找它的下一层级任务，接着对下一层再找下一层
	WorkStation ws7 = workStationVec[task7ID];
	for (int l2TaskIDP = 0; l2TaskIDP < ws7.preWorkStationID.size(); l2TaskIDP++) {
		// 找到下游任务
		WorkStation l2Task = workStationVec[l2TaskIDP];
		// 找到它的更加下游的任务
		WorkStation l1Task1 = workStationVec[l2Task.preWorkStationID[0]];
		WorkStation l1Task2 = workStationVec[l2Task.preWorkStationID[1]];
		// 将其添加到 level2Stations、 Path中
		path.push_back(make_tuple(l2Task.id, task7ID));
		path.push_back(make_tuple(l1Task1.id, l2Task.id));
		path.push_back(make_tuple(l1Task2.id, l2Task.id));
	}
	// 将到sellWorkStation的内容也加入
	path.push_back(make_tuple(task7ID, ws7.sellWorkStationID));
	return path;
}


// 计算当前节点和下级节点列表中最近的点
void getClosestLowerLevelStation(WorkStation ws, vector<WorkStation> llSt) {
	Position stPostion = ws.pos;
	Position tmpPostion;
	int closestPreWorkStationID;
	double minDis = INT_MAX;
	double curDis;
	// 遍历llSt中的所有元素，找到最近的
	for (int i = 0; i < llSt.size(); i++) {
		curDis = calDis(ws.pos, llSt[i].pos);
		if (curDis < minDis) {
			minDis = curDis;
			closestPreWorkStationID = llSt[i].id;
		}
	}
	// 更新当前计算节点的最近子工作台和它的代价
	ws.cost += minDis;
	ws.preWorkStationID.push_back(closestPreWorkStationID);
}

double calDis(Position p1, Position p2) {
	return sqrt((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y)*(p1.y - p2.y));
}

// 找到某一节点和一个List节点的最近距离
void getCloestSellStation(WorkStation ws, vector<WorkStation> sellWordStation8, vector<WorkStation> sellWordStation9) {
	Position stPostion = ws.pos;
	Position tmpPostion;
	int closestSellWorkStationID;
	double minDisWithSellStation = INT_MAX;
	double curDis;
	// 遍历sellWordStation8中的所有元素，找到最近的
	for (int i = 0; i < sellWordStation8.size(); i++) {
		curDis = calDis(ws.pos, sellWordStation8[i].pos);
		if (curDis < minDisWithSellStation) {
			minDisWithSellStation = curDis;
			closestSellWorkStationID = sellWordStation8[i].id;
		}
	}
	
	// 遍历sellWordStation9中的所有元素，找到最近的
	for (int i = 0; i < sellWordStation9.size(); i++) {
		curDis = calDis(ws.pos, sellWordStation9[i].pos);
		if (curDis < minDisWithSellStation) {
			minDisWithSellStation = curDis;
			closestSellWorkStationID = sellWordStation9[i].id;
		}
	}

	// 更新数据
	ws.cost += minDisWithSellStation;
	ws.sellWorkStationID = closestSellWorkStationID;
}



*/