#pragma once

#include "ofMain.h"

#import "PGMidi.h"

@interface ofxMidiDelegate () <PGMidiDelegate, PGMidiSourceDelegate>
- (void) midiSource:(PGMidiSource*)input midiReceived:(const MIDIPacketList *)packetList;
@end

///
/// a midi output port
///
/// create multiple instances to connect to multiple ports
///
class ofxiOSMidiInterface {

public:

	/// set the input client name (optional)
	ofxiOSMidiInterface();
	virtual ~ofxiOSMidiInterface();
	
/// \section Port Info
	
	/// print the connected output ports
	void listPorts();
	
	/// get a list of output port names
	/// 
	/// the vector index corresponds with the name's port number
	///
	/// note: this order may change when new devices are added/removed
	///		  from the system
	///
	vector<string>& getPortList();
	
	/// get the number of output ports
	int getNumPorts();
	
	/// get the name of an output port by it's number
	///
	/// returns "" if number is invalid
	///
	string getPortName(unsigned int portNumber);

/// \section Connection
	
	/// connect to an output port
	///
	/// setting port = 0 will open the first available
	///
	bool openPort(unsigned int portNumber=0);
	bool openPort(string deviceName);
	
	/// create and connect to a virtual output port (MacOS and Linux ALSA only)
	///
	/// allows for connections between software
	///
	/// note: a connected virtual port has a portNum = -1
	///	note: an open virtual port ofxMidiIn object cannot see it's virtual
	///       own virtual port when listing ports
	///
	bool openVirtualPort(string portName="ofxMidi Virtual Input");
	
	/// close the port connection
	void closePort();
	
	/// get the port number if connected
	///
	/// returns -1 if not connected or this is a virtual port
	///
	int getPort();
	
	/// get the connected output port name
	///
	/// returns "" if not connected
	///
	string getName();
	
	/// returns true if connected
	bool isOpen();
	
	/// returns true if this is a virtual port
	bool isVirtual();
	
/// \section Receiving

	/// specify if certain message types should be ignored
	///
	/// sysex messages are ignored by default
	///
	/// timing and active sensing messages have high data rates
	/// and are ignored by default 
	///
	void ignoreTypes(bool midiSysex=true, bool midiTiming=true, bool midiSense=true);

	/// add/remove listener for incoming midi events
	///
	/// listeners receive from *all* incoming midi channels
	///
	void addListener(ofxMidiListener* listener);
	void removeListener(ofxMidiListener* listener);

	/// set to verbose = true to print received byte info
	///
	/// warning: this will impact performance with large numbers
	///			 of midi messages
	///
	void setVerbose(bool verbose);

private:

	/// parses and sends received messages to listeners
	void manageNewMessage(double deltatime, vector<unsigned char> *message);
	
	PGMidi * midi;
	ofxMidiDelegate * midiDelegate;
	
	
	RtMidiIn midiin;
	int portNum;				//< current port num, -1 if not connected
	string portName;			//< current port name, "" if not connected
	
	vector<string> portList;	//< list of port names
	ofEvent<ofxMidiMessage> newMessageEvent;
	
	bool bOpen;					//< is the port currently open?
	bool bVerbose;				//< print incoming bytes?
	bool bVirtual;				//< are we connected to a virtual port?
	
	/// static callback for rtmidi
	static void _midiMessageCallback(double deltatime, vector< unsigned char > *message, void *userData);
};
