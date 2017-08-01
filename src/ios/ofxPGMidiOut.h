/*
 * Copyright (c) 2013 Dan Wilcox <danomatika@gmail.com>
 *
 * BSD Simplified License.
 * For information on usage and redistribution, and for a DISCLAIMER OF ALL
 * WARRANTIES, see the file, "LICENSE.txt," in this distribution.
 *
 * See https://github.com/danomatika/ofxMidi for documentation
 *
 */
#pragma once

#include "../ofxBaseMidi.h"

class ofxPGMidiOut : public ofxBaseMidiOut {

public:

	ofxPGMidiOut(const string name, ofxMidiApi api);
	virtual ~ofxPGMidiOut();
	
	static void listPorts();
	static vector<string>& getPortList();
	static int getNumPorts();
	static string getPortName(unsigned int portNumber);
	
	bool openPort(unsigned int portNumber);
	bool openPort(string deviceName);
	bool openVirtualPort(string portName); ///< currently noop on iOS
	void closePort();
	
private:
	
	void sendMessage();

	struct Destination; // forward declaration for Obj-C wrapper
	Destination * destination; ///< output destination
};
