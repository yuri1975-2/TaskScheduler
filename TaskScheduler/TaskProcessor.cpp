#include "TaskProcessor.h"
#include <iostream>
#include <stdexcept>
#include <sstream>

TaskProcessor::TaskProcessor(void (*processRequest)(Request*), ITaskRetriever& taskRetriever,
	const int workQuantum, const int overloadRestQuantum) noexcept(false) : Thread(workQuantum, overloadRestQuantum),
	mTaskRetriver(taskRetriever)
{
	if (nullptr == processRequest)
	{
		throw logic_error(NULL_POINTER_ARGUMENT);
	}

	mProcessRequest = processRequest;
}


bool TaskProcessor::processing()
{
	ostringstream os;
	os << "TaskProcessor::processing()" << endl;
	cout << os.str();

	unique_ptr<Request> task(mTaskRetriver.getTask());
	if (nullptr != task)
	{
		mProcessRequest(task.get());
	}

	return false;
}