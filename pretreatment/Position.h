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
// ����ID�����Robot�б�
extern vector<Robot> robotVec;
// ����ID�����WorkStation�б�
extern vector<WorkStation> workStationVec;
// �������ͷ��б��WorkStation�б�
extern vector<WorkStation> stationList1, stationList2, stationList3, stationList4, stationList5, stationList6, stationList7, stationList8, stationList9;
extern vector<vector<WorkStation>> stations = { stationList1, stationList2, stationList3, stationList4, stationList5, stationList6, stationList7, stationList8, stationList9 };

*/
