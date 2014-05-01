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

#include "ofConstants.h"
#include "ofEvents.h"
#include "ofxMidiConstants.h"

class ofxMidiMessage;

/// receives midi messages
class ofxMidiListener {

public:

	ofxMidiListener() {}
	virtual ~ofxMidiListener() {}
	
	virtual void newMidiMessage(ofxMidiMessage& msg)=0;
};

/// a single multi byte MIDI message
///
/// check status type and grab data:
/// 
/// if(midiArg.status == MIDI_NOTE_ON) {
///     cout << "note on " << midiArg.channel
///          << midiArg.note << " " << midiArg.velocity
///          << endl;
/// }
///
/// the message-specific types are only set for the appropriate
/// message types ie pitch is only set for noteon, noteoff, and
/// polyaftertouch messages
///
class ofxMidiMessage: public ofEventArgs {

public:

/// \section Variables

	MidiStatus status;
	int channel;        //< 1 - 16

	/// message-specific values,
	/// converted from raw bytes
	int pitch;          //< 0 - 127
	int velocity;       //< 0 - 127
	int control;        //< 0 - 127
	int value;          //< depends on message status type
	
	/// raw bytes
	vector<unsigned char> bytes;
	
	/// delta time since last message in ms
	double deltatime;

	/// the input port we received this message from
	///
	/// note: portNum will be -1 from a virtual port
	///
	int portNum;
	string portName;

/// \section Main

	ofxMidiMessage();
	ofxMidiMessage(vector<unsigned char>* rawBytes);
	ofxMidiMessage(const ofxMidiMessage& from);
	ofxMidiMessage& operator=(const ofxMidiMessage& from);
	
	/// clear the message contents, also resets status
	void clear();
	
/// \section Util
	
	/// get the raw message as a string in the format:
	///
	/// PortName: status channel [ raw bytes in hex ] deltatime
	///
	string toString();

	/// get a midi status byte as a string
	/// ie "Note On", "Note Off", "Control Change", etc
	static string getStatusString(MidiStatus status);
};

typedef ofEvent<ofxMidiMessage> ofxMidiEvent;
