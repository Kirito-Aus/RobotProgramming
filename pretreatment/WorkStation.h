#pragma once
#include <vector>
#include"Position.h"

using namespace std;


class WorkStation
{
public:
	Position pos;
	// ǰһ������񣨴��ĸ�id�Ĺ���̨����
	vector<int> preWorkStationID;
	// �������������Ŀ�������һ�㹤��̨���ܿ���+��һ�㹤��̨���ù���̨���ܿ�����
	double cost;
	int id;
	int type;
	int sellWorkStationID;
	WorkStation() {

	}
};