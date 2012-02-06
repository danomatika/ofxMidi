///
/// references:
///		http://www.srm.com/qtma/davidsmidispec.html
///
#pragma once

#include "ofConstants.h"

#ifdef TARGET_LINUX
	#define __LINUX_ALSASEQ__
#elif defined(TARGET_WIN32)
	#define __WINDOWS_MM__
#elif defined(TARGET_MACOSX)
	#define __MACOSX_CORE__
#endif

// defines for the MIDI status bytes

// channel voice messages
#define MIDI_NOTE_OFF			0x80
#define MIDI_NOTE_ON			0x90
#define MIDI_CONTROL_CHANGE		0xB0
#define MIDI_PROGRAM_CHANGE		0xC0
#define MIDI_PITCH_BEND			0xE0
#define MIDI_AFTERTOUCH			0xD0	// aka channel pressure
#define MIDI_POLY_AFTERTOUCH	0xA0	// aka key pressure

// system messages
#define MIDI_SYSEX				0xF0
#define MIDI_TIME_CODE			0xF1
#define MIDI_SONG_POS_POINTER	0xF2
#define MIDI_SONG_SELECT		0xF3
#define MIDI_TUNE_REQUEST		0xF6
#define MIDI_SYSEX_END			0xF7
#define MIDI_TIME_CLOCK			0xF8
#define MIDI_START				0xFA
#define MIDI_CONTINUE			0xFB
#define MIDI_STOP				0xFC
#define MIDI_ACTIVE_SENSING		0xFE
#define MIDI_SYSTEM_RESET		0xFF

// number range defines
#define MIDI_MAX_BEND			16383
