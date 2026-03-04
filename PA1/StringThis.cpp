//----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "StringThis.h"

namespace Azul
{
	StringThis::StringThis(Buffer::Name status)
	{
		switch(status)
		{
			case Buffer::Name::File:
				strcpy_s(this->buffer, BUFFER_SIZE, STRING_ME(Buffer::File));
				break;

			case Buffer::Name::CoordA:
				strcpy_s(this->buffer, BUFFER_SIZE, STRING_ME(Buffer::CoordA));
				break;

			case Buffer::Name::CoordB:
				strcpy_s(this->buffer, BUFFER_SIZE, STRING_ME(Buffer::CoordB));
				break;

			case Buffer::Name::Uninitialized:
				strcpy_s(this->buffer, BUFFER_SIZE, STRING_ME(Buffer::Uninitialized));
				break;

			default:
				assert(false);
		}
	}

	StringThis::operator char *()
	{
		return this->buffer;
	}
}

// --- End of File ---
