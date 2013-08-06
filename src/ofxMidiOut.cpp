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
#include "ofxMidiOut.h"

// -----------------------------------------------------------------------------
ofxMidiOut::ofxMidiOut(const string name) {
	midiOut = ofPtr<ofxBaseMidiOut>(new OFX_MIDI_OUT_TYPE(name));
}

// -----------------------------------------------------------------------------
ofxMidiOut::~ofxMidiOut() {}

// -----------------------------------------------------------------------------
void ofxMidiOut::listPorts() {
	#ifdef TARGET_OF_IPHONE
		ofxPGMidiOut::listPorts();
	#else
		ofxRtMidiOut::listPorts();
	#endif
}

// -----------------------------------------------------------------------------
vector<string>& ofxMidiOut::getPortList() {
	#ifdef TARGET_OF_IPHONE
		return ofxPGMidiOut::getPortList();
	#else
		return ofxRtMidiOut::getPortList();
	#endif
}

// -----------------------------------------------------------------------------
int ofxMidiOut::getNumPorts() {
	#ifdef TARGET_OF_IPHONE
		return ofxPGMidiOut::getNumPorts();
	#else
		return ofxRtMidiOut::getNumPorts();
	#endif
}

// -----------------------------------------------------------------------------
string ofxMidiOut::getPortName(unsigned int portNumber) {
	#ifdef TARGET_OF_IPHONE
		return ofxPGMidiOut::getPortName(portNumber);
	#else
		return ofxRtMidiOut::getPortName(portNumber);
	#endif
}

// -----------------------------------------------------------------------------
bool ofxMidiOut::openPort(unsigned int portNumber) {
	return midiOut->openPort(portNumber);
}

// -----------------------------------------------------------------------------
bool ofxMidiOut::openPort(string deviceName) {
	return midiOut->openPort(deviceName);
}

// -----------------------------------------------------------------------------
bool ofxMidiOut::openVirtualPort(string portName) {
	return midiOut->openVirtualPort(portName);
}

// -----------------------------------------------------------------------------
void ofxMidiOut::closePort() {
	midiOut->closePort();
}

// -----------------------------------------------------------------------------
int ofxMidiOut::getPort() {
	return midiOut->getPort();
}

// -----------------------------------------------------------------------------
string ofxMidiOut::getName() {
	return midiOut->getName();
}

// -----------------------------------------------------------------------------
bool ofxMidiOut::isOpen() {
	return midiOut->isOpen();
}

// -----------------------------------------------------------------------------
bool ofxMidiOut::isVirtual() {
	return midiOut->isVirtual();
}

// -----------------------------------------------------------------------------
void ofxMidiOut::sendNoteOn(int channel, int pitch, int velocity) {
	midiOut->sendNoteOn(channel, pitch, velocity);
}

// -----------------------------------------------------------------------------
void ofxMidiOut::sendNoteOff(int channel, int pitch, int velocity) {
	midiOut->sendNoteOff(channel, pitch, velocity);
}

// -----------------------------------------------------------------------------
void ofxMidiOut::sendControlChange(int channel, int control, int value) {
	midiOut->sendControlChange(channel, control, value);
}

// -----------------------------------------------------------------------------
void ofxMidiOut::sendProgramChange(int channel, int value) {
	midiOut->sendProgramChange(channel, value);
}

// -----------------------------------------------------------------------------
void ofxMidiOut::sendPitchBend(int channel, int value) {
	midiOut->sendPitchBend(channel, value);
}

void ofxMidiOut::sendPitchBend(int channel, unsigned char lsb, unsigned char msb) {
	midiOut->sendPitchBend(channel, lsb, msb);
}

// -----------------------------------------------------------------------------
void ofxMidiOut::sendAftertouch(int channel, int value) {
	midiOut->sendAftertouch(channel, value);
}

// -----------------------------------------------------------------------------
void ofxMidiOut::sendPolyAftertouch(int channel, int pitch, int value) {
	midiOut->sendPolyAftertouch(channel, pitch, value);
}

// -----------------------------------------------------------------------------
void ofxMidiOut::sendMidiByte(unsigned char byte) {
	midiOut->sendMidiByte(byte);
}

//----------------------------------------------------------
void ofxMidiOut::sendMidiBytes(vector<unsigned char>& bytes) {
	midiOut->sendMidiBytes(bytes);
}

//----------------------------------------------------------
ofxMidiOut& ofxMidiOut::operator<<(const NoteOn& var) {
	midiOut->sendNoteOn(var.channel, var.pitch, var.velocity);
	return *this;
}

//----------------------------------------------------------
ofxMidiOut& ofxMidiOut::operator<<(const NoteOff& var) {
	midiOut->sendNoteOff(var.channel, var.pitch, var.velocity);
	return *this;
}

//----------------------------------------------------------
ofxMidiOut& ofxMidiOut::operator<<(const ControlChange& var) {
	midiOut->sendControlChange(var.channel, var.control, var.value);
	return *this;
}

//----------------------------------------------------------
ofxMidiOut& ofxMidiOut::operator<<(const ProgramChange& var) {
	midiOut->sendProgramChange(var.channel, var.value);
	return *this;
}

//----------------------------------------------------------
ofxMidiOut& ofxMidiOut::operator<<(const PitchBend& var) {
	midiOut->sendPitchBend(var.channel, var.value);
	return *this;
}

//----------------------------------------------------------
ofxMidiOut& ofxMidiOut::operator<<(const Aftertouch& var) {
	midiOut->sendAftertouch(var.channel, var.value);
	return *this;
}

//----------------------------------------------------------
ofxMidiOut& ofxMidiOut::operator<<(const PolyAftertouch& var) {
	midiOut->sendPolyAftertouch(var.channel, var.pitch, var.value);
	return *this;
}

//----------------------------------------------------------
ofxMidiOut& ofxMidiOut::operator<<(const StartMidi& var) {
	midiOut->startMidiStream();
	return *this;
}

// ---------------------------------------------------------
ofxMidiOut& ofxMidiOut::operator<<(const FinishMidi& var) {
	midiOut->finishMidiStream();
	return *this;
}

// ---------------------------------------------------------
ofxMidiOut& ofxMidiOut::operator<<(unsigned char var) {
	midiOut->sendMidiByte(var);
	return *this;
}
