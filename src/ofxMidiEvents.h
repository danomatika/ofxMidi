#pragma once

#include "ofMain.h"
#include <map>

class ofxMidiEventArgs: public ofEventArgs{
public:
	int port;
	int channel;
	int status;
	int byteOne;
	int byteTwo;
	double timestamp;
};

typedef ofEvent <ofxMidiEventArgs> ofxMidiEvent;

class ofxMidiListener {
public:
	ofxMidiListener(){}
	virtual ~ofxMidiListener(){}
	
	virtual void newMidiMessage(ofxMidiEventArgs& eventArgs)=0;
};
