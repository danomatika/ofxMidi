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
#include "ofxBaseMidi.h"

// MIDI IN

vector<string> ofxBaseMidiIn::portList;

// -----------------------------------------------------------------------------
ofxBaseMidiIn::ofxBaseMidiIn(const string name) {
	portNum = -1;
	portName = "";
	bOpen = false;
	bVerbose = false;
	bVirtual = false;
}
// -----------------------------------------------------------------------------
int ofxBaseMidiIn::getPort() {
	return portNum;
}

// -----------------------------------------------------------------------------
string ofxBaseMidiIn::getName() {
	return portName;
}

// -----------------------------------------------------------------------------
bool ofxBaseMidiIn::isOpen() {
	return bOpen;
}

// -----------------------------------------------------------------------------
bool ofxBaseMidiIn::isVirtual() {
	return bVirtual;
}

// -----------------------------------------------------------------------------
void ofxBaseMidiIn::addListener(ofxMidiListener* listener) {
	ofAddListener(newMessageEvent, listener, &ofxMidiListener::newMidiMessage);
}

// -----------------------------------------------------------------------------
void ofxBaseMidiIn::removeListener(ofxMidiListener* listener) {
	ofRemoveListener(newMessageEvent, listener, &ofxMidiListener::newMidiMessage);
}

// -----------------------------------------------------------------------------
void ofxBaseMidiIn::setVerbose(bool verbose) {
	bVerbose = verbose;
}

// PRIVATE
// -----------------------------------------------------------------------------
void ofxBaseMidiIn::manageNewMessage(double deltatime, vector<unsigned char> *message) {
			
	// parse message and fill event
	ofxMidiMessage midiMessage(message);

	if((message->at(0)) >= MIDI_SYSEX) {
		midiMessage.status = (MidiStatus)(message->at(0) & 0xFF);
		midiMessage.channel = 0;
	} else {
		midiMessage.status = (MidiStatus) (message->at(0) & 0xF0);
		midiMessage.channel = (int) (message->at(0) & 0x0F)+1;
	}
	
	midiMessage.deltatime = deltatime;// * 1000; // convert s to ms
	midiMessage.portNum = portNum;
	midiMessage.portName = portName;
	
	switch(midiMessage.status) {
		case MIDI_NOTE_ON :
		case MIDI_NOTE_OFF:
			midiMessage.pitch = (int) message->at(1);
			midiMessage.velocity = (int) message->at(2);
			break;
		case MIDI_CONTROL_CHANGE:
			midiMessage.control = (int) message->at(1);
			midiMessage.value = (int) message->at(2);
			break;
		case MIDI_PROGRAM_CHANGE:
		case MIDI_AFTERTOUCH:
			midiMessage.value = (int) message->at(1);
			break;
		case MIDI_PITCH_BEND:
			midiMessage.value = (int) (message->at(2) << 7) +
								(int) message->at(1); // msb + lsb
			break;
		case MIDI_POLY_AFTERTOUCH:
			midiMessage.pitch = (int) message->at(1);
			midiMessage.value = (int) message->at(2);
			break;
		default:
			break;
	}
	
	if(bVerbose) {
		ofLogVerbose("ofxMidiIn") << midiMessage.toString();
	}
	
	// send event to listeners
	ofNotifyEvent(newMessageEvent, midiMessage, this);
}

// MIDI OUT

vector<string> ofxBaseMidiOut::portList;

// -----------------------------------------------------------------------------
ofxBaseMidiOut::ofxBaseMidiOut(const string name) {
	portNum = -1;
	portName = "";
	bOpen = false;
	bMsgInProgress = false;
	bVirtual = false;
}

// -----------------------------------------------------------------------------
int ofxBaseMidiOut::getPort() {
	return portNum;
}

// -----------------------------------------------------------------------------
string ofxBaseMidiOut::getName() {
	return portName;
}

// -----------------------------------------------------------------------------
bool ofxBaseMidiOut::isOpen() {
	return bOpen;
}

// -----------------------------------------------------------------------------
bool ofxBaseMidiOut::isVirtual() {
	return bVirtual;
}

// -----------------------------------------------------------------------------
void ofxBaseMidiOut::sendNoteOn(int channel, int pitch, int velocity) {
	
	if(bMsgInProgress) {
		ofLogWarning("ofxMidiOut") << "cannot send note on, byte stream in progress";
		return;
	}

	message.clear();
	message.push_back(MIDI_NOTE_ON+channel-1);
	message.push_back(pitch);
	message.push_back(velocity);
	sendMessage();
}

// -----------------------------------------------------------------------------
void ofxBaseMidiOut::sendNoteOff(int channel, int pitch, int velocity) {

	if(bMsgInProgress) {
		ofLogWarning("ofxMidiOut") << "cannot send note off, byte stream in progress";
		return;
	}

	message.clear();
	message.push_back(MIDI_NOTE_OFF+(channel-1));
	message.push_back(pitch);
	message.push_back(velocity);
	sendMessage();
}

// -----------------------------------------------------------------------------
void ofxBaseMidiOut::sendControlChange(int channel, int control, int value) {

	if(bMsgInProgress) {
		ofLogWarning("ofxMidiOut") << "cannot send note ctrl change, byte stream in progress";
		return;
	}
	
	message.clear();
	message.push_back(MIDI_CONTROL_CHANGE+(channel-1));
	message.push_back(control);
	message.push_back(value);
	sendMessage();
}

// -----------------------------------------------------------------------------
void ofxBaseMidiOut::sendProgramChange(int channel, int value) {

	if(bMsgInProgress) {
		ofLogWarning("ofxMidiOut") << "cannot send pgm change, byte stream in progress";
		return;
	}

	message.clear();
	message.push_back(MIDI_PROGRAM_CHANGE+(channel-1));
	message.push_back(value);
	sendMessage();
}

// -----------------------------------------------------------------------------
void ofxBaseMidiOut::sendPitchBend(int channel, int value) {

	if(bMsgInProgress) {
		ofLogWarning("ofxMidiOut") << "cannot send pitch bend, byte stream in progress";
		return;
	}

	message.clear();
	message.push_back(MIDI_PITCH_BEND+(channel-1));
	message.push_back(value & 0x7F);		// lsb 7bit
	message.push_back((value >> 7) & 0x7F);	// msb 7bit
	sendMessage();
}

void ofxBaseMidiOut::sendPitchBend(int channel, unsigned char lsb, unsigned char msb) {
	
	if(bMsgInProgress) {
		ofLogWarning("ofxMidiOut") << "cannot send pitch bend, byte stream in progress";
		return;
	}
	
	message.clear();
	message.push_back(MIDI_PITCH_BEND+(channel-1));
	message.push_back(lsb);
	message.push_back(msb);
	sendMessage();
}

// -----------------------------------------------------------------------------
void ofxBaseMidiOut::sendAftertouch(int channel, int value) {

	if(bMsgInProgress) {
		ofLogWarning("ofxMidiOut") << "cannot send aftertouch, byte stream in progress";
		return;
	}

	message.clear();
	message.push_back(MIDI_AFTERTOUCH+(channel-1));
	message.push_back(value);
	sendMessage();
}

// -----------------------------------------------------------------------------
void ofxBaseMidiOut::sendPolyAftertouch(int channel, int pitch, int value) {

	if(bMsgInProgress) {
		ofLogWarning("ofxMidiOut") << "cannot send poly aftertouch, byte stream in progress";
		return;
	}

	message.clear();
	message.push_back(MIDI_POLY_AFTERTOUCH+(channel-1));
	message.push_back(pitch);
	message.push_back(value);
	sendMessage();
}

// -----------------------------------------------------------------------------
void ofxBaseMidiOut::sendMidiByte(unsigned char byte) {

	// don't flush if a byte stream is in progress
	if(bMsgInProgress) {
		message.push_back(byte);
	}
	else {
		message.clear();
		message.push_back(byte);
		sendMessage();
	}
}

//----------------------------------------------------------
void ofxBaseMidiOut::sendMidiBytes(vector<unsigned char>& bytes) {

	// don't flush if a byte stream is in progress
	if(bMsgInProgress) {
		for(unsigned int i = 0; i < bytes.size(); ++i) {
			message.push_back(bytes[i]);
		}
	}
	else {
		message.clear();
		for(unsigned int i = 0; i < bytes.size(); ++i) {
			message.push_back(bytes[i]);
		}
		sendMessage();
	}
}

//----------------------------------------------------------
void ofxBaseMidiOut::startMidiStream() {
	if(bMsgInProgress) {
		ofLogWarning("ofxMidiOut") << "calling StartMidi when byte stream in progress";
		return;
	}
    message.clear();
	bMsgInProgress = true;
}

// -----------------------------------------------------------------------------
void ofxBaseMidiOut::finishMidiStream() {
    if(!bMsgInProgress) {
    	ofLogWarning("ofxMidiOut") << "can not finish midi byte stream, stream not in progress";
		return;
	}
	sendMessage();
}
