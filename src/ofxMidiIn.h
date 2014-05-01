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

#include "ofxBaseMidi.h"

// choose the midi backend
#ifdef TARGET_OF_IPHONE
	#include "ios/ofxPGMidiIn.h"
	#define OFX_MIDI_IN_TYPE ofxPGMidiIn
#else // OSX, Win, Linux
	#include "desktop/ofxRtMidiIn.h"
	#define OFX_MIDI_IN_TYPE ofxRtMidiIn
#endif

///
/// a midi input port
///
/// create multiple instances to connect to multiple ports
///
/// *do not* create static instances as this will lead to a crash on Linux,
/// instead create a static ofPtr and initialize it later:
///
/// in .h:
///    class MyClass {
///
///        ...
///
///        static ofPtr<ofxMidiIn> s_midiIn;
///    }
///
/// in .cpp:
///    ofPtr<ofxMidiIn> MyClass::s_midiIn;
///
///    ...
///
///    // initialize somewhere else
///    void MyClass::setup() {
///        if(s_midiIn == NULL) {
///            s_midiIn = ofPtr<ofxMidiIn>(new ofxMidiIn("ofxMidi Client"));
///        }
///    }
///
class ofxMidiIn {

public:

	/// set the input client name (optional)
	ofxMidiIn(const string name="ofxMidiIn Client");
	virtual ~ofxMidiIn();
	
/// \section Global Port Info
	
	/// print the connected output ports
	static void listPorts();
	
	/// get a list of output port names
	/// 
	/// the vector index corresponds with the name's port number
	///
	/// note: this order may change when new devices are added/removed
	///		  from the system
	///
	static vector<string>& getPortList();
	
	/// get the number of output ports
	static int getNumPorts();
	
	/// get the name of an output port by it's number
	///
	/// returns "" if number is invalid
	///
	static string getPortName(unsigned int portNumber);

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
	///          of midi messages
	///
	void setVerbose(bool verbose);

private:

	ofPtr<ofxBaseMidiIn> midiIn;
};
