/*
 * Copyright (c) 2013 Dan Wilcox <danomatika@gmail.com>
 *
 * BSD Simplified License.
 * For information on usage and redistribution, and for a DISCLAIMER OF ALL
 * WARRANTIES, see the file, "LICENSE.txt," in this distribution.
 *
 * See https://github.com/danomatika/ofxMidi for documentation
 *
 */
#pragma once

#ifndef TARGET_OF_IPHONE
    #include "RtMidi.h"
#endif

// MIDI backend APIs

enum ofxMidiApi {

#ifdef TARGET_OF_IPHONE
    OFXMIDI_UNSPECIFIED,
    OFXMIDI_PGMIDI,
#else
    OFXMIDI_UNSPECIFIED     = RtMidi::UNSPECIFIED,
    OFXMIDI_MACOSX_CORE     = RtMidi::MACOSX_CORE,
    OFXMIDI_LINUX_ALSA      = RtMidi::LINUX_ALSA,
    OFXMIDI_UNIX_JACK       = RtMidi::UNIX_JACK,
    OFXMIDI_WINDOWS_MM      = RtMidi::WINDOWS_MM,
    OFXMIDI_DUMMY           = RtMidi::RTMIDI_DUMMY
#endif
};

/// \section  stream interface midi objects
/// ref: http://www.gweep.net/~prefect/eng/reference/protocol/midispec.html

/// send a note on event (also set velocity = 0 for noteoff)
struct NoteOn {

	const int channel;  ///< channel 1 - 16
	const int pitch;    ///< pitch 0 - 127
	const int velocity; ///< velocity 0 - 127
	
	explicit NoteOn(const int channel, const int pitch, const int velocity=64) :
		channel(channel), pitch(pitch), velocity(velocity) {}
};

/// send a note off event (velocity is usually ignored)
struct NoteOff {

	const int channel;  ///< channel 1 - 16
	const int pitch;    ///< pitch 0 - 127
	const int velocity; ///< velocity 0 - 127
	
	explicit NoteOff(const int channel, const int pitch, const int velocity=64) :
		channel(channel), pitch(pitch), velocity(velocity) {}
};

/// change a control value aka send a CC message
struct ControlChange {

	const int channel;  ///< channel 1 - 16
	const int control;  ///< control 0 - 127
	const int value;    ///< value 0 - 127
	
	explicit ControlChange(const int channel, const int control, const int value) :
		channel(channel), control(control), value(value) {}
};

/// change a program value (ie an instrument)
struct ProgramChange {

	const int channel;  ///< channel 1 - 16
	const int value;    ///< value 0 - 127
	
	explicit ProgramChange(const int channel, const int value) :
		channel(channel), value(value) {}
};

/// change the pitch bend value
struct PitchBend {

	const int channel;  ///< channel 1 - 16
	const int value;    ///< value 0 - 16383
	
	explicit PitchBend(const int channel, const int value) :
		channel(channel), value(value) {}
};

/// change an aftertouch value
struct Aftertouch {

	const int channel;  ///< channel 1 - 16
	const int value;    ///< value 0 - 127
	
	explicit Aftertouch(const int channel, const int value) :
		channel(channel), value(value) {}
};

/// change a poly aftertouch value
struct PolyAftertouch {

	const int channel;  ///< channel 1 - 16
	const int pitch;    ///< controller 0 - 127
	const int value;    ///< value 0 - 127
	
	explicit PolyAftertouch(const int channel, const int pitch, const int value) :
		channel(channel), pitch(pitch), value(value) {}
};

/// start a raw midi byte stream
struct StartMidi {
	explicit StartMidi() {}
};

/// finish a midi byte stream
struct FinishMidi {
	explicit FinishMidi() {}
};
