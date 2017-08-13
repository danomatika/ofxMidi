/*
 * Copyright (c) 2016 Sol Bekic <s+of at s-ol dot nu>
 *
 * BSD Simplified License.
 * For information on usage and redistribution, and for a DISCLAIMER OF ALL
 * WARRANTIES, see the file, "LICENSE.txt," in this distribution.
 *
 * See https://github.com/danomatika/ofxMidi for documentation
 *
 */
#pragma once

#include "ofMain.h"
#include "ofxMidi.h"

///
/// a midi time code listener
///
/// receives MTC and MIDI start/continue/stop messages
///
class ofxMidiMTCIn : public ofxMidiListener {

public:
/// \section detected SMPTE state

	/// whether enough data has been received
	bool ready = false;
	/// whether the transport is currently playing or stopped
	bool playing = true;

	/// hours since start
	int hours = 0;
	/// minutes since last hour
	int minutes = 0;
	/// seconds since last minute
	int seconds = 0;
	/// frames since last second
	int frames = 0;
	
	/// SMPTE fps, drop-frames not supported atm
	int framesPerSecond = 30;
	
	/// the midi port to read from
	ofxMidiMTCIn(ofxMidiIn& midiIn);
	~ofxMidiMTCIn();
	
	void newMidiMessage(ofxMidiMessage& message);
	
protected:
	void updateFramesPerSecond();
	ofxMidiIn& midiIn;
};
