#include "Task.h"

int Task::CUR_SEQ = 0;

const char* Task::strTaskStatus[4] = {
	"READY",
	"TO_START_POINT",
	"TO_END_POINT",
	"DONE"
};