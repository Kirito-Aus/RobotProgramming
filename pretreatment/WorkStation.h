#pragma once
#include <vector>
#include"Position.h"

using namespace std;


class WorkStation
{
public:
	Position pos;
	// 前一层的任务（从哪个id的工作台来）
	vector<int> preWorkStationID;
	// 完成生产该任务的开销（上一层工作台的总开销+上一层工作台到该工作台的总开销）
	double cost;
	int id;
	int type;
	int sellWorkStationID;
	WorkStation() {

	}
};