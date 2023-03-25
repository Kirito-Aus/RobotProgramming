#pragma once
#include <string>
#include <sstream>
#include<vector>
#include "Position.h"
#include"CoordinatePos.h"
using namespace std;

class WorkStation {
private:
	// get the tier of workstation on production tree
	static int getTier(int type)
	{
		if (type >= 8) return 0;
		if (type == 7) return 1;
		return 2;
	}

public:
	static WorkStation nullWorkStation;

	// status
	int id;
	int type;
	Position position;
	CoordinatePos cPos;
	int remainingProduceTime;
	int materialStatus;
	int producionStatus;

	int priority;
	// the priority of task which this workstation request from others
	int requestTaskPriority;
	// wheather the workStation's production is registered
	bool resourcePushed = false;

	/*Pretreatment����*/
	// ǰһ������񣨴��ĸ�id�Ĺ���̨����
	vector<int> preWorkStationID;
	// �������������Ŀ�������һ�㹤��̨���ܿ���+��һ�㹤��̨���ù���̨���ܿ�����
	double cost;
	int sellWorkStationID;

	WorkStation():id(0), type(0), priority(1000) {}

	void updateRequestPriority() {
		requestTaskPriority = priority * 3 + getTier(type);
	}

	std::string toString() {
		std::stringstream buffer;
		buffer << "WorkStation{id=" << id << ", type=" << type << ", priority=" << priority	<< "}";
		return buffer.str();
	}

	std::string pretreatToString() {
		std::stringstream buffer;
		buffer << "WorkStation{id=" << id << ", type=" << type << ", CPos=" << cPos.toString()<<",Position="<<position.toString() << "}";
		return buffer.str();
	}
};

// WorkStation WorkStation::nullWorkStation;
