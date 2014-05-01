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
#include "ofUtils.h"

#include "ofxMidiConstants.h"
#include "ofxMidiMessage.h"
#include "ofxMidiTypes.h"

/// a base midi input port
///
/// see ofxMidiIn for functional documentation
///
class ofxBaseMidiIn {

public:

	ofxBaseMidiIn(const string name);
	virtual ~ofxBaseMidiIn() {}
	
	virtual bool openPort(unsigned int portNumber) = 0;
	virtual bool openPort(string deviceName) = 0;
	virtual bool openVirtualPort(string portName) = 0;
	virtual void closePort() = 0;
	
	int getPort();
	string getName();
	bool isOpen();
	bool isVirtual();

	virtual void ignoreTypes(bool midiSysex=true, bool midiTiming=true,
		bool midiSense=true) = 0;

	void addListener(ofxMidiListener* listener);
	void removeListener(ofxMidiListener* listener);

	void setVerbose(bool verbose);

protected:

	/// parses and sends received raw messages to listeners
	void manageNewMessage(double deltatime, vector<unsigned char> *message);
	
	int portNum;     //< current port num, -1 if not connected
	string portName; //< current port name, "" if not connected
	
	static vector<string> portList; //< list of port names
	ofEvent<ofxMidiMessage> newMessageEvent;
	
	bool bOpen;    //< is the port currently open?
	bool bVerbose; //< print incoming bytes?
	bool bVirtual; //< are we connected to a virtual port?
};

///
/// a midi output port
///
/// see ofxMidiOut for functional documentation
///
class ofxBaseMidiOut {

public:

	ofxBaseMidiOut(const string name);
	virtual ~ofxBaseMidiOut() {}
	
	virtual bool openPort(unsigned int portNumber=0) = 0;
	virtual bool openPort(string deviceName) = 0;
	virtual bool openVirtualPort(string portName) = 0;
	virtual void closePort() = 0;
	
	int getPort();
	string getName();
	bool isOpen();
	bool isVirtual();
	
	void sendNoteOn(int channel, int pitch, int velocity);
	void sendNoteOff(int channel, int pitch, int velocity);
	void sendControlChange(int channel, int control, int value);
	void sendProgramChange(int channel, int value);
	void sendPitchBend(int channel, int value);
	void sendPitchBend(int channel, unsigned char lsb, unsigned char msb);
	void sendAftertouch(int channel, int value);
	void sendPolyAftertouch(int channel, int pitch, int value);
	
	void sendMidiByte(unsigned char byte);
	void sendMidiBytes(vector<unsigned char>& bytes);
	
	void startMidiStream();
	void finishMidiStream();
	
protected:
	
	/// sends current message
	virtual void sendMessage() = 0;
	
	int portNum;         //< current port num, -1 if not connected
	string portName;     //< current port name, "" if not connected
	
	static vector<string> portList; //< list of port names
	vector<unsigned char> message;  //< message byte buffer
	
	bool bOpen;          //< is the port currently open?
	bool bMsgInProgress; //< used with byte stream
	bool bVirtual;       //< are we connected to a virtual port?
};
