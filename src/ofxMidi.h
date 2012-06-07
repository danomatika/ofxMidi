#pragma once

#include "ofxMidiIn.h"
#include "ofxMidiOut.h"

/// receives iOS midi device (dis)connection events
class ofxMidiConnectionListener {

public:

	ofxMidiConnectionListener() {}
	virtual ~ofxMidiConnectionListener() {}
	
	virtual void midiInputAdded(string name, bool isNetwork=false);
	virtual void midiInputRemoved(string name, bool isNetwork=false);
	
	virtual void midiOutputAdded(string nam, bool isNetwork=false);
	virtual void midiOutputRemoved(string name, bool isNetwork=false);
};

// global static access
class ofxMidi {
	
public:

/// \section iOS Specific
	
	/// set a listener to receieve device (dis)connection events
	///
	/// don't forget to clear before the listener is deallocated!
	///
	/// note: these are noops on Mac, Win, & Linux
	///
	static void setConnectionListener(ofxMidiConnectionListener * listener);
	static void clearConnectionListener();
	
	/// enables the network midi session between iOS and Mac OSX on a
	/// local wifi network
	///
	/// in ofxMidi: open the input/outport network ports named "Session 1"
	///
	/// on OSX: use the Audio MIDI Setup Utility to connect to the iOS device
	///
	/// note: this is a noop on Mac, Win, & Linux
	///
	static void enableNetworking();
};
