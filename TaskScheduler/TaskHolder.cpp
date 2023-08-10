#include <algorithm>
#include "TaskHolder.h"
#include <sstream>

TaskHolder::TaskHolder(Request* (*getRequest)(), void (*processRequest)(Request*), 
	const int workQuantum, const int overloadRestQuantum) noexcept(false) : Thread(workQuantum, overloadRestQuantum)
{
	cout << "TaskHolder c'tor" << endl;

	if (nullptr == getRequest)
	{
		throw logic_error(NULL_POINTER_ARGUMENT);
	}

	mGetRequest = getRequest;

	mTaskProcessors.reserve(NUMBER_OF_TASK_PROCESSORS);
	for (auto i(0); i < NUMBER_OF_TASK_PROCESSORS; ++i)
	{
		mTaskProcessors.emplace_back(new TaskProcessor(processRequest, *this, workQuantum, overloadRestQuantum));
	}
}

bool TaskHolder::processing()
{
	ostringstream os;
	os << "TaskHolder::processing()" << endl;
	cout << os.str();

	auto request(mGetRequest());
	if (nullptr == request)
	{
		// end of requests, terminate processor and this threads
		for_each(mTaskProcessors.begin(), mTaskProcessors.end(), [](auto taskProcessor)
			{
				taskProcessor->interrupt();
			});
		for_each(mTaskProcessors.begin(), mTaskProcessors.end(), [](auto taskProcessor)
			{
				taskProcessor->join();
			});

		return true;
	}
	else
	{
		// we have next request-set it to requests queue
		lock_guard lg(mRequestsMutex);
		mRequests.emplace(request);
		if (1 == mRequests.size())
		{
			mRequestCV.notify_one();
		}
	}
	
	return false;
}

void TaskHolder::preamble()
{
	// run processor thread (possibly, with several attempts, if we'd faced with error of launch)
	// this paranoic additional attempts-subject to discuss
	auto isRunError(false);
	auto numberOfAttempts(0);
	for(;;)
	{
		for_each(mTaskProcessors.cbegin(), mTaskProcessors.cend(), [&](const auto taskProcessor)
			{
				try
				{
					if (false == taskProcessor->isRun())
					{
						taskProcessor->start();
					}
				}
				catch (exception&)
				{
					ostringstream os;
					os << ERROR_DURING_THREAD_RUN << endl;
					cout << os.str();
					isRunError = true;
				}
			});

		if ((isRunError) && (numberOfAttempts < NUMBER_OF_TASK_PROCESSORS_RUN_ATTEMPTS))
		{
			// not all processor thread has been run, try again after while
			this_thread::sleep_for(chrono::milliseconds(TASK_PROCESSORS_RUN_ATTEMPTS_PERIOD));
			++numberOfAttempts;
		}
		else
		{
			break;
		}
	}

	// not all processing thread has been run. Tell about it
	if (isRunError)
	{
		auto numberOfFaultyThreads(0);
		for_each(mTaskProcessors.cbegin(), mTaskProcessors.cend(), [&](const auto taskProcessor)
			{
				if (false == taskProcessor->isRun())
				{
					++numberOfFaultyThreads;
				}
			});

		ostringstream os;
		os << numberOfFaultyThreads << " from " << NUMBER_OF_TASK_PROCESSORS << " not run. Possibly too many threads in system" << endl;
		cout << os.str();
	}
}

unique_ptr<Request> TaskHolder::getTask()
{
	lock_guard lg(mRetrieveMutex); // ensure only one processor thread accessing to task queue...

	unique_lock ul(mRequestsMutex);  // synchronize access to task queue by processor and scheduler thread
	if (mRequests.empty())
	{
		mRequestCV.wait_for(ul, chrono::milliseconds(TASK_WAIT_TIME)); // avoid infinite waiting and such uninteruptability of thread
	}
	
	if (false == mRequests.empty())
	{
		auto retValue(move(mRequests.front()));
		mRequests.pop();
		return retValue;
	}
	else
	{
		return nullptr;
	}
}

