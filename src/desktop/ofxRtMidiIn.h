#pragma once

#include "RtMidi.h"
#include "../ofxBaseMidi.h"

class ofxRtMidiIn : public ofxBaseMidiIn {

public:

	ofxRtMidiIn(const string name);
	virtual ~ofxRtMidiIn();
	
	static void listPorts();
	static vector<string>& getPortList();
	static int getNumPorts();
	static string getPortName(unsigned int portNumber);

	bool openPort(unsigned int portNumber);
	bool openPort(string deviceName);
	bool openVirtualPort(string portName);
	void closePort();
	
	void ignoreTypes(bool midiSysex, bool midiTiming, bool midiSense);

private:
	
	RtMidiIn midiIn;
	static RtMidiIn s_midiIn; //< for static calls (a little kludgey, I know)
	
	/// static callback for rtmidi
	static void _midiMessageCallback(double deltatime, vector<unsigned char> *message, void *userData);
};
