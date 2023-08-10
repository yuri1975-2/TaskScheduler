#pragma once
#include <sstream>

class Request
{
private:
	int mID;

public:
	Request(const int ID) noexcept : mID(ID)
	{
		ostringstream os;
		os << "Request c'tor, ID: " << ID << endl;
		cout << os.str();
	}

	~Request()
	{
		ostringstream os;
		os << "Request d'tor, ID: " << mID << endl;
		cout << os.str();
	}

	auto getID() const noexcept
	{
		return mID;
	}
};
