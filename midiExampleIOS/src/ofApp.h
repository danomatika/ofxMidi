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

#include "ofMain.h"
#include "ofxiOS.h"
#include "ofxiOSExtras.h"
#include "ofxMidi.h"

#include <deque>

/// subclasses ofxMidiListener for direct message handling
class ofApp : public ofxiOSApp, public ofxMidiListener, public ofxMidiConnectionListener {
	
public:
	void setup();
	void update();
	void draw();
	void exit();
	
	void touchDown(ofTouchEventArgs &touch);
	void touchMoved(ofTouchEventArgs &touch);
	void touchUp(ofTouchEventArgs &touch);
	void touchDoubleTap(ofTouchEventArgs &touch);
	void touchCancelled(ofTouchEventArgs &touch);

	void lostFocus();
	void gotFocus();
	void gotMemoryWarning();
	void deviceOrientationChanged(int newOrientation);
	
	// add a message to the display queue
	void addMessage(std::string msg);
	
	// midi message callback (direct message handling)
	void newMidiMessage(ofxMidiMessage &message);
	
	// midi device (dis)connection event callbacks
	void midiInputAdded(std::string name, bool isNetwork);
	void midiInputRemoved(std::string name, bool isNetwork);
	
	void midiOutputAdded(std::string nam, bool isNetwork);
	void midiOutputRemoved(std::string name, bool isNetwork);

	std::vector<ofxMidiIn *> inputs;
	std::vector<ofxMidiOut *> outputs;

	std::deque<std::string> messages;
	int maxMessages;
	ofMutex messageMutex; // make sure we don't read from queue while writing
	
	int note, ctl;
	std::vector<unsigned char> bytes;
};
