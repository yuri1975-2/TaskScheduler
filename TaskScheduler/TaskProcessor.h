#pragma once
#include <condition_variable>
#include <mutex>
#include <atomic>
#include "Thread.h"
#include "Request.h"

using namespace std;

class TaskProcessor : public Thread
{
public:
	class ITaskRetriever
	{
	public:

		virtual unique_ptr<Request> getTask() = 0;

		virtual ~ITaskRetriever() {};
	};

private:
	inline const static char* NULL_POINTER_ARGUMENT = "Argument shouldn't be null";
	inline const static char* THREAD_IS_BUSY = "Thread still is busy. Call isBusy() to test is thread free before setTask()";
	
	void (*mProcessRequest)(Request*) { nullptr };
	ITaskRetriever& mTaskRetriver;

protected:
	bool processing() override;

public:
	TaskProcessor(void (*processRequest)(Request*), ITaskRetriever& taskRetriver,
		const int workQuantum, const int overloadRestQuantum) noexcept(false);
};