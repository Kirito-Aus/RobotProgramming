#pragma once
#include<vector>
#include<iostream>
#include"Robot.h"
#include"WorkStation.h"
using namespace std;

bool readInStartData(vector<Robot>& robotVec, vector<WorkStation>& workStationVec, vector<vector<int>>& stations);
float getPosByIndex(int, int);
void getPath(vector<vector<int>>& stations, vector<WorkStation>& workStationVec, vector<tuple<int, int>>& path, vector<int>& taskStations);
bool getFrameInput(int& frameId, int& price);