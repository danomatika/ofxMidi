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
#include "ofxMidiIn.h"

// -----------------------------------------------------------------------------
ofxMidiIn::ofxMidiIn(const string name) {
	midiIn = ofPtr<ofxBaseMidiIn>(new OFX_MIDI_IN_TYPE(name));
}

// -----------------------------------------------------------------------------
ofxMidiIn::~ofxMidiIn() {}

// -----------------------------------------------------------------------------
void ofxMidiIn::listPorts() {
	#ifdef TARGET_OF_IPHONE
		ofxPGMidiIn::listPorts();
	#else
		ofxRtMidiIn::listPorts();
	#endif
}

// -----------------------------------------------------------------------------
vector<string>& ofxMidiIn::getPortList() {
	#ifdef TARGET_OF_IPHONE
		return ofxPGMidiIn::getPortList();
	#else
		return ofxRtMidiIn::getPortList();
	#endif
}

// -----------------------------------------------------------------------------
int ofxMidiIn::getNumPorts() {
	#ifdef TARGET_OF_IPHONE
		return ofxPGMidiIn::getNumPorts();
	#else
		return ofxRtMidiIn::getNumPorts();
	#endif
}

// -----------------------------------------------------------------------------
string ofxMidiIn::getPortName(unsigned int portNumber) {
	#ifdef TARGET_OF_IPHONE
		return ofxPGMidiIn::getPortName(portNumber);
	#else
		return ofxRtMidiIn::getPortName(portNumber);
	#endif
}

// -----------------------------------------------------------------------------
bool ofxMidiIn::openPort(unsigned int portNumber) {	
	return midiIn->openPort(portNumber);
}

// -----------------------------------------------------------------------------
bool ofxMidiIn::openPort(string deviceName) {
	return midiIn->openPort(deviceName);
}

// -----------------------------------------------------------------------------
bool ofxMidiIn::openVirtualPort(string portName) {
	return midiIn->openVirtualPort(portName);
}

// -----------------------------------------------------------------------------
void ofxMidiIn::closePort() {
	midiIn->closePort();
}

// -----------------------------------------------------------------------------
int ofxMidiIn::getPort() {
	return midiIn->getPort();
}

// -----------------------------------------------------------------------------
string ofxMidiIn::getName() {
	return midiIn->getName();
}

// -----------------------------------------------------------------------------
bool ofxMidiIn::isOpen() {
	return midiIn->isOpen();
}

bool ofxMidiIn::isVirtual() {
	return midiIn->isVirtual();
}

// -----------------------------------------------------------------------------
void ofxMidiIn::ignoreTypes(bool midiSysex, bool midiTiming, bool midiSense) {
	midiIn->ignoreTypes(midiSysex, midiTiming, midiSense);
}

// -----------------------------------------------------------------------------
void ofxMidiIn::addListener(ofxMidiListener* listener) {
	midiIn->addListener(listener);
}

// -----------------------------------------------------------------------------
void ofxMidiIn::removeListener(ofxMidiListener* listener) {
	midiIn->removeListener(listener);
}

// -----------------------------------------------------------------------------
void ofxMidiIn::setVerbose(bool verbose) {
	midiIn->setVerbose(verbose);
}
