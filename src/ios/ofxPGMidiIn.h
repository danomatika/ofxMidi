#pragma once

#include "../ofxBaseMidi.h"

class ofxPGMidiIn : public ofxBaseMidiIn {

public:

	ofxPGMidiIn(const string name);
	virtual ~ofxPGMidiIn();
	
	void listPorts();
	vector<string>& getPortList();
	int getNumPorts();
	string getPortName(unsigned int portNumber);

	bool openPort(unsigned int portNumber);
	bool openPort(string deviceName);
	bool openVirtualPort(string portName);	///< currently noop on iOS
	void closePort();
	
	void ignoreTypes(bool midiSysex, bool midiTiming, bool midiSense);

	/// wrapper around manageNewMessage
	void messageReceived(double deltatime, vector<unsigned char> *message);

private:

	/// parses and sends received messages to listeners
	///void manageNewMessage(double deltatime, vector<unsigned char> *message);
	
	struct InputDelegate; // forward declaration for Obj-C wrapper
	InputDelegate * inputDelegate;	///< Obj-C midi input interface
};
