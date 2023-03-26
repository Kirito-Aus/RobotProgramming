#include "Pretreatment.h"
#include <iostream>
#include <vector>
#include <tuple>
#include <fstream>
#include <string>
#include<algorithm>
#include <cstdlib>
#include <vector>
#include <cmath>
#include <cfloat>
using namespace std;


double calDis(Position p1, Position p2);
void getClosestLowerLevelStation(WorkStation& ws, vector<int>& llSt, vector<WorkStation>& workStationVec);
void getTotalPath(int task7ID, vector<WorkStation>& workStationVec, vector<tuple<int, int>>& path, vector<int>& taskStations);
void getCloestSellStation(WorkStation& ws, vector<int>& sellWordStation8, vector<int>& sellWordStation9, vector<WorkStation>& workStationVec);
void calSecondLevelWorkStationCost(vector<vector<int>>& stations, vector<WorkStation>& workStationVec);
int calThirdLevelWorkStationCostAndGetLowTask7(vector<vector<int>>& stations, vector<WorkStation>& workStationVec);
float getPosByIndex(int index);
bool existFixedLevelStation(vector<vector<int>>& stations, int level);
// 如果在没有7的情况下，则调用以下方式来获取第二层的任务，对于第二层的任务，我们选择最好实现的前3个作为需要调度的内容
bool calTasksWithOutStation7(vector<vector<int>>& stations, vector<int>& busyStation, vector<WorkStation>& workStationVec, vector<int>& secondLevelTask);
// 获取没有工作台7的第二层工作台的工作任务
void getTotalPathWithout7(vector<int>& secondLevelTask, vector<WorkStation>& workStationVec, vector<int>& taskStations);
// 获取所有第二层节点到出售它们的节点的距离——初始化时，只做一次
void getSecondLevelStationSellStation(vector<WorkStation>& workStationVec, vector<vector<int>>& stations, vector<int>& sellWordStation9);


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
					CoordinatePos cPos(99 - row, col);
					ws.cPos = cPos;
					// 根据行数和列数计算出当前的位置
					float rowPos = getPosByIndex(99-row);
					float colPos = getPosByIndex(col);
					ws.position = Position(colPos, rowPos);
					workStationVec.push_back(ws);
					stations[type - 1].push_back(ws.id);
					fprintf(stderr, "[DEBUG] pretreat add %s into workStationVec\n", ws.pretreatToString().c_str());
				}
				else if (line[col] == 'A') {
					// 如果是机器人
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

// 根据下标计算位置的函数
float getPosByIndex(int type, int index) {
	// 如果是行号
	if (type == 0) {
		float rowPos = (float)49.75 - (float)50 / 100 * index;
		return rowPos;
	}
	// 如果是列
	else
	{
		float colPos = (float)0.25 + (float)50 / 100 * index;
		return colPos;
	}
}




/****计算路径*****/
// 计算路径
/*
void getPath(vector<vector<int>>& stations, vector<WorkStation>& workStationVec, vector<tuple<int, int>>& path, vector<int>& taskStations) {
	fprintf(stderr, "[DEBUG] Start Calculation Path\n");
	calSecondLevelWorkStationCost(stations, workStationVec);
	int task7ID = calThirdLevelWorkStationCostAndGetLowTask7(stations, workStationVec);
	getTotalPath(task7ID, workStationVec, path, taskStations);
}
*/
// 计算路径
void getPath(vector<vector<int>>& stations, vector<WorkStation>& workStationVec, vector<tuple<int, int>>& path, vector<int>& taskStations) {
	fprintf(stderr, "[DEBUG] Start Calculation Path\n");
	// 需要根据是否存在7工作台来判断最终目标是构建第二层的任务还是第三层的任务
	calSecondLevelWorkStationCost(stations, workStationVec);
	if (existFixedLevelStation(stations, 3)) {
		// 如果存在第三层的工作台，则目标是7
		int task7ID = calThirdLevelWorkStationCostAndGetLowTask7(stations, workStationVec);
		getTotalPath(task7ID, workStationVec, path, taskStations);
	}
	else {
		// 默认存在第二层工作台，计算第二层工作台
		vector<int> busyStations;
		vector<int> secondLevelTask;
		// 对4、5、6计算它的出售位置_初始化时需要
		getSecondLevelStationSellStation(workStationVec, stations, stations[9 - 1]);
		calTasksWithOutStation7(stations, busyStations, workStationVec, secondLevelTask);// 获取调度任务
		getTotalPathWithout7(secondLevelTask, workStationVec, taskStations);// 获取路径
	}
}


// 计算第二层工作台的开销:4 5 6
void calSecondLevelWorkStationCost(vector<vector<int>>& stations, vector<WorkStation>& workStationVec) {
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
int calThirdLevelWorkStationCostAndGetLowTask7(vector<vector<int>>& stations, vector<WorkStation>& workStationVec) {
	// 参数定义
	double lowestTotalCost = DBL_MAX;
	int lowestTotalCostTask7ID = -1;
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
void getTotalPath(int task7ID, vector<WorkStation>& workStationVec, vector<tuple<int, int>>& path, vector<int>& taskStations) {
	// 首先找它的下一层级任务，接着对下一层再找下一层
	WorkStation& ws7 = workStationVec[task7ID];
	for (int l2TaskIDP = 0; l2TaskIDP < ws7.preWorkStationID.size(); l2TaskIDP++) {
		// 找到下游任务
		WorkStation& l2Task = workStationVec[ws7.preWorkStationID[l2TaskIDP]];
		// 找到它的更加下游的任务
		WorkStation& l1Task1 = workStationVec[l2Task.preWorkStationID[0]];
		WorkStation& l1Task2 = workStationVec[l2Task.preWorkStationID[1]];
		taskStations.push_back(l2Task.id);
		taskStations.push_back(l1Task1.id);
		taskStations.push_back(l1Task2.id);
		// 将其添加到 level2Stations、 Path中
		path.push_back(make_tuple(l2Task.id, task7ID));
		path.push_back(make_tuple(l1Task1.id, l2Task.id));
		path.push_back(make_tuple(l1Task2.id, l2Task.id));
		fprintf(stderr, "[DEBUG] Add Pretreatment Level2 Task %s\n", l2Task.toString().c_str());
		fprintf(stderr, "[DEBUG] Add Pretreatment Level1_1 Task %s\n", l1Task1.toString().c_str());
		fprintf(stderr, "[DEBUG] Add Pretreatment Level1_2 Task %s\n", l1Task2.toString().c_str());
	}
	// 将到sellWorkStation的内容也加入
	taskStations.push_back(ws7.id);
	path.push_back(make_tuple(task7ID, ws7.sellWorkStationID));
	fprintf(stderr, "[DEBUG] Add Pretreatment Level3 Task %s\n", ws7.toString().c_str());
}


// 获取没有工作台7的第二层工作台的工作任务
void getTotalPathWithout7(vector<int>& secondLevelTask, vector<WorkStation>& workStationVec, vector<int>& taskStations) {
	// 对于所有第二层任务进行处理

	for (int l2TaskIDP = 0; l2TaskIDP < secondLevelTask.size(); l2TaskIDP++) {
		// 找到下游任务
		WorkStation& l2Task = workStationVec[secondLevelTask[l2TaskIDP]];
		WorkStation& l2SellStation = workStationVec[l2Task.sellWorkStationID];
		// 找到它的更加下游的任务
		WorkStation& l1Task1 = workStationVec[l2Task.preWorkStationID[0]];
		// 将到SellStation的内容也加入
		WorkStation& l1Task2 = workStationVec[l2Task.preWorkStationID[1]];
		// 将其添加到 taskStations 中
		taskStations.push_back(l2Task.id);
		taskStations.push_back(l2Task.sellWorkStationID);
		taskStations.push_back(l1Task1.id);
		taskStations.push_back(l2Task.sellWorkStationID);
		fprintf(stderr, "[DEBUG] Add Pretreatment Level2 Task %s\n", l2Task.toString().c_str());
		fprintf(stderr, "[DEBUG] Add Pretreatment SellStation Task %s\n", l2SellStation.toString().c_str());
		fprintf(stderr, "[DEBUG] Add Pretreatment Level1_1 Task %s\n", l1Task1.toString().c_str());
		fprintf(stderr, "[DEBUG] Add Pretreatment Level1_2 Task %s\n", l1Task2.toString().c_str());
	}
}


// 计算当前节点和下级节点列表中最近的点
void getClosestLowerLevelStation(WorkStation& ws, vector<int>& llSt, vector<WorkStation>& workStationVec) {
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
void getCloestSellStation(WorkStation& ws, vector<int>& sellWordStation8, vector<int>& sellWordStation9, vector<WorkStation>& workStationVec) {
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

// 找到某第1or2层节点和一个9节点的最近距离
void getCloestSellStation(WorkStation& ws, vector<int>& sellWordStation9, vector<WorkStation>& workStationVec) {
	float curDis = 0;
	float minDisWithSellStation = FLT_MAX;
	int closestSellWorkStationID = -1;
	for (int i = 0; i < sellWordStation9.size(); i++) {
		// 找当前节点和所有9之间的最小值
		curDis = calDis(ws.position, workStationVec[sellWordStation9[i]].position);
		if (curDis < minDisWithSellStation) {
			minDisWithSellStation = curDis;
			closestSellWorkStationID = workStationVec[sellWordStation9[i]].id;
		}
	}
	// 将这个找到的最近的出售点设置为当前l2节点的出售节点
	ws.sellWorkStationID = closestSellWorkStationID;
	ws.sellCost = minDisWithSellStation;
}

/****读取每一帧的内容*****/
bool getFrameInput(int& frameId, int& price) {
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


// 获取所有第二层节点到出售它们的节点的距离——初始化时，只做一次
void getSecondLevelStationSellStation(vector<WorkStation>& workStationVec, vector<vector<int>>& stations, vector<int>& sellWordStation9) {
	for (int i = 4; i <= 6; i++) {
		for (int j = 0; j < stations[i - 1].size(); j++) {
			getCloestSellStation(workStationVec[stations[i - 1][j]], sellWordStation9, workStationVec);
		}
	}
}

// 获取所有第一层节点到出售它们的节点的距离——初始化时，只做一次
void getFirstLevelStationSellStation(vector<WorkStation>& workStationVec, vector<vector<int>>& stations, vector<int>& sellWordStation8) {
	for (int i = 1; i <= 3; i++) {
		for (int j = 0; j < stations[i - 1].size(); i++) {
			getCloestSellStation(workStationVec[stations[i][j]], sellWordStation8, workStationVec);
		}
	}
}


//重写排序方法
bool comp(const WorkStation& a, const WorkStation& b) {
	//常引用const T &xxx
	return  (a.cost + a.sellCost) < (b.cost + b.sellCost);
	// <代表升序，>代表降序
}


// 如果在没有7的情况下，则调用以下方式来获取第二层的任务，对于第二层的任务，我们选择最好实现的前3个作为需要调度的内容
bool calTasksWithOutStation7(vector<vector<int>>& stations, vector<int>& busyStation, vector<WorkStation>& workStationVec, vector<int>& secondLevelTask) {
	// 默认存在第二层的任务

	// 从当前可能的第二层任务中进行查找，从4、5、6中找
	// 用一个List存排名前三的Station，然后按照cost进行排序后作为输出结果,tuple的组成（id，cost），后面按照cost做一次排序
	vector<WorkStation> secondLevelTaskStations;
	for (int i = 4; i <= 6; i++) {
		// 分别对4、5、6每一类进行处理
		for (int j = 0; j < stations[i - 1].size(); j++) {
			// 从里面开始找
			if (!busyStation.empty() && count(busyStation.begin(), busyStation.end(), workStationVec[stations[i - 1][j]].id)) {
				// 如果有任务正在忙，则不考虑它
				continue;
			}
			// 进行加工处理
			secondLevelTaskStations.push_back(workStationVec[stations[i - 1][j]]);
		}
	}
	// 对最终结果进行排序输出
	sort(secondLevelTaskStations.begin(), secondLevelTaskStations.end(), comp);
	// 选取前三名作为输出的任务

	for (int i = 0; i < 3; i++)
	{
		secondLevelTask.push_back(secondLevelTaskStations[i].id);
	}

	return true;
}

/*
// 好像没有用到
// 在初始化的时候进行判断，到底要计算哪一些的sellcost
void findWhichLevelStationExist(vector<vector<int>>& stations) {
	// 首先看是否有7，如果有的话则全局的目标就是构造7
	if (existFixedLevelStation(stations, 3)) {
		// 对7计算它的出售位置
	}
	// 如果没有7但是有第2层节点，则全局的目标是构造第二层数据
	else if(existFixedLevelStation(stations, 2))
	{
		// 对4、5、6计算它的出售位置
		getSecondLevelStationSellStation(workStation)

	}
	else {
		// 没有第2层节点，全局的目标是第一层数据
		getFirstLevelStationSellStation(workStation);
	}


}

*/


bool existFixedLevelStation(vector<vector<int>>& stations, int level) {
	int start, end;
	switch (level)
	{
	case 1:
		start = 1;
		end = 3;
		break;
	case 2:
		start = 4;
		end = 6;
		break;
	case 3:
		start = 7;
		end = 7;
		break;
	default:
		return false;
		break;
	}
	// 找它的几层里是否有需要的数据
	for (int i = start; i <= end; i++) {
		if (stations[i - 1].size() > 0) {
			return true;
		}
	}
	return false;
}
