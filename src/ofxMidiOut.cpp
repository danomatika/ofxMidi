#include "ofxMidiOut.h"

// --------------------------------------------------------------------------------------
ofxMidiOut::ofxMidiOut(const string name) : midiout(name) {
	portNum = -1;
	bMsgInProgress = false;
}

// --------------------------------------------------------------------------------------
ofxMidiOut::~ofxMidiOut() {
	closePort();
}

// --------------------------------------------------------------------------------------
// TODO: replace cout with ofLogNotice when OF_LOG_NOTICE is the default log level
void ofxMidiOut::listPorts() {
	cout << "ofxMidiOut: " << midiout.getPortCount() << " ports available" << endl;
	for(unsigned int i = 0; i < midiout.getPortCount(); ++i){
		cout << "ofxMidiOut: " <<  i << ": " << midiout.getPortName(i) << endl;
	}
}

// --------------------------------------------------------------------------------------
vector<string>& ofxMidiOut::getPortList() {
	for(unsigned int i=0; i < midiout.getPortCount(); ++i) {
		portNames.push_back(midiout.getPortName(i));
	}
	return portNames;
}

// --------------------------------------------------------------------------------------
int ofxMidiOut::getNumPorts() {
	return midiout.getPortCount();
}

// --------------------------------------------------------------------------------------
string ofxMidiOut::getPortName(unsigned int portNumber) {
	return midiout.getPortName(portNumber);
}

// --------------------------------------------------------------------------------------
bool ofxMidiOut::openPort(unsigned int portNumber) {	
	// handle rtmidi exceptions
	try {
		midiout.closePort();
		midiout.openPort(portNumber);
	}
	catch(RtError& err) {
		ofLog(OF_LOG_ERROR, "ofxMidiOut: couldn't open port %i: %s", portNumber, err.what());
		return false;
	}
	portNum = portNumber;
	ofLog(OF_LOG_VERBOSE, "ofxMidiOut: opened port %i %s",
		portNum, midiout.getPortName(portNum).c_str());
	return true;
}

// --------------------------------------------------------------------------------------
bool ofxMidiOut::openPort(string deviceName) {
	
	// iterate through MIDI ports, find requested device
	int port = -1;
	for(unsigned int i = 0; i < midiout.getPortCount(); ++i) {
		string portName = midiout.getPortName(i);
		if(portName == deviceName) {
			port = i;
			break;
		}
	}
	
	// bail if not found
	if(port == -1) {
		ofLog(OF_LOG_ERROR, "ofxMidiOut: port \"%s\" is not available", deviceName.c_str());
		return false;
	} 
	
	return openPort(port);
}

// --------------------------------------------------------------------------------------
bool ofxMidiOut::openVirtualPort(string portName) {
	// handle rtmidi exceptions
	try {
		midiout.closePort();
		midiout.openVirtualPort(portName);
	}
	catch(RtError& err) {
		ofLog(OF_LOG_ERROR, "ofxMidiOut: couldn't open virtual port \"%s\": %s",
			portName.c_str(), err.what());
		return false;
	}
	return true;
}

// --------------------------------------------------------------------------------------
void ofxMidiOut::closePort() {
	if(portNum > -1) {
		ofLog(OF_LOG_VERBOSE, "ofxMidiOut: closing port %i %s",
			portNum, midiout.getPortName(portNum).c_str());
	}
	midiout.closePort();
	portNum = -1;
	bMsgInProgress = false;
}

// --------------------------------------------------------------------------------------
unsigned int ofxMidiOut::getPort() {
	return portNum;
}

// --------------------------------------------------------------------------------------
string ofxMidiOut::getName() {
	return midiout.getPortName(portNum);
}

// --------------------------------------------------------------------------------------
bool ofxMidiOut::isOpen() {
	return portNum > -1;
}

// --------------------------------------------------------------------------------------
void ofxMidiOut::sendNoteOn(int channel, int pitch, int velocity) {
	
	if(bMsgInProgress) {
		ofLog(OF_LOG_WARNING, "ofxMidiOut: cannot send note on, byte stream in progress");
		return;
	}

	message.clear();
	message.push_back(MIDI_NOTE_ON+ofClamp(channel-1, 0, 15));
	message.push_back(pitch);
	message.push_back(velocity);
	sendMessage();
}

// --------------------------------------------------------------------------------------
void ofxMidiOut::sendNoteOff(int channel, int pitch, int velocity) {

	if(bMsgInProgress) {
		ofLog(OF_LOG_WARNING, "ofxMidiOut: cannot send note off, byte stream in progress");
		return;
	}

	message.clear();
	message.push_back(MIDI_NOTE_OFF+(channel-1));
	message.push_back(pitch);
	message.push_back(velocity);
	sendMessage();
}

// --------------------------------------------------------------------------------------
void ofxMidiOut::sendControlChange(int channel, int controller, int value) {

	if(bMsgInProgress) {
		ofLog(OF_LOG_WARNING, "ofxMidiOut: cannot send note ctrl change, byte stream in progress");
		return;
	}
	
	message.clear();
	message.push_back(MIDI_CONTROL_CHANGE+(channel-1));
	message.push_back(controller);
	message.push_back(value);
	sendMessage();
}

// --------------------------------------------------------------------------------------
void ofxMidiOut::sendProgramChange(int channel, int value) {

	if(bMsgInProgress) {
		ofLog(OF_LOG_WARNING, "ofxMidiOut: cannot send pgm change, byte stream in progress");
		return;
	}

	message.clear();
	message.push_back(MIDI_PROGRAM_CHANGE+(channel-1));
	message.push_back(value);
	sendMessage();
}

// --------------------------------------------------------------------------------------
void ofxMidiOut::sendPitchBend(int channel, int value) {

	if(bMsgInProgress) {
		ofLog(OF_LOG_WARNING, "ofxMidiOut: cannot send pitch bend, byte stream in progress");
		return;
	}

	message.clear();
	message.push_back(MIDI_PITCH_BEND+(channel-1));
	message.push_back(value & 0x7F);		// lsb 7bit
	message.push_back((value >> 7) & 0x7F);	// msb 7bit
	sendMessage();
}

void ofxMidiOut::sendPitchBend(int channel, unsigned char lsb, unsigned char msb) {
	
	if(bMsgInProgress) {
		ofLog(OF_LOG_WARNING, "ofxMidiOut: cannot send pitch bend, byte stream in progress");
		return;
	}
	
	message.clear();
	message.push_back(MIDI_PITCH_BEND+(channel-1));
	message.push_back(lsb);
	message.push_back(msb);
	sendMessage();
}

// --------------------------------------------------------------------------------------
void ofxMidiOut::sendAftertouch(int channel, int value) {

	if(bMsgInProgress) {
		ofLog(OF_LOG_WARNING, "ofxMidiOut: cannot send aftertouch, byte stream in progress");
		return;
	}

	message.clear();
	message.push_back(MIDI_AFTERTOUCH+(channel-1));
	message.push_back(value);
	sendMessage();
}

// --------------------------------------------------------------------------------------
void ofxMidiOut::sendPolyAftertouch(int channel, int pitch, int value) {

	if(bMsgInProgress) {
		ofLog(OF_LOG_WARNING, "ofxMidiOut: cannot send poly aftertouch, byte stream in progress");
		return;
	}

	message.clear();
	message.push_back(MIDI_POLY_AFTERTOUCH+(channel-1));
	message.push_back(pitch);
	message.push_back(value);
	sendMessage();
}

// --------------------------------------------------------------------------------------
void ofxMidiOut::sendMidiByte(unsigned char byte) {

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
void ofxMidiOut::sendMidiBytes(vector<unsigned char>& bytes) {

	// don't flush if a byte stream is in progress
	if(bMsgInProgress) {
		for(int i = 0; i < bytes.size(); ++i) {
			message.push_back(bytes[i]);
		}
	}
	else {
		message.clear();
		for(int i = 0; i < bytes.size(); ++i) {
			message.push_back(bytes[i]);
		}
		sendMessage();
	}
}

//----------------------------------------------------------
ofxMidiOut& ofxMidiOut::operator<<(const NoteOn& var) {
	sendNoteOn(var.channel, var.pitch, var.velocity);
	return *this;
}

//----------------------------------------------------------
ofxMidiOut& ofxMidiOut::operator<<(const NoteOff& var) {
	sendNoteOff(var.channel, var.pitch, var.velocity);
	return *this;
}

//----------------------------------------------------------
ofxMidiOut& ofxMidiOut::operator<<(const ControlChange& var) {
	sendControlChange(var.channel, var.controller, var.value);
	return *this;
}

//----------------------------------------------------------
ofxMidiOut& ofxMidiOut::operator<<(const ProgramChange& var) {
	sendProgramChange(var.channel, var.value);
	return *this;
}

//----------------------------------------------------------
ofxMidiOut& ofxMidiOut::operator<<(const PitchBend& var) {
	sendPitchBend(var.channel, var.value);
	return *this;
}

//----------------------------------------------------------
ofxMidiOut& ofxMidiOut::operator<<(const Aftertouch& var) {
	sendAftertouch(var.channel, var.value);
	return *this;
}

//----------------------------------------------------------
ofxMidiOut& ofxMidiOut::operator<<(const PolyAftertouch& var) {
	sendPolyAftertouch(var.channel, var.pitch, var.value);
	return *this;
}

//----------------------------------------------------------
ofxMidiOut& ofxMidiOut::operator<<(const StartMidi& var) {
	if(bMsgInProgress) {
		ofLog(OF_LOG_WARNING, "ofxMidiOut: calling StartMidi when byte stream in progress");
		return *this;
	}
    message.clear();
	bMsgInProgress = true;
	return *this;
}

// --------------------------------------------------------------------------------------
ofxMidiOut& ofxMidiOut::operator<<(const FinishMidi& var) {
      
    if(!bMsgInProgress) {
    	ofLog(OF_LOG_WARNING, "ofxMidiOut: can not finish midi byte stream, stream not in progress");
		return *this;
	}
	sendMessage();
    return *this;
}

// --------------------------------------------------------------------------------------
ofxMidiOut& ofxMidiOut::operator<<(unsigned char var) {
    message.push_back(var);
	return *this;
}

// PRIVATE
// --------------------------------------------------------------------------------------
void ofxMidiOut::sendMessage() {
	// handle rtmidi exceptions
	try {
		midiout.sendMessage(&message);
	}
	catch(RtError& err) {
		ofLog(OF_LOG_ERROR, "ofxMidiOut: couldn't send message: %s", err.what());
	}
	bMsgInProgress = false;
}
