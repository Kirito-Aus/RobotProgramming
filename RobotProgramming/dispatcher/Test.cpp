#include "Task.h"
#include <iostream>
#include <queue>
using namespace std;

priority_queue<Task> taskList;

void createTaskTest() {
	taskList.push(Task(Position(1.0, 2.0), Position(1.1, 2.1), 0));
	taskList.push(Task(Position(1.0, 2.0), Position(1.1, 2.1), 0));
}

int main() {
	//cout << "test" << endl;
	createTaskTest();
	cout << taskList.top().seq << endl;
	taskList.pop();
	cout << taskList.top().seq << endl;
	return 0;
}