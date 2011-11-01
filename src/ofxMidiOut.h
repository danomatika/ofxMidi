#pragma once

#include <iostream>
#include "RtMidi.h"

#include "ofxMidiConstants.h"
#include "ofxMidiEvents.h"

#include "ofMain.h"

class ofxMidiOut{
public:
	ofxMidiOut();
	virtual ~ofxMidiOut();
	
	void listPorts();
	void openPort(unsigned int port=0);
	void openPort(string _deviceName);
	void openVirtualPort(string _port);
	void closePort();
	unsigned int getPort();
	
	void sendNoteOn(int channel, int id, int value);
	void sendNoteOff(int channel, int id, int value);
	void sendControlChange(int channel, int id, int value);
	void sendProgramChange(int channel, int value);
	void sendPitchBend(int channel, int lsb, int msb);
	
	vector<string> portNames;
	
protected:
	
	void sendMessage();
	void findPorts();
	
	RtMidiOut midiout;
	unsigned int nPorts;
	unsigned int port;
	vector<unsigned char> message;
};
