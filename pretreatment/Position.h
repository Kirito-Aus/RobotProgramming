#pragma once
class Position {
public:
	double x;
	double y;
	Position(double x, double y) {
		this->x = x;
		this->y = y;
	}
	Position() {

	}
};

/*
// 根据ID排序的Robot列表
extern vector<Robot> robotVec;
// 根据ID排序的WorkStation列表
extern vector<WorkStation> workStationVec;
// 按照类型分列表的WorkStation列表
extern vector<WorkStation> stationList1, stationList2, stationList3, stationList4, stationList5, stationList6, stationList7, stationList8, stationList9;
extern vector<vector<WorkStation>> stations = { stationList1, stationList2, stationList3, stationList4, stationList5, stationList6, stationList7, stationList8, stationList9 };

*/
