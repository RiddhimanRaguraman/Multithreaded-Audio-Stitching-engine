//--------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//--------------------------------------------------------------

#ifndef FILE_COORD_SHARE_H
#define FILE_COORD_SHARE_H

struct FileCoordShare
{
	enum class Status
	{
		Empty,
		Ready,
		Transfer,
		Uninitialize
	};

	FileCoordShare();
	FileCoordShare(const FileCoordShare &) = delete;
	FileCoordShare &operator = (const FileCoordShare &) = delete;
	~FileCoordShare() = default;

	FileCoordShare(FileCoordShare &&) = delete;
	FileCoordShare &operator=(FileCoordShare &&) = delete;

	std::mutex				 mtx;
	std::condition_variable  cv;

	Status status;
	char pad[4];
};

#endif

// --- End of File ---
