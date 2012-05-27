#include "ofxMidiIn.h"

// -----------------------------------------------------------------------------
ofxMidiIn::ofxMidiIn(const string name) {
	midiIn = ofPtr<ofxBaseMidiIn>(new OFX_MIDI_IN_TYPE(name));
}

// -----------------------------------------------------------------------------
ofxMidiIn::~ofxMidiIn() {
	closePort();
}

// -----------------------------------------------------------------------------
void ofxMidiIn::listPorts() {
	midiIn->listPorts();
}

// -----------------------------------------------------------------------------
vector<string>& ofxMidiIn::getPortList() {
	return midiIn->getPortList();
}

// -----------------------------------------------------------------------------
int ofxMidiIn::getNumPorts() {
	return midiIn->getNumPorts();
}

// -----------------------------------------------------------------------------
string ofxMidiIn::getPortName(unsigned int portNumber) {
	return midiIn->getPortName(portNumber);
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
