#pragma once

#include "RtMidi.h"
#include "../ofxBaseMidi.h"

class ofxRtMidiOut : public ofxBaseMidiOut {

public:

	/// set the output client name (optional)
	ofxRtMidiOut(const string name);
	virtual ~ofxRtMidiOut();
	

	void listPorts();
	vector<string>& getPortList();
	int getNumPorts();
	string getPortName(unsigned int portNumber);
	

	bool openPort(unsigned int portNumber);
	bool openPort(string deviceName);
	bool openVirtualPort(string portName);
	void closePort();
	
private:
	
	void sendMessage();

	RtMidiOut midiout;
};
