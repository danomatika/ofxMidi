#include "ofxMidiIn.h"

// --------------------------------------------------------------------------------------
ofxMidiIn::ofxMidiIn(const string name) : midiin(name) {
	portNum = -1;
	portName = "";
	bOpen = false;
	bVerbose = false;
	bVirtual = false;
}

// --------------------------------------------------------------------------------------
ofxMidiIn::~ofxMidiIn() {
	closePort();
}

// --------------------------------------------------------------------------------------
// TODO: replace cout with ofLogNotice when OF_LOG_NOTICE is the default log level
void ofxMidiIn::listPorts() {
	cout << "ofxMidiIn: " << midiin.getPortCount() << " ports available" << endl;
	for(unsigned int i = 0; i < midiin.getPortCount(); ++i){
		cout << "ofxMidiIn: " <<  i << ": " << midiin.getPortName(i) << endl;
	}
}

// --------------------------------------------------------------------------------------
vector<string>& ofxMidiIn::getPortList() {
	portList.clear();
	for(unsigned int i=0; i < midiin.getPortCount(); ++i) {
		portList.push_back(midiin.getPortName(i));
	}
	return portList;
}

// --------------------------------------------------------------------------------------
int ofxMidiIn::getNumPorts() {
	return midiin.getPortCount();
}

// --------------------------------------------------------------------------------------
string ofxMidiIn::getPortName(unsigned int portNumber) {
	// handle rtmidi exceptions
	try {
		return midiin.getPortName(portNumber);
	}
	catch(RtError& err) {
		ofLog(OF_LOG_ERROR, "ofxMidiIn: couldn't get name for port %i: %s",
			portNumber, err.what());
	}
	return "";
}

// --------------------------------------------------------------------------------------
bool ofxMidiIn::openPort(unsigned int portNumber) {	
	// handle rtmidi exceptions
	try {
		closePort();
		midiin.setCallback(&_midiMessageCallback, this);
		midiin.openPort(portNumber);
	}
	catch(RtError& err) {
		ofLog(OF_LOG_ERROR, "ofxMidiIn: couldn't open port %i: %s", portNumber, err.what());
		return false;
	}
	portNum = portNumber;
	portName = midiin.getPortName(portNumber);
	bOpen = true;
	ofLog(OF_LOG_VERBOSE, "ofxMidiIn: opened port %i %s",
		portNum, portName.c_str());
	return true;
}

// --------------------------------------------------------------------------------------
bool ofxMidiIn::openPort(string deviceName) {
	
	// iterate through MIDI ports, find requested device
	int port = -1;
	for(unsigned int i = 0; i < midiin.getPortCount(); ++i) {
		string name = midiin.getPortName(i);
		if(name == deviceName) {
			port = i;
			break;
		}
	}
	
	// bail if not found
	if(port == -1) {
		ofLog(OF_LOG_ERROR, "ofxMidiIn: port \"%s\" is not available", deviceName.c_str());
		return false;
	} 
	
	return openPort(port);
}

// --------------------------------------------------------------------------------------
bool ofxMidiIn::openVirtualPort(string portName) {
	// handle rtmidi exceptions
	try {
		closePort();
		midiin.setCallback(&_midiMessageCallback, this);
		midiin.openVirtualPort(portName);
	}
	catch(RtError& err) {
		ofLog(OF_LOG_ERROR, "ofxMidiIn: couldn't open virtual port \"%s\": %s",
			portName.c_str(), err.what());
		return false;
	}
	this->portName = portName;
	bOpen = true;
	bVirtual = true;
	ofLog(OF_LOG_VERBOSE, "ofxMidiOut: opened virtual port %s", portName.c_str());
	return true;
}

// --------------------------------------------------------------------------------------
void ofxMidiIn::closePort() {
	if(bVirtual && bOpen) {
		ofLog(OF_LOG_VERBOSE, "ofxMidiIn: closing virtual port %s", portName.c_str());
	}
	else if(portNum > -1) {
		ofLog(OF_LOG_VERBOSE, "ofxMidiIn: closing port %i %s", portNum, portName.c_str());
	}
	midiin.closePort();
	if(bOpen)
		midiin.cancelCallback();
	portNum = -1;
	portName = "";
	bOpen = false;
	bVirtual = false;
}

// --------------------------------------------------------------------------------------
int ofxMidiIn::getPort() {
	return portNum;
}

// --------------------------------------------------------------------------------------
string ofxMidiIn::getName() {
	return portName;
}

// --------------------------------------------------------------------------------------
bool ofxMidiIn::isOpen() {
	return bOpen;
}

// --------------------------------------------------------------------------------------
void ofxMidiIn::ignoreTypes(bool midiSysex, bool midiTiming, bool midiSense) {
	midiin.ignoreTypes(midiSysex, midiTiming, midiSense);
	ofLog(OF_LOG_VERBOSE, "ofxMidiIn: ignore types on %s: sysex: %d timing: %d sense: %d",
			portName.c_str(), midiSysex, midiTiming, midiSense);
}

// --------------------------------------------------------------------------------------
void ofxMidiIn::addListener(ofxMidiListener* listener) {
	ofAddListener(newMessageEvent, listener, &ofxMidiListener::newMidiMessage);
}

// --------------------------------------------------------------------------------------
void ofxMidiIn::removeListener(ofxMidiListener* listener) {
	ofRemoveListener(newMessageEvent, listener, &ofxMidiListener::newMidiMessage);
}

// --------------------------------------------------------------------------------------
void ofxMidiIn::setVerbose(bool verbose) {
	bVerbose = verbose;
}

// PRIVATE
// --------------------------------------------------------------------------------------
// TODO: replace cout with ofLogNotice, etc?
void ofxMidiIn::manageNewMessage(double deltatime, vector<unsigned char> *message) {
			
	// parse message and fill event
	ofxMidiMessage midiMessage(message);

	if((message->at(0)) >= MIDI_SYSEX) {
		midiMessage.status = (MidiStatus)(message->at(0) & 0xFF);
		midiMessage.channel = 0;
	} else {
		midiMessage.status = (MidiStatus) (message->at(0) & 0xF0);
		midiMessage.channel = (int) (message->at(0) & 0x0F)+1;
	}
	
	midiMessage.deltatime = deltatime * 1000; // convert s to ms
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
		cout << midiMessage.toString() << endl;
	}
	
	// send event to listeners
	ofNotifyEvent(newMessageEvent, midiMessage, this);
}

// --------------------------------------------------------------------------------------
void ofxMidiIn::_midiMessageCallback(double deltatime, vector<unsigned char> *message, void *userData) {
	((ofxMidiIn*) userData)->manageNewMessage(deltatime, message);
}
