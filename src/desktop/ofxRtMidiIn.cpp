#include "ofxRtMidiIn.h"

// -----------------------------------------------------------------------------
ofxRtMidiIn::ofxRtMidiIn(const string name) :
	ofxBaseMidiIn(name), midiin(name) {}

// -----------------------------------------------------------------------------
ofxRtMidiIn::~ofxRtMidiIn() {
	closePort();
}

// -----------------------------------------------------------------------------
// TODO: replace cout with ofLogNotice when OF_LOG_NOTICE is the default log level
void ofxRtMidiIn::listPorts() {
	cout << "ofxMidiIn: " << midiin.getPortCount() << " ports available" << endl;
	for(unsigned int i = 0; i < midiin.getPortCount(); ++i){
		cout << "ofxMidiIn: " <<  i << ": " << midiin.getPortName(i) << endl;
	}
}

// -----------------------------------------------------------------------------
vector<string>& ofxRtMidiIn::getPortList() {
	portList.clear();
	for(unsigned int i=0; i < midiin.getPortCount(); ++i) {
		portList.push_back(midiin.getPortName(i));
	}
	return portList;
}

// -----------------------------------------------------------------------------
int ofxRtMidiIn::getNumPorts() {
	return midiin.getPortCount();
}

// -----------------------------------------------------------------------------
string ofxRtMidiIn::getPortName(unsigned int portNumber) {
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

// -----------------------------------------------------------------------------
bool ofxRtMidiIn::openPort(unsigned int portNumber) {	
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

// -----------------------------------------------------------------------------
bool ofxRtMidiIn::openPort(string deviceName) {
	
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

// -----------------------------------------------------------------------------
bool ofxRtMidiIn::openVirtualPort(string portName) {
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

// -----------------------------------------------------------------------------
void ofxRtMidiIn::closePort() {
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

// -----------------------------------------------------------------------------
void ofxRtMidiIn::ignoreTypes(bool midiSysex, bool midiTiming, bool midiSense) {
	midiin.ignoreTypes(midiSysex, midiTiming, midiSense);
	ofLog(OF_LOG_VERBOSE, "ofxMidiIn: ignore types on %s: sysex: %d timing: %d sense: %d",
			portName.c_str(), midiSysex, midiTiming, midiSense);
}
// -----------------------------------------------------------------------------
void ofxRtMidiIn::_midiMessageCallback(double deltatime, vector<unsigned char> *message, void *userData) {
	((ofxRtMidiIn*) userData)->manageNewMessage(deltatime * 1000, message); // convert s to ms
}
