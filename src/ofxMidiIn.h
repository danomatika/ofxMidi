#pragma once

#include <iostream>
#include "RtMidi.h"

#include "ofxMidiConstants.h"
#include "ofxMidiEvents.h"

#include "ofMain.h"

void ofxMidiInCallback( double deltatime, vector< unsigned char > *message, void *userData );

class ofxMidiIn {
public:
	ofxMidiIn();
	virtual ~ofxMidiIn();
	
	void listPorts();
	void openPort(unsigned int port=0);
	void closePort();
	void manageNewMessage(double deltatime, vector< unsigned char > *message);
	unsigned int getPort();
	void setVerbose(bool verbose);
	
	vector<string> portNames;
	
protected:
	RtMidiIn midii;
	unsigned int nPorts;
	unsigned int port;
	bool bVerbose;
	
	// events
public:
	void addListener(int id,ofxMidiListener * listener);
	void removeListener(int id,ofxMidiListener * listener);
	void addListener(ofxMidiListener * listener);
	void removeListener(ofxMidiListener * listener);
	
	map<int,ofEvent<ofxMidiEventArgs>*> newIdMessageEvents;
	ofEvent<ofxMidiEventArgs> newMessageEvent;
};
