#pragma once

#include "ofConstants.h"

// for rtmidi
#ifdef TARGET_MACOSX
	#define __MACOSX_CORE__
#elif defined TARGET_WIN32
	#define __WINDOWS_KS__
	#ifdef __WINDOWS_MM__
		#undef __WINDOWS_MM__
	#endif
#else
	#define __LINUX_ALSA__
#endif
