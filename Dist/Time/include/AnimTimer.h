//--------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//--------------------------------------------------------------

#ifndef ANIM_TIMER_H
#define ANIM_TIMER_H

#ifdef TIME_USE_DLL
#ifdef TIME_LIBRARY_EXPORTS
#define TIME_LIBRARY_API __declspec(dllexport)
#else
#define TIME_LIBRARY_API __declspec(dllimport)
#endif
#else
#define TIME_LIBRARY_API
#endif

/*
 * class AnimTimer
 * Measures real durations of time in terms of Time.
 *
 * The AnimTimer interface you see here owes itself to Matlab.
 * You only measure segements of time with a AnimTimer.  Use tic() to
 * mark the beginning of the segment.  Use toc() to mark the end.
 *
 * toc() always returns the amount of time that has elapsed since tic()
 * was called.  If you never call tic() on your AnimTimer then toc() will
 * return @link Time::Time() zero @endlink.
 *
 *
 * Example
 *
 *    #include "AnimTimer.h"
 *
 *    // Create a AnimTimer.
 *    AnimTimer AnimTimer;
 *
 *    // Mark our begin time.
 *    AnimTimer.tic();
 *
 *    // Do something for a while.
 *    ...
 *
 *    // Measure how much time has gone by since we called tic().
 *    const Time elapsedTime = AnimTimer.toc();
 *
 *
 */

 //---------------------------------------------------------------------------
 // HEADER FILES:
 //---------------------------------------------------------------------------

#include "AnimTime.h"

//---------------------------------------------------------------------------
// TEMPLATE DECLARATION:
//---------------------------------------------------------------------------

namespace Azul
{
	class AnimTimer
	{
	public:
		// Constants for the library
		static const uint32_t MAJOR_VERSION = 4;
		static const uint32_t MINOR_VERSION = 0;

		//-----------------------------------------------------------------
		// Constructors / Destructors
		//-----------------------------------------------------------------

		/* Creates a AnimTimer for the platform you are compiling on. */
		TIME_LIBRARY_API AnimTimer();
		TIME_LIBRARY_API AnimTimer(const AnimTimer &) = delete;
		TIME_LIBRARY_API AnimTimer &operator = (const AnimTimer &) = delete;
		TIME_LIBRARY_API ~AnimTimer();

		//-----------------------------------------------------------------
		// Timing Methods
		//-----------------------------------------------------------------

		/*
		* Marks the moment in time that toc() will get measured against.
		* Call tic() to mark the beginning of the segment of time that
		* you want to measure.
		*/
		TIME_LIBRARY_API void Tic();

		/*
		* return: The duration of Time that has elapsed since tic() was
		* called.  Returns Time::Time() zero if tic() has
		* never been called on this AnimTimer.
		*/
		TIME_LIBRARY_API const AnimTime Toc() const;


	private:
		// static method
		TIME_LIBRARY_API static const AnimTime privGetSystemTime();

		// Data
		AnimTime privTicMark;
	};
}

#endif   // AnimTimer_h

// ---  End of File ---
