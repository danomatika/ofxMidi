#include "ofxPGMidiOut.h"

// --------------------------------------------------------------------------------------
ofxPGMidiOut::ofxPGMidiOut(const string name) : midiout(name) {
	portNum = -1;
	portName = "";
	bOpen = false;
	bMsgInProgress = false;
	bVirtual = false;
}

// --------------------------------------------------------------------------------------
ofxPGMidiOut::~ofxPGMidiOut() {
	closePort();
}

// --------------------------------------------------------------------------------------
// TODO: replace cout with ofLogNotice when OF_LOG_NOTICE is the default log level
void ofxPGMidiOut::listPorts() {
	cout << "ofxPGMidiOut: " << midiout.getPortCount() << " ports available" << endl;
	for(unsigned int i = 0; i < midiout.getPortCount(); ++i){
		cout << "ofxPGMidiOut: " <<  i << ": " << midiout.getPortName(i) << endl;
	}
}

// --------------------------------------------------------------------------------------
vector<string>& ofxPGMidiOut::getPortList() {
	portList.clear();
	for(unsigned int i = 0; i < midiout.getPortCount(); ++i) {
		portList.push_back(midiout.getPortName(i));
	}
	return portList;
}

// --------------------------------------------------------------------------------------
int ofxPGMidiOut::getNumPorts() {
	return midiout.getPortCount();
}

// --------------------------------------------------------------------------------------
string ofxPGMidiOut::getPortName(unsigned int portNumber) {
	// handle rtmidi exceptions
	try {
		return midiout.getPortName(portNumber);
	}
	catch(RtError& err) {
		ofLog(OF_LOG_ERROR, "ofxMidiIn: couldn't get name for port %i: %s",
			portNumber, err.what());
	}
	return "";
}

// --------------------------------------------------------------------------------------
bool ofxPGMidiOut::openPort(unsigned int portNumber) {	
	// handle rtmidi exceptions
	try {
		closePort();
		midiout.openPort(portNumber);
	}
	catch(RtError& err) {
		ofLog(OF_LOG_ERROR, "ofxPGMidiOut: couldn't open port %i: %s", portNumber, err.what());
		return false;
	}
	portNum = portNumber;
	portName = midiout.getPortName(portNumber);
	bOpen = true;
	ofLog(OF_LOG_VERBOSE, "ofxPGMidiOut: opened port %i %s", portNum, portName.c_str());
	return true;
}

// --------------------------------------------------------------------------------------
bool ofxPGMidiOut::openPort(string deviceName) {
	
	// iterate through MIDI ports, find requested device
	int port = -1;
	for(unsigned int i = 0; i < midiout.getPortCount(); ++i) {
		string name = midiout.getPortName(i);
		if(name == deviceName) {
			port = i;
			break;
		}
	}
	
	// bail if not found
	if(port == -1) {
		ofLog(OF_LOG_ERROR, "ofxPGMidiOut: port \"%s\" is not available", deviceName.c_str());
		return false;
	} 
	
	return openPort(port);
}

// --------------------------------------------------------------------------------------
bool ofxPGMidiOut::openVirtualPort(string portName) {
	// handle rtmidi exceptions
	try {
		closePort();
		midiout.openVirtualPort(portName);
	}
	catch(RtError& err) {
		ofLog(OF_LOG_ERROR, "ofxPGMidiOut: couldn't open virtual port \"%s\": %s",
			portName.c_str(), err.what());
		return false;
	}
	
	this->portName = portName;
	bOpen = true;
	bVirtual = true;
	ofLog(OF_LOG_VERBOSE, "ofxPGMidiOut: opened virtual port %s", portName.c_str());
	return true;
}

// --------------------------------------------------------------------------------------
void ofxPGMidiOut::closePort() {
	if(bVirtual && bOpen) {
		ofLog(OF_LOG_VERBOSE, "ofxPGMidiOut: closing virtual port %s", portName.c_str());
	}
	else if(portNum > -1) {
		ofLog(OF_LOG_VERBOSE, "ofxPGMidiOut: closing port %i %s", portNum, portName.c_str());
	}
	midiout.closePort();
	portNum = -1;
	portName = "";
	bOpen = false;
	bMsgInProgress = false;
	bVirtual = false;
}

// --------------------------------------------------------------------------------------
int ofxPGMidiOut::getPort() {
	return portNum;
}

// --------------------------------------------------------------------------------------
string ofxPGMidiOut::getName() {
	return portName;
}

// --------------------------------------------------------------------------------------
bool ofxPGMidiOut::isOpen() {
	return bOpen;
}

// --------------------------------------------------------------------------------------
bool ofxPGMidiOut::isVirtual() {
	return bVirtual;
}

// --------------------------------------------------------------------------------------
void ofxPGMidiOut::sendNoteOn(int channel, int pitch, int velocity) {
	
	if(bMsgInProgress) {
		ofLog(OF_LOG_WARNING, "ofxPGMidiOut: cannot send note on, byte stream in progress");
		return;
	}

	message.clear();
	message.push_back(MIDI_NOTE_ON+ofClamp(channel-1, 0, 15));
	message.push_back(pitch);
	message.push_back(velocity);
	sendMessage();
}

// --------------------------------------------------------------------------------------
void ofxPGMidiOut::sendNoteOff(int channel, int pitch, int velocity) {

	if(bMsgInProgress) {
		ofLog(OF_LOG_WARNING, "ofxPGMidiOut: cannot send note off, byte stream in progress");
		return;
	}

	message.clear();
	message.push_back(MIDI_NOTE_OFF+(channel-1));
	message.push_back(pitch);
	message.push_back(velocity);
	sendMessage();
}

// --------------------------------------------------------------------------------------
void ofxPGMidiOut::sendControlChange(int channel, int control, int value) {

	if(bMsgInProgress) {
		ofLog(OF_LOG_WARNING, "ofxPGMidiOut: cannot send note ctrl change, byte stream in progress");
		return;
	}
	
	message.clear();
	message.push_back(MIDI_CONTROL_CHANGE+(channel-1));
	message.push_back(control);
	message.push_back(value);
	sendMessage();
}

// --------------------------------------------------------------------------------------
void ofxPGMidiOut::sendProgramChange(int channel, int value) {

	if(bMsgInProgress) {
		ofLog(OF_LOG_WARNING, "ofxPGMidiOut: cannot send pgm change, byte stream in progress");
		return;
	}

	message.clear();
	message.push_back(MIDI_PROGRAM_CHANGE+(channel-1));
	message.push_back(value);
	sendMessage();
}

// --------------------------------------------------------------------------------------
void ofxPGMidiOut::sendPitchBend(int channel, int value) {

	if(bMsgInProgress) {
		ofLog(OF_LOG_WARNING, "ofxPGMidiOut: cannot send pitch bend, byte stream in progress");
		return;
	}

	message.clear();
	message.push_back(MIDI_PITCH_BEND+(channel-1));
	message.push_back(value & 0x7F);		// lsb 7bit
	message.push_back((value >> 7) & 0x7F);	// msb 7bit
	sendMessage();
}

void ofxPGMidiOut::sendPitchBend(int channel, unsigned char lsb, unsigned char msb) {
	
	if(bMsgInProgress) {
		ofLog(OF_LOG_WARNING, "ofxPGMidiOut: cannot send pitch bend, byte stream in progress");
		return;
	}
	
	message.clear();
	message.push_back(MIDI_PITCH_BEND+(channel-1));
	message.push_back(lsb);
	message.push_back(msb);
	sendMessage();
}

// --------------------------------------------------------------------------------------
void ofxPGMidiOut::sendAftertouch(int channel, int value) {

	if(bMsgInProgress) {
		ofLog(OF_LOG_WARNING, "ofxPGMidiOut: cannot send aftertouch, byte stream in progress");
		return;
	}

	message.clear();
	message.push_back(MIDI_AFTERTOUCH+(channel-1));
	message.push_back(value);
	sendMessage();
}

// --------------------------------------------------------------------------------------
void ofxPGMidiOut::sendPolyAftertouch(int channel, int pitch, int value) {

	if(bMsgInProgress) {
		ofLog(OF_LOG_WARNING, "ofxPGMidiOut: cannot send poly aftertouch, byte stream in progress");
		return;
	}

	message.clear();
	message.push_back(MIDI_POLY_AFTERTOUCH+(channel-1));
	message.push_back(pitch);
	message.push_back(value);
	sendMessage();
}

// --------------------------------------------------------------------------------------
void ofxPGMidiOut::sendMidiByte(unsigned char byte) {

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
void ofxPGMidiOut::sendMidiBytes(vector<unsigned char>& bytes) {

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
ofxPGMidiOut& ofxPGMidiOut::operator<<(const NoteOn& var) {
	sendNoteOn(var.channel, var.pitch, var.velocity);
	return *this;
}

//----------------------------------------------------------
ofxPGMidiOut& ofxPGMidiOut::operator<<(const NoteOff& var) {
	sendNoteOff(var.channel, var.pitch, var.velocity);
	return *this;
}

//----------------------------------------------------------
ofxPGMidiOut& ofxPGMidiOut::operator<<(const ControlChange& var) {
	sendControlChange(var.channel, var.control, var.value);
	return *this;
}

//----------------------------------------------------------
ofxPGMidiOut& ofxPGMidiOut::operator<<(const ProgramChange& var) {
	sendProgramChange(var.channel, var.value);
	return *this;
}

//----------------------------------------------------------
ofxPGMidiOut& ofxPGMidiOut::operator<<(const PitchBend& var) {
	sendPitchBend(var.channel, var.value);
	return *this;
}

//----------------------------------------------------------
ofxPGMidiOut& ofxPGMidiOut::operator<<(const Aftertouch& var) {
	sendAftertouch(var.channel, var.value);
	return *this;
}

//----------------------------------------------------------
ofxPGMidiOut& ofxPGMidiOut::operator<<(const PolyAftertouch& var) {
	sendPolyAftertouch(var.channel, var.pitch, var.value);
	return *this;
}

//----------------------------------------------------------
ofxPGMidiOut& ofxPGMidiOut::operator<<(const StartMidi& var) {
	if(bMsgInProgress) {
		ofLog(OF_LOG_WARNING, "ofxPGMidiOut: calling StartMidi when byte stream in progress");
		return *this;
	}
    message.clear();
	bMsgInProgress = true;
	return *this;
}

// --------------------------------------------------------------------------------------
ofxPGMidiOut& ofxPGMidiOut::operator<<(const FinishMidi& var) {
    if(!bMsgInProgress) {
    	ofLog(OF_LOG_WARNING, "ofxPGMidiOut: can not finish midi byte stream, stream not in progress");
		return *this;
	}
	sendMessage();
    return *this;
}

// --------------------------------------------------------------------------------------
ofxPGMidiOut& ofxPGMidiOut::operator<<(unsigned char var) {
    message.push_back(var);
	return *this;
}

// PRIVATE
// --------------------------------------------------------------------------------------
void ofxPGMidiOut::sendMessage() {
	// handle rtmidi exceptions
	try {
		midiout.sendMessage(&message);
	}
	catch(RtError& err) {
		ofLog(OF_LOG_ERROR, "ofxPGMidiOut: couldn't send message: %s", err.what());
	}
	bMsgInProgress = false;
}
