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
#include "ofxRtMidiIn.h"

ofPtr<RtMidiIn> ofxRtMidiIn::s_midiIn;

// -----------------------------------------------------------------------------
ofxRtMidiIn::ofxRtMidiIn(const string name) :
	ofxBaseMidiIn(name), midiIn(RtMidi::UNSPECIFIED, name) {
}

// -----------------------------------------------------------------------------
ofxRtMidiIn::~ofxRtMidiIn() {
	closePort();
}

// -----------------------------------------------------------------------------
void ofxRtMidiIn::listPorts() {
	if(s_midiIn == NULL) {
		s_midiIn = ofPtr<RtMidiIn>(new RtMidiIn(RtMidi::UNSPECIFIED, "ofxMidi Client"));
	}
	ofLogNotice("ofxMidiIn") << s_midiIn->getPortCount() << " ports available";
	for(unsigned int i = 0; i < s_midiIn->getPortCount(); ++i){
		ofLogNotice("ofxMidiIn") <<  i << ": " << s_midiIn->getPortName(i);
	}
}

// -----------------------------------------------------------------------------
vector<string>& ofxRtMidiIn::getPortList() {
	if(s_midiIn == NULL) {
		s_midiIn = ofPtr<RtMidiIn>(new RtMidiIn(RtMidi::UNSPECIFIED, "ofxMidi Client"));
	}
	portList.clear();
	for(unsigned int i=0; i < s_midiIn->getPortCount(); ++i) {
		portList.push_back(s_midiIn->getPortName(i));
	}
	return portList;
}

// -----------------------------------------------------------------------------
int ofxRtMidiIn::getNumPorts() {
	if(s_midiIn == NULL) {
		s_midiIn = ofPtr<RtMidiIn>(new RtMidiIn(RtMidi::UNSPECIFIED, "ofxMidi Client"));
	}
	return s_midiIn->getPortCount();
}

// -----------------------------------------------------------------------------
string ofxRtMidiIn::getPortName(unsigned int portNumber) {
	if(s_midiIn == NULL) {
		s_midiIn = ofPtr<RtMidiIn>(new RtMidiIn(RtMidi::UNSPECIFIED, "ofxMidi Client"));
	}
	// handle rtmidi exceptions
	try {
		return s_midiIn->getPortName(portNumber);
	}
	catch(RtMidiError& err) {
		ofLogError("ofxMidiIn") << "couldn't get name for port " << portNumber << ": " << err.what();
	}
	return "";
}

// -----------------------------------------------------------------------------
bool ofxRtMidiIn::openPort(unsigned int portNumber) {	
	// handle rtmidi exceptions
	try {
		closePort();
		midiIn.setCallback(&_midiMessageCallback, this);
		midiIn.openPort(portNumber, "ofxMidi Input "+ofToString(portNumber));
	}
	catch(RtMidiError& err) {
		ofLogError("ofxMidiIn") << "couldn't get open port " << portNumber << ": " << err.what();
		return false;
	}
	portNum = portNumber;
	portName = midiIn.getPortName(portNumber);
	bOpen = true;
	ofLogVerbose("ofxMidiIn") << "opened port " << portNumber << " " << portName;
	return true;
}

// -----------------------------------------------------------------------------
bool ofxRtMidiIn::openPort(string deviceName) {
	
	// iterate through MIDI ports, find requested device
	int port = -1;
	for(unsigned int i = 0; i < midiIn.getPortCount(); ++i) {
		string name = midiIn.getPortName(i);
		if(name == deviceName) {
			port = i;
			break;
		}
	}
	
	// bail if not found
	if(port == -1) {
		ofLogError("ofxMidiIn") << "port \"" << deviceName << "\" is not available";
		return false;
	} 
	
	return openPort(port);
}

// -----------------------------------------------------------------------------
bool ofxRtMidiIn::openVirtualPort(string portName) {
	// handle rtmidi exceptions
	try {
		closePort();
		midiIn.setCallback(&_midiMessageCallback, this);
		midiIn.openVirtualPort(portName);
	}
	catch(RtMidiError& err) {
		ofLogError("ofxMidiIn") << "couldn't open virtual port \"" << portName << "\": " << err.what();
		return false;
	}
	this->portName = portName;
	bOpen = true;
	bVirtual = true;
	ofLogVerbose("ofxMidiIn") << "opened virtual port " << portName;
	return true;
}

// -----------------------------------------------------------------------------
void ofxRtMidiIn::closePort() {
	if(bVirtual && bOpen) {
		ofLogVerbose("ofxMidiIn") << "closing virtual port " << portName;
	}
	else if(bOpen && portNum > -1) {
		ofLogVerbose("ofxMidiIn") << "closing port " << portNum << " " << portName;
	}
	midiIn.closePort();
	if(bOpen)
		midiIn.cancelCallback();
	portNum = -1;
	portName = "";
	bOpen = false;
	bVirtual = false;
}

// -----------------------------------------------------------------------------
void ofxRtMidiIn::ignoreTypes(bool midiSysex, bool midiTiming, bool midiSense) {
	midiIn.ignoreTypes(midiSysex, midiTiming, midiSense);
	ofLogVerbose("ofxMidiIn") <<"ignore types on " << portName << ": sysex: " << midiSysex
		<< " timing: " << midiTiming << " sense: " << midiSense;
}
// -----------------------------------------------------------------------------
void ofxRtMidiIn::_midiMessageCallback(double deltatime, vector<unsigned char> *message, void *userData) {
	((ofxRtMidiIn*) userData)->manageNewMessage(deltatime * 1000, message); // convert s to ms
}
