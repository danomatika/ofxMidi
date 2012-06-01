#pragma once

#include "ofMain.h"
#include "ofxiPhone.h"
#include "ofxiPhoneExtras.h"

#include "ofxMidi.h"

#include <deque>

class testApp : public ofxiPhoneApp, public ofxMidiListener {
	
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
	
	void newMidiMessage(ofxMidiMessage& msg);
	
	void addMessage(string msg);

	ofxMidiIn midiIn;
	ofxMidiOut midiOut;

	deque<string> messages;
	int maxMessages;
	
	int note, ctl;
	vector<unsigned char> bytes;
};


