#pragma once

#include "../ofxBaseMidi.h"

class ofxMidiConnectionListener;

// reference http://syntheticbits.com/blog/?p=878
class ofxPGMidiIn : public ofxBaseMidiIn {

public:

	ofxPGMidiIn(const string name);
	virtual ~ofxPGMidiIn();
	
	static void listPorts();
	static vector<string>& getPortList();
	static int getNumPorts();
	static string getPortName(unsigned int portNumber);

	bool openPort(unsigned int portNumber);
	bool openPort(string deviceName);
	bool openVirtualPort(string portName);	///< currently noop on iOS
	void closePort();
	
	void ignoreTypes(bool midiSysex, bool midiTiming, bool midiSense);

	/// wrapper around manageNewMessage
	void messageReceived(double deltatime, vector<unsigned char> *message);
	
	// iOS specific global stuff,
	// easier to route through here thanks to Obj-C/C++ mix
	static void setConnectionListener(ofxMidiConnectionListener * listener);
	static void clearConnectionListener();
	static void enableNetworking();

private:
	
	struct InputDelegate; // forward declaration for Obj-C wrapper
	InputDelegate * inputDelegate;	///< Obj-C midi input interface
};
