#pragma once

#include "RtMidi.h"
#include "../ofxBaseMidi.h"

class ofxRtMidiOut : public ofxBaseMidiOut {

public:

	/// set the output client name (optional)
	ofxRtMidiOut(const string name);
	virtual ~ofxRtMidiOut();

	static void listPorts();
	static vector<string>& getPortList();
	static int getNumPorts();
	static string getPortName(unsigned int portNumber);

	bool openPort(unsigned int portNumber);
	bool openPort(string deviceName);
	bool openVirtualPort(string portName);
	void closePort();
	
private:
	
	void sendMessage(unsigned int deltatime);
	void sendMessageAtTime(unsigned long long timestamp);

	RtMidiOut midiOut;
	static RtMidiOut s_midiOut; //< for static calls (a little kludgey, I know)
};
