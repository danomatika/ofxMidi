#pragma once

#include "../ofxBaseMidi.h"
#import "PGMidi.h"

class ofxPGMidiOut : public ofxBaseMidiOut {

public:

	ofxPGMidiOut(const string name);
	virtual ~ofxPGMidiOut();
	
	void listPorts();
	vector<string>& getPortList();
	int getNumPorts();
	string getPortName(unsigned int portNumber);
	
	bool openPort(unsigned int portNumber);
	bool openPort(string deviceName);
	bool openVirtualPort(string portName);	///< currently noop on iOS
	void closePort();
	
private:
	
	void sendMessage();

	PGMidi * midi; ///< static global pgmidi instance
	PGMidiDestination * destination; ///< output destination
};
