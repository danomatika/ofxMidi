/*
 * Copyright (c) 2018 Dan Wilcox <danomatika@gmail.com>
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
#include "ofxMidiClock.h"
#include "ofxMidiTimecode.h"

/// subclasses ofxMidiListener for direct message handling
class ofApp : public ofBaseApp, public ofxMidiListener {

public:

	void setup();
	void update();
	void draw();
	void exit();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage message);

	/// ofxMidiListener callback
	void newMidiMessage(ofxMidiMessage &message);

	ofxMidiIn midiIn;
	bool verbose = false;

	// MIDI CLOCK

	ofxMidiClock clock; ///< clock message parser
	bool clockRunning = false; ///< is the clock sync running?
	unsigned int beats = 0; ///< song pos in beats
	double seconds = 0; ///< song pos in seconds, computed from beats
	double bpm = 120; ///< song tempo in bpm, computed from clock length

	// MIDI TIMECODE
	
	ofxMidiTimecode timecode; ///< timecode message parser
	bool timecodeRunning = false; ///< is the timecode sync running?
	long timecodeTimestamp = 0; ///< when last quarter frame message was received
	ofxMidiTimecodeFrame frame; ///< timecode frame data, ie. H M S frame rate
};
