//--------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//--------------------------------------------------------------

#include "Buffer.h"

Buffer::Buffer(size_t s, Buffer::Name name)
	: poBuff(new unsigned char[s]()),
	mtx(),
	mName(name),
	totalSize(s),
	currSize(0)
{
	assert(poBuff);
}
unsigned char *Buffer::GetRawBuff()
{
	assert(poBuff);
	return this->poBuff;
}

size_t Buffer::GetTotalSize() const
{
	return this->totalSize;
}

size_t Buffer::GetCurrSize() const
{
	return this->currSize;
}

void Buffer::SetCurrSize(const size_t s)
{
	assert(s <= this->totalSize);
	this->currSize = s;
}

std::mutex &Buffer::GetMutuex()
{
	return this->mtx;
}

Buffer::~Buffer()
{
	delete poBuff;
	poBuff = nullptr;
}

// --- End of File ---
