/*
 * Copyright (c) 2018 Dan Wilcox <danomatika@gmail.com>
 *
 * BSD Simplified License.
 * For information on usage and redistribution, and for a DISCLAIMER OF ALL
 * WARRANTIES, see the file, "LICENSE.txt," in this distribution.
 *
 * See https://github.com/danomatika/ofxMidi for documentation
 *
 * Adapted from code written in Swift for ZKM | Karlsruhe
 *
 */
#pragma once

#include <vector>
#include <chrono>
#include "ofxMidiConstants.h"

using namespace std::chrono;

///
/// MIDI clock message parser
///
/// currently input only, does not send clock ticks
///
class ofxMidiClock {

public:

	ofxMidiClock() {}
	virtual ~ofxMidiClock() {}

	/// update clock from a raw MIDI message,
	/// returns true if the message was handled
	bool update(std::vector<unsigned char> &message);

	/// measure length since last tick
	void tick();

	/// reset timestamp
	void reset();

/// \section Status

	/// get the song position in beats
	double getBeats();

	/// set the song position in beats
	void setBeats(double b);

	/// get the song position in seconds
	double getSeconds();

	/// set the song position in seconds
	void setSeconds(double s);

	/// get tempo in beats per minute calculated from clock tick length
	double getBpm();

	/// set clock tick length from tempo in beats per minute
	void setBpm(double bpm);

/// \section Util

	/// get the song position in seconds from a beat position,
    /// 1 beat = 1/16 note = 6 clock ticks
	double beatsToSeconds(double beats);

	/// get the song position in beats from seconds,
	/// 1 beat = 1/16 note = 6 clock ticks
	double secondsToBeats(double seconds);

	/// get the song position in seconds from a raw MIDI song pos message
	double songPosToSeconds(std::vector<unsigned char> &songPos);

	/// calculate MIDI clock length in ms from a give tempo bpm
	static double bpmToMs(double bpm);

	/// calculate tempo bpm from a give MIDI clock length in ms
	static double msToBpm(double ms);

protected:

	double length = 20.833; //< averaged tick length in ms, default 120 bpm
	double beats = 0.0; //< current song pos in beats (6 ticks = 1 beat)
	steady_clock::time_point timestamp; //< last timestamp
};
