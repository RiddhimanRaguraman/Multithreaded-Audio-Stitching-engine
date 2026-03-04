//--------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//--------------------------------------------------------------

#ifndef BUFFER_H
#define BUFFER_H

struct Buffer
{
public:
	enum class Name
	{
		File,
		CoordA,
		CoordB,
		Uninitialized
	};

public:
	Buffer(size_t size, Buffer::Name name);

	Buffer() = delete;
	Buffer(const Buffer &) = delete;
	Buffer &operator = (const Buffer &) = delete;
	~Buffer();

	unsigned char *GetRawBuff();

	// why size_t?
	size_t GetTotalSize() const;
	size_t GetCurrSize() const;
	void   SetCurrSize(const size_t s);
	std::mutex &GetMutuex();

private:
	unsigned char *poBuff;
	std::mutex     mtx;

	Name		   mName;
	char pad[4];
	size_t         totalSize;
	size_t         currSize;
};

#endif

// --- End of File ---

