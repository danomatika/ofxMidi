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
#include "ofxMidiClock.h"

#include "ofLog.h"

// -----------------------------------------------------------------------------
bool ofxMidiClock::update(std::vector<unsigned char> &message) {
	switch(message[0]) {
		case MIDI_TIME_CLOCK:
			tick();
			beats += 0.166667; // 1 tick = 1/6 beat
			return true;
		case MIDI_SONG_POS_POINTER:
			if(message.size() < 3) {return false;}
			beats = (message[2] << 7) + message[1];
			ofLogVerbose("ofxMidiClock") << "Song Pos " << beats;
			return true;
	}
	return false;
}

// -----------------------------------------------------------------------------
void ofxMidiClock::tick() {
	double us = duration_cast<microseconds>(steady_clock::now()-timestamp).count();
	if(us < 200000) { // filter obviously bad values
		length += (us/1000 - length) / 5; // average last 5 ticks
	}
	timestamp = steady_clock::now();
}

// -----------------------------------------------------------------------------
void ofxMidiClock::reset() {
	timestamp = steady_clock::now();
}

/// Status

// -----------------------------------------------------------------------------
double ofxMidiClock::getBeats() {
	return beats;
}

// -----------------------------------------------------------------------------
void ofxMidiClock::setBeats(double b) {
	beats = b;
}

// -----------------------------------------------------------------------------
double ofxMidiClock::getSeconds() {
	return beatsToSeconds(beats);
}

// -----------------------------------------------------------------------------
void ofxMidiClock::setSeconds(double s) {
	beats = secondsToBeats(s);
}

// -----------------------------------------------------------------------------
double ofxMidiClock::getBpm() {
	return msToBpm(length);
}

// -----------------------------------------------------------------------------
void ofxMidiClock::setBpm(double bpm) {
	length = bpmToMs(bpm);
}

// Util

// -----------------------------------------------------------------------------
double ofxMidiClock::beatsToSeconds(double beats) {
	return (beats * 6 * length) / 1000.0;
}

// -----------------------------------------------------------------------------
double ofxMidiClock::secondsToBeats(double seconds) {
	return (seconds * 1000.0) / (6 * length);
}

// -----------------------------------------------------------------------------
double ofxMidiClock::songPosToSeconds(std::vector<unsigned char> &songPos) {
	if(songPos.size() < 3) {return 0;}
	unsigned int beats = (songPos[2] << 7) + songPos[1];
	return beatsToSeconds(beats);
}

// -----------------------------------------------------------------------------
double ofxMidiClock::bpmToMs(double bpm) {
	if(bpm == 0) {return 0;}
	return 1000.0 / ((bpm * 24.0) / 60.0);
}

// -----------------------------------------------------------------------------
double ofxMidiClock::msToBpm(double ms) {
	if(ms == 0) {return 0;}
	return (1000.0 / ms / 24.0) * 60.0;
}
