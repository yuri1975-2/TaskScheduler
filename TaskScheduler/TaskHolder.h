#pragma once

#include <vector>
#include <memory>
#include <list>
#include <queue>
#include <string>
#include <mutex>
#include <condition_variable>
#include "Thread.h"
#include "TaskProcessor.h"
#include "Request.h"

class TaskHolder : public Thread, public TaskProcessor::ITaskRetriever
{
private:
	inline const static char* NULL_POINTER_ARGUMENT = "argument shouldn't be null";
	inline const static char* ERROR_DURING_THREAD_RUN = "Error during run thread. Possibly too many threads in system. Trying again";

	const static int NUMBER_OF_TASK_PROCESSORS = 2;
	const static int NUMBER_OF_TASK_PROCESSORS_RUN_ATTEMPTS = 10;
	const static int TASK_PROCESSORS_RUN_ATTEMPTS_PERIOD = 500;
	const static int TASK_WAIT_TIME = 1000;

	Request* (*mGetRequest)() = nullptr;
	vector<shared_ptr<TaskProcessor>> mTaskProcessors;
	
	queue<unique_ptr<Request>, list<unique_ptr<Request>>> mRequests;
	mutex mRequestsMutex;
	condition_variable mRequestCV;

protected:
	bool processing() override;
	void preamble() override;

public:
	TaskHolder(Request* (*getRequest)(), void (*processRequest)(Request*),
		const int workQuantum, const int overloadRestQuantum) noexcept(false);

	virtual unique_ptr<Request> getTask() override;
};