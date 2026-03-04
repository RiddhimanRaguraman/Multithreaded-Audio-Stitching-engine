//-----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------- 

#ifndef CIRCULAR_DATA_H
#define CIRCULAR_DATA_H

#include "Command.h"

class CircularData
{
private:
	class CircularIndex
	{
	public:
		CircularIndex(unsigned int buffSize);

		CircularIndex() = delete;
		CircularIndex(const CircularIndex &) = delete;
		const CircularIndex &operator = (const CircularIndex &) = delete;
		~CircularIndex() = default;

		// postfix
		unsigned int operator++(int);
		bool operator==(const CircularIndex &tmp);
		bool operator!=(const CircularIndex &tmp);

		// accessor
		unsigned int Index() const;

	private:
		unsigned int index;
		unsigned int size;
		unsigned int mask;
	};

public:
	// Needs to be a power of 2
	static const int CIRCULAR_DATA_SIZE = 1 << 5;

	// verify that buffSize is a power of 2
	static_assert((CIRCULAR_DATA_SIZE &(CIRCULAR_DATA_SIZE - 1)) == 0, "not Power of 2");

public:
	CircularData();

	CircularData(const CircularData &) = delete;
	const CircularData &operator = (const CircularData &) = delete;
	~CircularData();

	bool PushBack(Command *pCmd);
	bool PopFront(Command *&pCmd);

	bool IsEmpty();

private:
	Command *data[CIRCULAR_DATA_SIZE];

	CircularIndex front;
	CircularIndex back;
	bool empty;
	bool full;
	char pad[6];
	std::mutex mtx;
};

#endif

//---  End of File ---
