GIVEN:

Request-class describing some request to be processed;
Request* GetRequest()-function returning request, may take a while to work;
void ProcessRequest(Request* request)-function processing request, also can consume a noticable time.


NEEDS TO BE IMPLEMENTED:

Multithreaded application with:
1) one base thread which:
	a) start >=2 processing requests threads at own start;
	b) retrieving requests by means of GetRequest() untill last one returns not nullptr;
	c) after nullptr GetRequest() thread should stop processing threads and him itself.

2) >=2 processing threads which:
	a) should process given request;
	b) should be able to be stopped by another thread (more precisely, base thread).


SOLUTION:
Request-describes request;
Thread-Java-like class incapsulating work with C++ thread;
TaskHolder-base thread derived from Thread;
TaskProcessor-processing thread derived from Thread;
TaskSchedulerTest-module to test application.