#pragma once

#include "RtMidi.h"
#include "../ofxBaseMidi.h"

class ofxRtMidiIn : public ofxBaseMidiIn {

public:

	ofxRtMidiIn(const string name);
	virtual ~ofxRtMidiIn();
	
	void listPorts();
	vector<string>& getPortList();
	int getNumPorts();
	string getPortName(unsigned int portNumber);


	bool openPort(unsigned int portNumber);
	bool openPort(string deviceName);
	bool openVirtualPort(string portName);
	void closePort();
	
	void ignoreTypes(bool midiSysex, bool midiTiming, bool midiSense);

private:
	
	RtMidiIn midiin;
	
	/// static callback for rtmidi
	static void _midiMessageCallback(double deltatime, vector<unsigned char> *message, void *userData);
};
