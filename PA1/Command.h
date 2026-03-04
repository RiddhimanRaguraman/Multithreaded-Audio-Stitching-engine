//----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef COMMAND_H
#define COMMAND_H

// Commands 
class Command
{
public:
	// Contract for the command
	virtual void Execute() = 0;
};

#endif

// --- End of File ---

