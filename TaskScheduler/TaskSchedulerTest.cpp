// TaskHolderTest.cpp: определяет точку входа для приложения.
//

#include "TaskSchedulerTest.h"

using namespace std;

const int NUMBER_OF_REQUESTS = 10;
const int WORK_QUANTUM = 1500;  // for processor rest technique
const int MINIMUM_REST_QUANTUM = 500;  // for processor rest technique

void ProcessRequest(Request* request)
{
	if (nullptr == request)
	{
		return;
	}
	
	// simulate request processing
	this_thread::sleep_for(chrono::milliseconds(500 + rand() % 1000));
	
	ostringstream os;
	os << "processing request, id: " << request->getID() << endl;
	cout << os.str();
}

Request* GetRequest()
{
	static int currentRequestID = 1;

	ostringstream os;
	os << "GetRequest()" << endl;
	cout << os.str();

	// simulate request retrieving
	this_thread::sleep_for(chrono::milliseconds(rand() % 750));

	auto retValue((currentRequestID <= NUMBER_OF_REQUESTS) ? new Request(currentRequestID++) : nullptr);
	return retValue;
}

int main()
{
	srand(static_cast<unsigned int>(time(0)));

	unique_ptr<TaskHolder> th(make_unique<TaskHolder>(GetRequest, ProcessRequest, 0, 0));	
	// with processor rest technique-less interesting balansing of incoming and processed requests but with processor rest
	//unique_ptr<TaskHolder> ts(make_unique<TaskHolder>(GetRequest, ProcessRequest, WORK_QUANTUM, MINIMUM_REST_QUANTUM));
	try
	{
		th->start();
		th->join();
	}
	catch (exception&)
	{
		cout << "Error during scheduler thread run. Possibly, too many threads in system. Try later" << endl;
	}
	return 0;
}
