#include "ofxRtMidiOut.h"

// -----------------------------------------------------------------------------
ofxRtMidiOut::ofxRtMidiOut(const string name) :
	ofxBaseMidiOut(name), midiout(name) {}

// -----------------------------------------------------------------------------
ofxRtMidiOut::~ofxRtMidiOut() {
	closePort();
}

// -----------------------------------------------------------------------------
// TODO: replace cout with ofLogNotice when OF_LOG_NOTICE is the default log level
void ofxRtMidiOut::listPorts() {
	cout << "ofxMidiOut: " << midiout.getPortCount() << " ports available" << endl;
	for(unsigned int i = 0; i < midiout.getPortCount(); ++i){
		cout << "ofxMidiOut: " <<  i << ": " << midiout.getPortName(i) << endl;
	}
}

// -----------------------------------------------------------------------------
vector<string>& ofxRtMidiOut::getPortList() {
	portList.clear();
	for(unsigned int i = 0; i < midiout.getPortCount(); ++i) {
		portList.push_back(midiout.getPortName(i));
	}
	return portList;
}

// -----------------------------------------------------------------------------
int ofxRtMidiOut::getNumPorts() {
	return midiout.getPortCount();
}

// -----------------------------------------------------------------------------
string ofxRtMidiOut::getPortName(unsigned int portNumber) {
	// handle rtmidi exceptions
	try {
		return midiout.getPortName(portNumber);
	}
	catch(RtError& err) {
		ofLog(OF_LOG_ERROR, "ofxMidiOut: couldn't get name for port %i: %s",
			portNumber, err.what());
	}
	return "";
}

// -----------------------------------------------------------------------------
bool ofxRtMidiOut::openPort(unsigned int portNumber) {	
	// handle rtmidi exceptions
	try {
		closePort();
		midiout.openPort(portNumber);
	}
	catch(RtError& err) {
		ofLog(OF_LOG_ERROR, "ofxMidiOut: couldn't open port %i: %s", portNumber, err.what());
		return false;
	}
	portNum = portNumber;
	portName = midiout.getPortName(portNumber);
	bOpen = true;
	ofLog(OF_LOG_VERBOSE, "ofxMidiOut: opened port %i %s", portNum, portName.c_str());
	return true;
}

// -----------------------------------------------------------------------------
bool ofxRtMidiOut::openPort(string deviceName) {
	
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
		ofLog(OF_LOG_ERROR, "ofxMidiOut: port \"%s\" is not available", deviceName.c_str());
		return false;
	} 
	
	return openPort(port);
}

// -----------------------------------------------------------------------------
bool ofxRtMidiOut::openVirtualPort(string portName) {
	// handle rtmidi exceptions
	try {
		closePort();
		midiout.openVirtualPort(portName);
	}
	catch(RtError& err) {
		ofLog(OF_LOG_ERROR, "ofxMidiOut: couldn't open virtual port \"%s\": %s",
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
void ofxRtMidiOut::closePort() {
	if(bVirtual && bOpen) {
		ofLog(OF_LOG_VERBOSE, "ofxMidiOut: closing virtual port %s", portName.c_str());
	}
	else if(bOpen && portNum > -1) {
		ofLog(OF_LOG_VERBOSE, "ofxMidiOut: closing port %i %s", portNum, portName.c_str());
	}
	midiout.closePort();
	portNum = -1;
	portName = "";
	bOpen = false;
	bMsgInProgress = false;
	bVirtual = false;
}

// PRIVATE
// -----------------------------------------------------------------------------
void ofxRtMidiOut::sendMessage() {
	// handle rtmidi exceptions
	try {
		midiout.sendMessage(&message);
	}
	catch(RtError& err) {
		ofLog(OF_LOG_ERROR, "ofxMidiOut: couldn't send message: %s", err.what());
	}
	bMsgInProgress = false;
}
