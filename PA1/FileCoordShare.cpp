//--------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//--------------------------------------------------------------

#include "FileCoordShare.h"

FileCoordShare::FileCoordShare()
	: mtx(),
	cv(),
	status(Status::Empty)
{
}

// --- End of File ---
