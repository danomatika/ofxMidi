/*
 * Copyright (c) 2013-2023 Dan Wilcox <danomatika@gmail.com>
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

/// MIDI input messages can be handled in one of two ways: direct or queued
///
/// direct
/// Subclass ofxMidiListener if you want to receive messages directly on the
/// MIDI thread. Messages should be manually buffered for use on the GUI thread.
/// This is faster as it is not limited by the draw framerate but you have to be
/// careful of memory access.
///
/// queued
/// Manually handle messages using hasWaitingMessages()/getNextMessage().
/// Received messages are thread safe.
///
/// queued message passing is enabled by default and is disabled when an
/// ofxMidiListener is added
///
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
	void mouseReleased();

	/// direct message handling
	/// ofxMidiListener callback, called by ofxMidiIn instance if set as listener
	/// note: this is not needed if you use queued message passing
	void newMidiMessage(ofxMidiMessage &message);
	
	ofxMidiIn midiIn;
	std::vector<ofxMidiMessage> midiMessages; ///< received messages
	std::size_t maxMessages = 10; ///< max number of messages to keep track of
};
