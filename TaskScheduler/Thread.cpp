#include "Thread.h"
#include <stdexcept>
#include <iostream>
#include <chrono>

void Thread::run()
{

	ostringstream os;
	os << "Thread::run(), thread id:" << this_thread::get_id() << endl;
	cout << os.str();
	
	preamble();
	for (;;)
	{
		const auto start(chrono::steady_clock::now());
		if (processing())
		{
			break;
		}

		if (mIsInterruped)
		{
			break;
		}
		const auto end(chrono::steady_clock::now());

		// subject to discuss:
		// if processing in thread too heavy we can faced with processor high load
		// too avoid this, let processor to rest
		if (mWorkQuantum > 0)
		{
			const auto interval((end - start) / 1ms);
			auto restTime(mWorkQuantum - interval);
			if (restTime < 0)
			{
				restTime = mOverloadRestQuantum;
			}
			else
			{
				restTime /= 2;
			}
			this_thread::sleep_for(chrono::milliseconds(restTime));
		}
	}
}

void Thread::start() noexcept(false)
{
	if (nullptr != mThread)
	{
		throw logic_error(THREAD_MULTIPLE_START_ERROR);
	}

	mThread = make_unique<thread>([this]()
		{
			run();
		});
}

void Thread::join() noexcept(false)
{
	if (nullptr == mThread)
	{
		throw logic_error(THREAD_NOT_RUN_ERROR);
	}
	mThread->join();
}