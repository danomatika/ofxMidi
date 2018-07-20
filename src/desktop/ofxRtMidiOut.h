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

#include "RtMidi.h"
#include "../ofxBaseMidi.h"

class ofxRtMidiOut : public ofxBaseMidiOut {

public:

	/// set the output client name (optional)
	ofxRtMidiOut(const std::string name);
	virtual ~ofxRtMidiOut();

	static void listPorts();
	static std::vector<std::string>& getPortList();
	static int getNumPorts();
	static std::string getPortName(unsigned int portNumber);

	bool openPort(unsigned int portNumber);
	bool openPort(std::string deviceName);
	bool openVirtualPort(std::string portName);
	void closePort();
	
private:
	
	void sendMessage();

	RtMidiOut midiOut;
	static std::shared_ptr<RtMidiOut> s_midiOut; //< for static calls (a little kludgey, I know)
};
