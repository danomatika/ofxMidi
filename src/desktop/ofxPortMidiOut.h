#pragma once

#include "RtMidi.h"
#include "portmidi.h"
#include "../ofxBaseMidi.h"

class ofxPortMidiOut : public ofxBaseMidiOut {

public:

	/// set the output client name (optional)
	ofxPortMidiOut(const string name);
	virtual ~ofxPortMidiOut();

	static void listPorts();
	static vector<string>& getPortList();
	static int getNumPorts();
	static string getPortName(unsigned int portNumber);

	bool openPort(unsigned int portNumber);
	bool openPort(string deviceName);
	bool openVirtualPort(string portName);
	void closePort();
	
private:
	
	void sendMessage();

	PmStream *stream;
};
