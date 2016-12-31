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

/// \section Util

	/// convert midi note to frequency in Hz
	/// ala the [mtof] object in Max & Pure Data
	static float mtof(float note);

	/// convert a frequency in Hz to a MIDI note
	/// ala the [ftom] object in Max & Pure Data
	static float ftom(float frequency);

/// \section iOS Specific
	
	/// set a listener to receieve iOS device (dis)connection events
	///
	/// don't forget to clear before the listener is deallocated!
	///
	/// note: these are noops on Mac, Win, & Linux
	///
	static void setConnectionListener(ofxMidiConnectionListener *listener);

	/// clear iOS device event receiver
	static void clearConnectionListener();
	
	/// enables the network midi session between iOS and macOS on a
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
