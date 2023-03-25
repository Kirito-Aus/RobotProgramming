#pragma once
#include <vector>
#include <iostream>
#include "Robot.h"
using namespace std;

bool readInStartData(vector<Robot>& robotVec, vector<WorkStation>& workStationVec, vector<vector<int>>& stations);
float getPosByIndex(int, int);
void getPath(vector<vector<int>>& stations, vector<WorkStation>& workStationVec, vector<tuple<int, int>>& path, vector<int>& taskStations);
bool getFrameInput(int& frameId, int& price);
double calDis(Position p1, Position p2);
void getClosestLowerLevelStation(WorkStation& ws, vector<int>& llSt, vector<WorkStation>& workStationVec);
void getTotalPath(int task7ID, vector<WorkStation>& workStationVec, vector<tuple<int, int>>& path, vector<int>& taskStations);
void getCloestSellStation(WorkStation& ws, vector<int>& sellWordStation8, vector<int>& sellWordStation9, vector<WorkStation>& workStationVec);
void calSecondLevelWorkStationCost(vector<vector<int>>& stations, vector<WorkStation>& workStationVec);
int calThirdLevelWorkStationCostAndGetLowTask7(vector<vector<int>>& stations, vector<WorkStation>& workStationVec);
float getPosByIndex(int index);