#pragma once

#include "../ofxBaseMidi.h"

class ofxPGMidiOut : public ofxBaseMidiOut {

public:

	ofxPGMidiOut(const string name);
	virtual ~ofxPGMidiOut();
	
	static void listPorts();
	static vector<string>& getPortList();
	static int getNumPorts();
	static string getPortName(unsigned int portNumber);
	
	bool openPort(unsigned int portNumber);
	bool openPort(string deviceName);
	bool openVirtualPort(string portName);	///< currently noop on iOS
	void closePort();
	
private:
	
	void sendMessage();

	struct Destination; // forward declaration for Obj-C wrapper
	Destination * destination; ///< output destination
};
