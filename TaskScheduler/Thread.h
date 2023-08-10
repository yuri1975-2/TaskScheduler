#pragma once

#include <atomic>
#include <thread>
#include <memory>
#include <iostream>
#include <sstream>

using namespace std;

class Thread
{
private:
	inline const static char* THREAD_MULTIPLE_START_ERROR = "the thread could be run only once";
	inline const static char* THREAD_NOT_RUN_ERROR = "the thread still ain't run";

	unique_ptr<thread> mThread{ nullptr };
	atomic_bool mIsInterruped{ false };
	atomic_bool mIsRun{ false };

	// subject to discuss:
	// if processing in thread too heavy we can faced with processor high load
	// too avoid this, let processor to rest
	int mWorkQuantum{ 0 };
	int mOverloadRestQuantum{ 0 };

protected:
	virtual bool processing() = 0;
	virtual void preamble() {};

public:
	Thread()
	{
	}
	Thread(const int workQuantum, const int overloadRestQuantum) : mWorkQuantum(workQuantum),
		mOverloadRestQuantum(overloadRestQuantum)
	{
	}
	~Thread()
	{
		// here and later, intermediate ostringstream is used,'cause, unfortunatelly till C++ 20, cout ain't thread safe
		ostringstream os;
		os << "~Thread d'tor" << endl;
		cout << os.str();
	}

	void run();

	void start() noexcept(false);

	void interrupt() noexcept
	{
		mIsInterruped = true;
	}

	bool isRun() const noexcept
	{
		return mIsRun;
	}

	void join() noexcept(false);
};
